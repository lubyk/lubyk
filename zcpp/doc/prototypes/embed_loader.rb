class Loader
  def secure_load(filename)
    begin
      load filename
    rescue => err
      puts err.inspect
    end
  end
end