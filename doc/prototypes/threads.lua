threads = {}

function browse(name)
  table.insert(threads, coroutine.create(function()
    -- this is the code executed in the
    -- callback
    print(name, 'new service Foo')
    coroutine.yield()
    print(name, 'new service Bar')
    if name ~= 'a' then
      coroutine.yield()
      print(name, 'new service Baz')
    end
  end))
end

function main()
  local threads = threads

  while #threads > 0 do
    local dead_threads = {}
    for i = 1, #threads do
      local ok = coroutine.resume(threads[i])
      if not ok then
        table.insert(dead_threads, i)
      end
    end

    -- remove dead threads
    for i = 1, #dead_threads do
      table.remove(threads, dead_threads[i] - i + 1)
    end
  end
end

browse('a')
browse('b')

main()