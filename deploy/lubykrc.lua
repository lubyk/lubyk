local settings = {}
-- additional paths
local base = os.getenv('HOME') .. '/Dropbox/LUBYK_mac'
settings.paths = {
  -- Dropbox edge
  base .. '/?.lua',
}

settings.cpaths = {
  -- Dropbox edge
  base .. '/?.so',
}

settings.editor = {
  editor_cmd = os.getenv('EDITOR') or 'mate',
  library_sources = {
    lubyk = base .. '/lubyk',
  }
}
return settings
