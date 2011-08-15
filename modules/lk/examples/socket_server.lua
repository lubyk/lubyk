require 'lubyk'

srv = lk.Socket()
srv:bind()
print("Server running on port", srv:port())
srv:listen()

clients = {}

while true do
  local cli = srv:accept()
  table.insert(clients, lk.Thread(function()
    while true do
      local msg = cli:recv()
      print(msg)
      if not msg or msg == 'quit' then
        break
      end
    end
  end))
end

