require 'rubygems'
require 'mkrf'

Mkrf::Generator.new('RtMidi', ["*.cpp"]) do |g|
  g.ldshared << ' -L/usr/lib -lgcc -lstdc++'
  case RUBY_PLATFORM
  when /darwin/
    g.add_define('__MACOSX_CORE__')
    g.ldshared << ' -framework CoreMIDI' << ' -framework CoreFoundation' << ' -framework CoreAudio'
  else
    # FIXME
    # g.include_library( 'GL', 'glVertex3d')
  end
end