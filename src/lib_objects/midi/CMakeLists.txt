if(UNIX)
  if(APPLE)
		set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -framework CoreMIDI -framework CoreFoundation -framework CoreAudio")
  else(APPLE)

  endif(APPLE)
else(UNIX)
  if(WIN32)

  else(WIN32)

  endif(WIN32)
endif(UNIX)

add_rko_object (MidiOut MidiOut.cpp RtMidi.cpp)