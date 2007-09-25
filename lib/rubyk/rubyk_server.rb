require File.join(File.dirname(__FILE__), '../simple/dav_server')
require File.join(File.dirname(__FILE__), 'worker')


class RubykServer < DavServer
  
  def initialize(basepath, *args)
    super
    @running = :run
    @files_to_load  = []
    @server_binding = nil
    
    
    Thread.new do
      @server_binding = binding
      while(@running)
        if @running == :run
          begin
            runner
          rescue => err
            puts err.inspect
            @running = :runtime_error
          end
        end
      end
    end
    
    load_all(basepath)
  end
  
  def update(fullpath, content=nil)
    path = fullpath.split('/')
    filename  = path.last
    directory = path[-2]
    case directory
    when 'classes'
      load_class(filename, content || File.read(fullpath))
    when 'instances'
      load_instance(filename, content || File.read(fullpath))
    when 'links'
      load_links(filename, content || File.read(fullpath))
    end
    @running = :run if @running == :runtime_error
    return true
  rescue
    return false
  rescue SyntaxError
    # compilation error
    return false
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
      if filename =~ /\Aself(\.rb|)\Z/
        instance = self
      elsif filename =~ /\A(@\w+)\s*-\s*(\w+)(\.rb|)\Z/
        instance_name, klass_name = $1, $2
        instance = self if instance_name == 'self'
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
      else
        # bad filename
        return
      end  
      instance.instance_eval(content)
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
    if name =~ /.rb\Z/
      if @server.update(@fullpath, request.body)
        super
      else  
        response.set_status(500, 'Compilation Error')
      end
    else
      super
    end
  end
  
end
