require 'lubyk'

srv = lk.DavServer(1024)
cache = {}

local function mockResource(path, body)
  local self = {davProperty = rawget, path = path, body = body or ''}
  if path == '/' then
    self.resourcetype = {xml = 'collection'}
    self.getlastmodified = 9298347
    self.creationdate    = 8909234
    return self
  end
  self.getcontentlength = string.len(self.body)
  self.contenttype = 'text/plain'
  self.getlastmodified = 9298447
  self.creationdate    = 8909334
  return self
end

function srv:find(path)
  print(string.format("srv:resource '%s'", path))
  -- mock a resource
  if path == '/' or path == '/foo.lua' or path == '/bar.lua' then
    local res = cache[path]
    if not res then
      res = mockResource(path)
      cache[path] = res
    end
    return res
  end
end

function srv:findChildren(resource)
  if resource.path == '/' then
    if not resource.children then
      resource.children = {
        self:find('/foo.lua'),
        self:find('/bar.lua'),
      }
      setmetatable(resource.children, {__mode='v'})
    end
    return resource.children
  end
end

function srv:update(resource, content)
  resource.body = content or ''
  resource.getcontentlength = string.len(resource.body)
  return nil, {status = "200"}
end

function srv:create(path, content)
  if string.match(path, '[a-z]+.lua') then
    local res = mockResource(path, content)
    cache[path] = res
    table.insert(self:findChildren(self:find('/')), res)
    return nil, {status = "201"}
  else
    return nil, {status = "400"}
  end
end

function srv:delete(resource)
  cache[resource.path] = nil
  collectgarbage()
  return nil, {status = "204"}
end

print(string.format("Starting server on port %i...\nConnect with: http://localhost:%i", srv.port, srv.port))
srv:listen()
