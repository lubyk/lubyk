--[[------------------------------------------------------

  midi.Out
  --------

  Create/open a midi port.

--]]------------------------------------------------------
require 'midi.Out_core'
local constr = midi.Out
local mt     = midi.Out_

function midi.Out(port_or_name)
  local self = constr()
  if type(port_or_name) == 'string' then
    -- open a virtual port
    self:virtualPort(port_or_name)
  else
    self:openPort(port_or_name)
  end
  return self
end

function mt:sendNote(channel, note, velocity, length)
  if channel > 16 then
    channel = 16
  elseif channel < 1 then
    channel = 1
  end
  self:send(
    -- NoteOn
    144 + channel - 1,
    note,
    velocity
  )
  if length then
    self:sendAt(sched.now + length, {
      -- NoteOff
      128 + channel - 1,
      note,
      velocity
    })
  end
end

function mt:sendAt(time, msg)
  local prev = self
  local at_top
  -- Get current top event
  if prev.at_next then
    at_top = prev.at_next.at
  else
    at_top = 0
  end
  msg.at = time
  -- sorted insert (next event first)
  while true do
    local ne = prev.at_next
    if not ne then
      prev.at_next = msg
      msg.at_next = nil
      break
    elseif at < ne.at then
      prev.at_next = msg
      msg.at_next  = ne
      break
    else
      prev = ne
    end
  end
  if time < at_top then
    -- reschedule sender thread
    -- FIXME: code not finished
    assert(false)
    sched:scheduleAt(time, self.thread)
  end
end

--[[
  virtual LuaStackSize unpack(lua_State *L, void *data) {
    MsgVector *message = (MsgVector*)data;
    if (message.size() == 0) return 0;  // no message
    
    lua_newtable(L);
    // <tbl>
    // type = 'xxxx'
    lua_pushstring(L, "type");
    switch(message[0]) {
      case 0xfa:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Start");
        lua_settable(L, -3);
        break;
      case 0xfc:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Stop");
        lua_settable(L, -3);
        break;
      case 0xf8:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Tick");
        lua_settable(L, -3);
        break;
      case 0xfb:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Continue");
        lua_settable(L, -3);
        break;
      default:
        // FIXME: other messages not implemented yet.
        unsigned char channel = message[0];
        if (channel >= 0x90) {
          unsigned int velocity = message[2];
          if (velocity > 0) {
            lua_pushstring(L, "NoteOn");
          } else {
            lua_pushstring(L, "NoteOff");
          }
          lua_settable(L, -3);
          lua_pushstring(L, "channel");
          lua_pushnumber(L, channel - 0x90 + 1);
          lua_settable(L, -3);

          lua_pushstring(L, "note");
          lua_pushnumber(L, message[1]);
          lua_settable(L, -3);

          lua_pushstring(L, "velocity");
          lua_pushnumber(L, velocity);
          lua_settable(L, -3);
        } else if (channel >= 0x80) {
          lua_pushstring(L, "NoteOff");
          lua_settable(L, -3);
          lua_pushstring(L, "channel");
          lua_pushnumber(L, channel - 0x80 + 1);
          lua_settable(L, -3);

          lua_pushstring(L, "note");
          lua_pushnumber(L, message[1]);
          lua_settable(L, -3);

          lua_pushstring(L, "velocity");
          lua_pushnumber(L, message[2]);
          lua_settable(L, -3);
        } else if (channel >= 0xB0) {
          lua_pushstring(L, "Ctrl");
          lua_settable(L, -3);
          lua_pushstring(L, "channel");
          lua_pushnumber(L, channel - 0x80 + 1);
          lua_settable(L, -3);
          
          lua_pushstring(L, "ctrl");
          lua_pushnumber(L, message[1]);
          lua_settable(L, -3);

          lua_pushstring(L, "value");
          lua_pushnumber(L, message[2]);
          lua_settable(L, -3);
        } else {  
          fprintf(stderr, "unknown message type %i.\n", (int)channel);
          return 0;
        }
    }
    return 1;
  }
--]]
