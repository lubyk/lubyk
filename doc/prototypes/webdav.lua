--------------------------------------------------------------------------------
-- Title:               WikiDAV.lua
-- Description:         Like a square peg in a round hole
-- Author:              Raphaël Szwarc http://alt.textdrive.com/lua/
-- Creation Date:       January 30, 2007
-- Legal:               Copyright (C) 2007 Raphaël Szwarc
--                      Under the terms of the MIT License
--                      http://www.opensource.org/licenses/mit-license.html
--                      http://alt.textdrive.com/nanoki/
--------------------------------------------------------------------------------

-- import dependencies
local HTTP = require( 'HTTP' )
local HTTPExtra = require( 'HTTPExtra' )
local HTTPResponse = require( 'HTTPResponse' )
local URL = require( 'URL' )
local URLPath = require( 'URLPath' )
local WikiService = require( 'WikiService' )

local Encode = WikiService.Encode

local os = require( 'os' )

local assert = assert
local getmetatable = getmetatable
local module = module
local pairs = pairs
local require = require
local setmetatable = setmetatable
local tonumber = tonumber
local tostring = tostring

--------------------------------------------------------------------------------
-- HTTPResponse (Extra)
--------------------------------------------------------------------------------

module( 'HTTPResponse' )

local self = _M


function WebDAVFilter( aRequest, aResponse )
    aResponse.header[ 'dav' ] = '1, 2'
    aResponse.header[ 'ms-author-via' ] = 'DAV'
end

self.filter[ #self.filter + 1 ] = WebDAVFilter

--------------------------------------------------------------------------------
-- WikiDAV
--------------------------------------------------------------------------------

module( 'WikiDAV' )
_VERSION = '1.0'

local self = setmetatable( _M, {} )
local meta = getmetatable( self )

--------------------------------------------------------------------------------
-- Utitities
--------------------------------------------------------------------------------

local function Status( aCode, aDescription )
    HTTP.response.status.code = aCode
    HTTP.response.status.description = aDescription
    HTTP.response.header[ 'content-type' ] = 'text/plain'
    
    return ( '%d %s' ):format( aCode, aDescription )
end

local function NamedResource( aResource )
    if not aResource.name then
        local aPath = HTTP.request.url.path
        
        aResource = aPath[ #aPath ]
    end
    
    return aResource
end

local function Href( aResource, isFirst )
    local anURL = HTTP.request.url
    local aName = aResource.name or anURL.path[ #anURL.path ] or '/'
    local aPath = anURL.path( aResource.name )
    
    if isFirst then
        aPath = anURL.path
    end
    
    aPath = URLPath( aPath )
    aPath.directory = aResource.mode == 'directory'
    aResource.name = aName
    
    return tostring( aPath )
end

local function ContentType( aResource )
    if aResource.mode == 'file' then
        local MIME = require( 'MIME' )
        local MIMEType = require( 'MIMEType' )
        local aName = aResource.name
        local anExtension = ( aName:match( '^.+%.(%w+)$' ) or '' ):lower()
        
        return MIMEType[ anExtension ] or 'application/octet-stream'
    end
    
    return 'text/html'
end

local function ETag( aResource, aLocation )
    local sha1 = require( 'sha1' )
    local aTag = ( 'DAV:%s:%s:%d:%d' ):format( aLocation, aResource.name, aResource.modification, aResource.size )
    
    return sha1.digest( aTag )
end

local function ResourceType( aResource )
    if aResource.mode == 'directory' then
        return '<collection/>'
    end
end

local function ResourceIterator( aResource, aLimit )
    local anIterator = aResource.iterator
    local isFirst = true
    local aCount = 0
    
    return function()
        if aCount >= aLimit then
            return nil
        end
    
        aCount = aCount + 1
    
        if isFirst then
            isFirst = false
        
            return aResource, true
        end
        
        if anIterator then
            return anIterator(), false
        end
    end
end

--------------------------------------------------------------------------------
-- Service methods
--------------------------------------------------------------------------------

function self:propfind()
    local aDepth = tonumber( HTTP.request.header[ 'depth' ] )
    
    if aDepth and aDepth >= 0 and aDepth <= 1 then
        local sha1 = require( 'sha1' )
        local Template = require( 'Template' )
        local aTemplate = Template[ 'WikiDAV.txt' ]
        local anEtag = 'DAV'
        local aModification = 0
        
        for aResource, isFirst in ResourceIterator( self.resource, self.limit ) do
            local aResourceTemplate = aTemplate[ 'resources' ]
            local aResourceHref = Href( aResource, isFirst )
            local aResourceEtag = ETag( aResource, aResourceHref )
            
            aResourceTemplate[ 'href' ] = Encode( aResourceHref )
            aResourceTemplate[ 'displayName' ] = Encode( aResource.name )
            aResourceTemplate[ 'contentLength' ] = aResource.size
            aResourceTemplate[ 'contentType' ] = ContentType( aResource )
            aResourceTemplate[ 'etag' ] = aResourceEtag
            aResourceTemplate[ 'creationDate' ] = os.date( '!%a, %d %b %Y %H:%M:%S GMT', aResource.creation or aResource.modification )
            aResourceTemplate[ 'lastModified' ] = os.date( '!%a, %d %b %Y %H:%M:%S GMT', aResource.modification )
            aResourceTemplate[ 'lockToken' ] = Encode( sha1.digest( ( 'DAV:LOCK:%s' ):format( tostring( aResourceHref ) ) ) )
            aResourceTemplate[ 'resourceType' ] = ResourceType( aResource )
            
            aTemplate[ 'resources' ] = aResourceTemplate
            
            anEtag = sha1.digest( ( '%s:%s' ):format( anEtag, aResourceEtag ) )
            
            if aResource.modification > aModification then
                aModification = aResource.modification
            end
            
            if aDepth == 0 then
                break
            end
        end

        HTTP.response.status.code = 207
        HTTP.response.status.description = 'Multi-Status'
        HTTP.response.header[ 'content-type' ] = 'application/xml; charset=utf-8' 
        HTTP.response.header[ 'etag' ] = anEtag
        HTTP.response.header[ 'last-modified' ] = os.date( '!%a, %d %b %Y %H:%M:%S GMT', aModification )
        
        return tostring( aTemplate )
    end

    return Status( 403, 'Forbidden' )
end

--------------------------------------------------------------------------------
-- Metamethods
--------------------------------------------------------------------------------

function meta:__call( aResource, aLimit )
    local aService = { resource = assert( aResource ), limit = aLimit or 1000 }
    
    setmetatable( aService, self )
    self.__index = self    
    
    return aService
end

function meta:__concat( aValue )
    return tostring( self ) .. tostring( aValue )
end

function meta:__tostring()
    return ( '%s/%s' ):format( self._NAME, self._VERSION )
end

