inlet(1, 'frequency', RealIO('Hz', 'Frequency of the damn thing.'))

function inlet(port, name, type){
  inlets_[port] = inlets_[port] or new_inlet(port, 'bang', type)
  return inlets_[port]
}

function RealIO(units, info) {
  return {0.0, units, info}
}

function bang(val)
  print(val)
end