--[[------------------------------------------------------

  lk.DavServer
  ------------

  This is a simple callback based WebDav server. It is not
  meant for heavy traffic or lots of clients.

--]]------------------------------------------------------
local lib = {type='lk.DavServer'}
lib.__index  = lib
lk.DavServer = lib


-- the DavServer is an extension of the WebServer
for k, v in pairs(lk.WebServer) do
  if k ~= 'type' and k ~= '__index' then
    lib[k] = v
  end
end

setmetatable(lib, {
  -- new method
 __call = function(lib, port)
  local self = {
    should_run = true,
    server     = lk.Socket(),
  }
  self.server:bind('*', port or 0)
  self.server:listen()
  self.host, self.port = self.server:localHost(), self.server:localPort()
  self.href_base = 'http://' .. self.host .. ':' .. self.port
  setmetatable(self, lib)
  return self
end})

--------------------- PRIVATE
local DATE_FORMAT_HTTP_DATE = '!%a, %d %b %Y %H:%M:%S GMT'
lib.DATE_FORMAT_HTTP_DATE = DATE_FORMAT_HTTP_DATE

local DATE_FORMAT_ISO_8601 = '%Y-%m-%dT%H:%M:%SZ'
lib.DATE_FORMAT_ISO_8601 = DATE_FORMAT_ISO_8601

-- More status codes for WebDav
lib.STATUS_CODES['207'] = 'Multi-Status'

function lib:OPTIONS(request)
  return nil, {
    DAV = "1, 2"
  }
end

local function etag(...)
  -- TODO: sha1
  return lk.crypto.sha1(string.format(...))
end

local function getResourceType(is_dir)
  if is_dir then
    return {xml = 'collection'}
  end
end

local function getContentLength(is_dir)
  if is_dir then
    return 2
  else
    return 0
  end
end

local function isDirectory(resource)
  local rtype = resource:davProperty('resourcetype')
  if rtype then
    return rtype.xml == 'collection'
  end
end

local function makeResponse(self, resource, prop_list)
  local prop = {xml='prop'}
  for _, k in ipairs(prop_list) do
    k = k.xml
    local v = resource:davProperty(k)
    if v then
      if k == 'creationdate' then
        v = os.date(DATE_FORMAT_ISO_8601, v)
      elseif k == 'getlastmodified' then
        v = os.date(DATE_FORMAT_HTTP_DATE, v)
      end
    end
    table.insert(prop, {xml = k, v or ''})
  end
  return {
    xml = 'response',
    {xml = 'href', self.href_base .. resource:davProperty('path')},
    {xml = 'propstat',
      prop,
      {xml = 'status', "HTTP/1.1 200 OK"},
    }
  }
end

local function getPropList(body)
  local data = xml.parse(body or '')
  if not data then
    return
  end
  xml.removeNamespace(data, 'DAV:')
  return data:find('prop')
end

function lib:PROPFIND(request)
  local resource = self:find(request.path)
  local prop_list = getPropList(request.body)
  if not resource or not prop_list then
    return nil, {status = '404'}
  end
  
  if params then

  end

  local headers = request.headers
  local depth = tonumber(headers['Depth'])

  local res = {
    xml = 'multistatus',
    xmlns = 'DAV:'
  }
  -- use oldest change to build etag
  local last_mod = nil

  table.insert(res, makeResponse(self, resource, prop_list))
  if isDirectory(resource) and depth == 1 then
    local children = self:findChildren(resource)
    for _, child in ipairs(children) do
      table.insert(res, makeResponse(self, child, prop_list))
    end
  end

  local body = "<?xml version='1.0' encoding='utf-8'?>\n" .. xml.encode(res)

  headers = {
    status = 207,
    Etag   = etag(body),
    ['Content-Type']  = 'text/xml; charset="utf-8"',
    --['Last-Modified'] =  os.date(DATE_FORMAT, last_mod)
  }

  return body, headers
end

function lib:GET(request)
  local resource = self:find(request.path)
  if not resource then
    return nil, {status = '404'}
  end

  return resource.body, {status=200, 
    ['Content-type']=resource:davProperty('contenttype') or 'text/plain'
  }
end

function lib:PUT(request)
  local resource = self:find(request.path)
  if resource then
    return self:update(resource, request.body)
  else
    return self:create(request.path, request.body)
  end
end

function lib:DELETE(request)
  local resource = self:find(request.path)
  if not resource then
    return nil, {status = '204'}
  end
  return self:delete(resource)
end

function lib:find(path)
  error("'find(path)' callback not implemented for DAVServer")
end

function lib:find(path)
  error("'findChildren(path)' callback not implemented for DAVServer")
end

function lib:create(path, content)
  -- forbidden
  return nil, {status = "403"}
end

function lib:update(resource, content)
  -- forbidden
  return nil, {status = "403"}
end

function lib:delete(resource)
  error("'delete(resource)' callback not implemented for DAVServer")
end
