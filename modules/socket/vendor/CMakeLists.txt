# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------

include_directories(src)

set(target "${MODULE_NAME}_slua")
add_custom_target(${target})

set(TO_TOP_SHARE src/ltn12.lua src/mime.lua)
set(TO_SOCKET_SHARE src/socket.lua src/http.lua src/url.lua src/tp.lua src/ftp.lua src/smtp.lua)

copy_files(${target} "${TO_TOP_SHARE}"    lib)
copy_files(${target} "${TO_SOCKET_SHARE}" lib/socket)

add_dependencies(${MODULE_NAME} ${target})

set(SUB_MODULE_SOURCES src/luasocket.c src/timeout.c src/buffer.c src/io.c src/auxiliar.c
                       src/options.c src/inet.c src/tcp.c src/udp.c src/except.c
                       src/select.c src/usocket.c)
build_sub_module("core" ${SUB_MODULE_SOURCES})

set(MAIN_MODULE_NAME ${MODULE_NAME})
set(MODULE_NAME mime)
add_custom_target(${MODULE_NAME} true) # noop
set(SUB_MODULE_SOURCES src/mime.c)
build_sub_module("core" ${SUB_MODULE_SOURCES})

add_dependencies(${MAIN_MODULE_NAME} ${MODULE_NAME})

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------
