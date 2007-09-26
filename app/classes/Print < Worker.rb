
inlet :print => :string

def update!(time, conn)
  puts conn.value.to_s
end