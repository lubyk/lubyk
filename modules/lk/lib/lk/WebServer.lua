--[[------------------------------------------------------

  lk.WebServer
  ------------

  This is a simple callback based web server. It is not
  meant for heavy traffic or lots of clients.

--]]------------------------------------------------------
local lib = {type='lk.WebServer'}
lib.__index  = lib
lk.WebServer = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, port)
  local self = {
    should_run = true,
    server     = lk.Socket(),
  }
  self.server:bind()
  self.server:listen()
  self.host, self.port = self.server:localHost(), self.server:localPort()
  setmetatable(self, lib)
  return self
end})

lib.STATUS_CODES = {
  ["100"]  = "Continue",
  ["101"]  = "Switching Protocols",
  ["200"]  = "OK",
  ["201"]  = "Created",
  ["202"]  = "Accepted",
  ["203"]  = "Non-Authoritative Information",
  ["204"]  = "No Content",
  ["205"]  = "Reset Content",
  ["206"]  = "Partial Content",
  ["300"]  = "Multiple Choices",
  ["301"]  = "Moved Permanently",
  ["302"]  = "Found",
  ["303"]  = "See Other",
  ["304"]  = "Not Modified",
  ["305"]  = "Use Proxy",
  ["307"]  = "Temporary Redirect",
  ["400"]  = "Bad Request",
  ["401"]  = "Unauthorized",
  ["402"]  = "Payment Required",
  ["403"]  = "Forbidden",
  ["404"]  = "Not Found",
  ["405"]  = "Method Not Allowed",
  ["406"]  = "Not Acceptable",
  ["407"]  = "Proxy Authentication Required",
  ["408"]  = "Request Time-out",
  ["409"]  = "Conflict",
  ["410"]  = "Gone",
  ["411"]  = "Length Required",
  ["412"]  = "Precondition Failed",
  ["413"]  = "Request Entity Too Large",
  ["414"]  = "Request-URI Too Large",
  ["415"]  = "Unsupported Media Type",
  ["416"]  = "Requested range not satisfiable",
  ["417"]  = "Expectation Failed",
  ["500"]  = "Internal Server Error",
  ["501"]  = "Not Implemented",
  ["502"]  = "Bad Gateway",
  ["503"]  = "Service Unavailable",
  ["504"]  = "Gateway Time-out",
  ["505"]  = "HTTP Version not supported",
}

local function buildReply(body, headers)
  local status = tostring(headers.status or "200")
  local status_msg = lib.STATUS_CODES[status]
  if not status_msg then
    --print("Bad HTTP status", status)
    return nil
  end
  local res = string.format("HTTP/1.1 %s %s\r\n", status, status_msg)
  res = res .. string.format("Server: lubyk 0.1\r\n")
  res = res .. "Connection: close\r\n"

  for k, v in pairs(headers) do
    if k ~= 'status' then
      res = res .. string.format("%s: %s\r\n", k, v)
    end
  end
  if body then
    res = res .. string.format("Content-Length: %i\r\n", string.len(body))
    res = res .. '\r\n' .. body
  else
    res = res .. "Content-Length: 0\r\n"
  end
  --print("--------------------------------------------------")
  --print(res)
  return res
end

-- TODO: protection against DOS attack ?
local function getRequest(client)
  local request = {}
  --print("==================================================")
  local line, err = client:recv()
  if err then
    return nil, err
  end
  --print('STATUS', line)
  -- GET /foo/bar.html HTTP/1.1
  request.method, request.path, request.http = string.match(line, '(%S+)%s+(%S+)%s+(%S+)')
  request.path = string.match(request.path, '^(.*)/$') or request.path

  local headers = {}
  request.headers = headers
  while true do
    local line, err = client:recv()
    if err then
      return nil, err
    end
    --print('HEADER', line)

    local key, value = string.match(line, '([^:]+): (.*)')
    if key then
      headers[key] = value
    else
      break
    end
  end

  local content_length = tonumber(headers['Content-Length']) or 0
  if content_length > 0 then
    request.body, err = client:recv(content_length)
    if err then
      return nil, err
    end
    --print('BODY', request.body)
  end
  return request
end

function lib:listen()
  local server = self.server
  while self.should_run do
    local client = server:accept()
    -- receive request
    local request, err = getRequest(client)
    if err then
      print("error: " .. tostring(err))
    else
      local fun = self[request.method]
      if fun then
        local body, headers = fun(self, request)
        if body or headers then
          client:send(buildReply(body, headers))
        end
      else
        print(string.format("Could not handle '%s' method with url '%s'.", request.method, request.path))
      end
    end
    client:close()
  end
end

function lib:GET(request)
  return "<html><head><title>YO</title></head><body>Hello World</body>", {status=200, ['Content-type']='text/html'}
end

