require 'lubyk'

local port = ...
cli = lk.Socket()
cli:connect('127.0.0.1', port)
while true do
  local line = io.read()
  cli:send(line)
  if line == 'quit' then
    break
  end
end

