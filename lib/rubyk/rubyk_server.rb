require File.join(File.dirname(__FILE__), '../simple/dav_server')
require File.join(File.dirname(__FILE__), 'worker')


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
    
    load_all(basepath)
  end
  
  def update(fullpath)
    path = fullpath.split('/')
    filename  = path.last
    directory = path[-2]
    case directory
    when 'classes'
      load_class(filename, File.read(fullpath))
    when 'instances'
      load_instance(filename, File.read(fullpath))
    when 'links'
      load_links(filename, File.read(fullpath))
    end
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
    def load_all(base)
      [ ['classes', :load_class],
        ['instances', :load_instance],
        ['links', :load_links]].each do |group, action|
        path = File.join(base, group)
        if File.directory?(path)
          Dir.entries(path).reject {|d| d =~ /\A\.|\.\.\Z/}.map do |e|
            if e =~ /.rb\Z/
              self.send(action, e, File.read(File.join(path,e)))
            end
          end
        end
      end
    end
    
    def load_class(filename, content)
      if filename =~ /(\w+)(\s*<\s*(\w+)|)(\.rb|)\Z/
        klass_name, superklass = $1, $3
        unless self.class.const_defined?(klass_name)
          if superklass
            # create a class
            eval "class #{klass_name} < #{superklass}; end"
          else
            # create a module
            eval "module #{klass_name}; end"
          end
        end
        klass = self.class.const_get(klass_name)
        if klass.kind_of?(Class)
          klass.class_eval(content)
        elsif klass.kind_of?(Module)
          klass.module_eval(content)
        end
      end
    end
    
    def load_instance(filename, content)
      if filename =~ /(@\w+)\s*-\s*(\w+)(\.rb|)\Z/
        instance_name, klass_name = $1, $2
        unless instance = instance_variable_get(instance_name)
          if self.class.const_defined?(klass_name) || (Module.const_get(klass_name) rescue nil)
            eval "#{instance_name} = #{klass_name}.new"
            instance = instance_variable_get(instance_name)
          else
            puts "unknown class #{klass_name.inspect}"
            # error unknown class
            return
          end
        end
        instance.instance_eval(content)
      end
    end
    
    def load_links(filename, content)
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
