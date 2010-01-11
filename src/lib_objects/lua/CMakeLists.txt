# This makefile just copies "*.lua" files from lib_objects/lua into "lib/lua" (compilation result directory)

file (GLOB LUA_FILES ${RUBYK_SOURCE_DIR}/src/lib_objects/lua/*.lua)
set  (LUA_LIB_DIR ${RUBYK_SOURCE_DIR}/lib/lua)

foreach(LUA_FILE ${LUA_FILES})
	get_filename_component (LUA_FILE_NAME ${LUA_FILE} NAME)
	configure_file (${LUA_FILE} ${LUA_LIB_DIR}/${LUA_FILE_NAME} COPYONLY)
endforeach(LUA_FILE)
