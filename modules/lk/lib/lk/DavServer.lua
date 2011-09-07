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
 __call = function(lib, port, root)
  local self = {
    should_run = true,
    server     = lk.Socket(),
  }
  self.server:bind('*', port or 0)
  self.server:listen()
  self.host, self.port = self.server:localHost(), self.server:localPort()
  self.href_base = 'http://' .. self.host .. ':' .. self.port
  setmetatable(self, lib)
  if root then
    self:setRoot(root)
  else
    -- if no root path is provided, all the functions (find, findChildren,
    -- update, create, delete and move) should be implemented.
  end
  return self
end})

function lib:setRoot(root)
  if type(root) == 'string' then
    self.root  = lk.FileResource(root, root)
  else
    self.root = root
  end
  self.cache = setmetatable({}, {__mode = 'v'})
end

--=============================================== Functions to sub-class
function lib:find(href)
  local rez = self.cache[href]
  if not rez then
    -- this only happens on special files because we list children
    -- (and cache result) before we are asked for a path.
    rez = lk.FileResource(self.root.path .. href, self.root.path)
    self.cache[href] = rez
  end
  return rez
end

function lib:findChildren(resource)
  local children = resource:children()
  if not children.cached then
    for _, child in ipairs(children) do
      self.cache[child.href] = child
    end
    children.cached = true
  end
  return children
end

function lib:update(resource, content)
  if resource:update(content) then
    return nil, {status = '200'}
  else
    return nil, {status = '400'}
  end
end

function lib:create(parent, name, content)
  -- create resource
  if parent:createChild(name, content) then
    return nil, {status = '201'}
  end
  return nil, {status = '400'}
end

function lib:delete(parent, resource)
  if parent:deleteChild(resource) then
    -- clear cache
    collectgarbage()
    return nil, {status = '204'}
  end
  return nil, {status = '400'}
end

function lib:move(parent, resource, dest_parent, dest_name)
  if parent:moveChild(resource, dest_parent, dest_name) then
    return nil, {status = '201'}
  else
    return nil, {status = '400'}
  end
end

--=============================================== PRIVATE

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
    {xml = 'href', self.href_base .. resource:davProperty('href')},
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
    Etag   = etag('%s', body),
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

  return resource:body(), {status=200, 
    ['Content-type']=resource:davProperty('contenttype') or 'text/plain'
  }
end

local function getParent(self, path)
  local base, name = lk.directory(path)
  return self:find(base), name
end
  
function lib:PUT(request)
  local resource = self:find(request.path)
  if resource then
    return self:update(resource, request.body)
  else
    local parent, name = getParent(self, request.path)
    if parent then
      return self:create(parent, name, request.body or '')
    else
      return nil, {status = '400'}
    end
  end
end

function lib:DELETE(request)
  local resource = self:find(request.path)
  if not resource then
    return nil, {status = '204'}
  end
  local parent = getParent(self, request.path)
  if parent then
    return self:delete(parent, resource)
  else
    return nil, {status = '400'}
  end
end

function lib:MOVE(request)
  local resource = self:find(request.path)
  local destination = request.headers['Destination'] or ''
  destination = string.match(destination, 'http://[^/]+(/.+)$')
  if destination then
    local parent = getParent(self, request.path)
    local dest_parent, dest_name = getParent(self, destination)
    return self:move(parent, resource, dest_parent, dest_name)
  end
  return nil, {status = '400'}
end

function lib:MKCOL(request)
  local resource = self:find(request.path)
  if not resource then
    local parent, name = getParent(self, request.path)
    return self:create(parent, name)
  elseif resource.is_dir then
    return nil, {status = '201'}
  end
  return nil, {status = '400'}
end
