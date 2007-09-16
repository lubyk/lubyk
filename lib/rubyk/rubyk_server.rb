require File.join(File.dirname(__FILE__), '../simple/dav_server')


class RubykServer < DavServer
  
  def initialize(basepath, *args)
    super
    @running = true
    @files_to_load  = []
    @server_binding = nil
    
    
    Thread.new do
      @server_binding = binding
      while(@running)
        runner
      end
    end
    
    puts "Loading app..."
    load_all(basepath)
    puts "Loaded."
  end
  
  def update(fullpath)
    load fullpath
    #eval(string, @server_binding)
  end
  
  def runner
    # do nothing
  end
  
  def halt
    @running = false
  end
  
  def create_session(socket)
    RubykSession.new(self, socket, @basepath)
  end
  
  private
    def load_all(path)
      if File.directory?(path)
        Dir.entries(path).reject {|d| d =~ /\A\.|\.\.\Z/}.map do |e|
          if e =~ /.rb\Z/
            update(File.join(path,e))
          elsif File.directory?(e)
            load_all(File.join(path,e))
          else
            # ignore
          end
        end
      end
    end
end

class RubykSession < DavSession
  
  def initialize(server, socket, basepath)
    super
    @fullpath = File.join(@basepath, @request.path)
  end
  
  def do_put
    name = @fullpath.split('/').last
    return if name =~ /\A\./
    super
    if File.exist?(@fullpath) && name =~ /.rb\Z/
      @server.update(@fullpath)
    end
  end
  
end
