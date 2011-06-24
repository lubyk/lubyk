-- This file is needed for inline Patch testing (loaded classes need to be
-- in the same folder as the patch definition "Patch_test.lua").
inlet('value', 'Stores the value')

function inlet.value(v)
  value = v
end
