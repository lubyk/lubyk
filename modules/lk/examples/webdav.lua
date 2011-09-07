require 'lubyk'

srv = lk.DavServer(1024)
root = lk.FileResource(arg[1])
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
    rez = lk.FileResource(root.path .. href, root.path)
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
  -- create resource
  if parent:createChild(name, content) then
    return nil, {status = '201'}
  end
  return nil, {status = '400'}
end

function srv:delete(parent, resource)
  if parent:deleteChild(resource) then
    -- clear cache
    collectgarbage()
    return nil, {status = '204'}
  end
  return nil, {status = '400'}
end

function srv:move(parent, resource, dest_parent, dest_name)
  if parent:moveChild(resource, dest_parent, dest_name) then
    return nil, {status = '201'}
  else
    return nil, {status = '400'}
  end
end

print(string.format("Starting server on port %i...\nConnect with: http://localhost:%i", srv.port, srv.port))
srv:listen()
