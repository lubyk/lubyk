require 'fileutils'

class SimpleResource
  attr_accessor :path
  
  class << self
    # Return a mapper to an existing resource.
    def get(basepath, path, depth)
      if File.exist?(File.join(basepath, path))
        new(basepath, path, depth)
      else
        nil
      end
    end
    
    # Create a new resource and return a mapper.
    def get_or_new(basepath, path, depth)
      get(basepath, path, depth) || create_resource(basepath, path) && get(basepath, path, depth)
    end
    
    def create_resource(basepath, path)
      fullpath  = File.join(basepath, path)
      directory = File.dirname(fullpath)
      FileUtils.mkpath(directory) unless File.exist?(directory)
      return nil unless File.directory?(directory)
      FileUtils.touch(fullpath)
      return true
    end
  end
  
  def initialize(basepath, path, depth)
    @basepath = basepath
    @path     = path
    @fullpath = File.join(@basepath,@path)
    @depth  = depth.to_i
    @file   = File.new(@fullpath)
  end
  
  def name
    @path.split('/').last
  end
  
  def is_collection?
    @file && File.directory?(@fullpath)
  end
  
  def data
    @file ? @file.read : nil
  end
  
  def set_data(new_data)
    if @file
      File.open(@fullpath,'wb') {|f| f.syswrite(new_data) }
    else
      false
    end
  end
  
  def delete!
    FileUtils.rmtree(@fullpath)
  end
  
  def children
    @children ||= 
      if is_collection? && @depth > 0
        Dir.entries(@fullpath).reject {|d| d =~ /\A\.|\.\.\Z/}.map do |e|
          SimpleResource.get(@basepath, File.join(@path,e), @depth -1)
        end
      else
        []
      end
  end
  
  def all_children
    if @depth > 1
      children.map do |c|
        [c, c.all_children]
      end.flatten
    else
      children
    end
  end
  
  def each(&block)
    if @depth > 0 && is_collection?
      [self] + all_children
    else
      [self]
    end.each do |elem|
      yield(elem)
    end
  end
  
  def [](prop)
    return nil unless @file
    case prop
    when 'getlastmodified'
      @file.mtime
    when 'resourcetype'
      is_collection? ? '<D:collection/>' : nil
    when 'getetag'
      stat = File.stat(@file.path)
      stat.mtime.to_i.to_s + '-' + stat.ino.to_s
    when 'getcontentlength'
      File.stat(@file.path).size
      #is_collection? ? nil : @file.size
    else
      puts "  #{prop} not implemented" if $debug
      nil
    end
  end
end
