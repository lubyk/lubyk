--[[------------------------------------------------------

  yaml
  ----

  FIXME: description for module 'yaml'

--]]------------------------------------------------------
yaml = Autoload('yaml')
require 'yaml.vendor'

function yaml.loadpath(path)
  return yaml.load(lk.readall(path))
end
