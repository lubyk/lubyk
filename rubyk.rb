require 'lib/rubyk/rubyk_server'
app_path = File.join(File.dirname(__FILE__), 'app') + '/'

$debug = false

opts = {
  :realm => 'Rubyk',
  :authenticator => nil, # Hash['gaspard', 'secret']
}

RubykServer.new(app_path, '127.0.0.1', 9090, opts).run