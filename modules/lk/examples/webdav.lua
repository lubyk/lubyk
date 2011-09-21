require 'lubyk'

srv = lk.DavServer(1024, '/Users/gaspard/git/lubyk/examples/patch/ab')
-- when caching children nodes, we count on the parent to cache entries
print(string.format("Starting server on port %i...\nConnect with: http://localhost:%i", srv.port, srv.port))

run()
