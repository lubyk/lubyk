require File.join(File.dirname(__FILE__), 'server')
require 'rexml/document'

require File.join(File.dirname(__FILE__), 'resource')

class DavServer < SimpleServer
  def initialize(basepath, *args)
    @basepath = basepath
    super(*args)
  end
  
  def create_session(socket)
    DavSession.new(self, socket, @basepath)
  end
end

class DavSession < SimpleSession
  def initialize(server, socket, basepath)
    super(server, socket)
    @basepath = basepath
  end
  
  def before_do
    return false unless super
    
    if ['PROPFIND', 'GET', 'DELETE'].include?(request_method) && !resource
      not_found
      return false
    end
    #return
  end
  
  def do_options
    response['Allow'] = 'GET, PROPFIND, PUT, DELETE'
    response.headers['DAV'] = "1,2"
    response.headers['MS-Author-Via'] = "DAV"
  end
  
  def do_lock
    res = resource(true)
    # create lock (lock_id, master). lock_id MUST be unique globally. 'If' header ?
    # propagate lock depending on depth (lock_id, slave)
    # add/remove element ==> add/remove (lock_id, slave)
    # remove element  ==> remove (lock_id, all)
  end
  
  def do_get
    return not_found unless response
    response.body = resource.data
  end
  
  def do_put
    resource(true).set_data(request.body)
  end
  
  def do_delete
    resource.delete!
  end
  
  def do_propfind
    res = response
    return not_found unless resource
    res["Content-Type"] = 'text/xml; charset="utf-8"'
    res.set_status(207, 'Multistatus')
    xml = REXML::Document.new(request.body)
    properties = xml.elements.to_a('D:propfind/D:prop/*').map {|p| p.name}
    res.out "<?xml version='1.0' encoding='utf-8'?>"
    res.out "<D:multistatus xmlns:D = 'DAV:'>"
    
    resource.each do |resource|
      res.out "<D:response>"
      res.out "  <D:href>#{resource.path}</D:href>"
      res.out "  <D:propstat>"
      res.out "    <D:status>HTTP/1.1 200 OK</D:status>"
      res.out "    <D:prop>"
      properties.each do |prop|
        if value = resource[prop]
          res.out "      <D:#{prop}>#{value}</D:#{prop}>"
        else
          res.out "      <D:#{prop}/>"
        end
      end
      res.out "    </D:prop>"
      res.out "  </D:propstat>"
      res.out "</D:response>"
    end
    res.out "</D:multistatus>"
  end
  
  private
    def not_found
      response.set_status(404, 'Not Found')
    end
    
    def resource(create_new = false)
      @resource ||= create_new ? 
                    SimpleResource.get_or_new(@basepath, @request.path, @request['Depth']) :
                    SimpleResource.get(@basepath, @request.path, @request['Depth'])
    end
end
