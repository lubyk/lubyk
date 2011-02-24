inlet('input', 'Passes input values to output.')
out = outlet('output', 'Sends input values [?].')

function inlet.input(val)
  out(val)
end