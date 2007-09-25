def runner
  sleep 0.1
  now = Time.now
  puts now.strftime('%H:%M:%S') + sprintf('.%i', 10 * (now.to_f - now.to_i))
end