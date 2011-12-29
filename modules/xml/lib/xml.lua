--[[------------------------------------------------------

  xml
  ---

  LuaXML 1.7.4 Copyright (C) 2007-2010 Gerald Franz, www.viremo.de

  This is a slightly modified version by Gaspard Bucher.
  Changes:
    * uses the 'xml' attribute for the tag name instead of [0].
    * uses 'encode' and 'decode' methods instead of 'str' and
      'eval'.
    * added xml.removeNamespace

--]]------------------------------------------------------
require 'LuaXml'

local function doRemoveNamespace(data, prefix)
  data.xml = string.match(data.xml, prefix .. ':(.*)') or data.xml
  for _, sub in ipairs(data) do
    if type(sub) == 'table' then
      doRemoveNamespace(sub, prefix)
    end
  end
end

-- This function finds the xmlns:[NAME]='[namespace_value]' declaration
-- and removes [NAME]: from the tag names.
-- Example with this input:
--   <foo:document xmlns:foo='bar'>
--     <foo:name>Blah</foo:name>
--   </foo:document>
-- The command xml.removeNamespace(data, 'bar') produces
--   <document>
--     <name>Blah</name>
--   </document>
function xml.removeNamespace(data, namespace_value)
  local nm
  for k, v in pairs(data) do
    if v == namespace_value then
      nm = string.match(k, 'xmlns:(.*)')
      if nm == '' then
        -- error
        return
      else
        doRemoveNamespace(data, nm)
      end
    end
  end
end

