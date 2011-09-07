require 'lubyk'

srv = lk.DavServer(1024)
root = lk.ResourceFile(arg[1])
srv.cache = {
 ['/'] = root,
}
-- when caching children nodes, we count on the parent to cache entries
setmetatable(srv.cache, {__mode = 'v'})

function srv:find(href)
  local rez = srv.cache[href]
  if not rez then
    -- this only happens on special files because we list children
    -- (and cache result) before we are asked for a path.
    rez = lk.ResourceFile(root.path .. href, root.path)
    srv.cache[href] = rez
  end
  return rez
end

function srv:findChildren(resource)
  local children = resource:children()
  if not children.cached then
    for _, child in ipairs(children) do
      self.cache[child.href] = child
    end
    children.cached = true
  end
  return children
end

function srv:update(resource, content)
  if resource:update(content) then
    return nil, {status = '200'}
  else
    return nil, {status = '400'}
  end
end

function srv:create(parent, name, content)
  if string.match(name, '[a-z]+.lua$') then
    -- create resource
    if parent:createChild(name, content) then
      return nil, {status = '201'}
    end
  end
  return nil, {status = '400'}
end

function srv:delete(parent, name)
  if parent:deleteChild(name) then
    -- clear cache
    collectgarbage()
    return nil, {status = '204'}
  end
  return nil, {status = '400'}
end

print(string.format("Starting server on port %i...\nConnect with: http://localhost:%i", srv.port, srv.port))
srv:listen()
