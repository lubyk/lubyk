#set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -framework CoreMIDI -framework CoreAudio")
set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS}")
build_sub_module("vendor" RtMidi.cpp lubyk.cpp)
