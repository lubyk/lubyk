# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------
set(MODULE_NAME crypto)
FIND_PACKAGE(OpenSSL REQUIRED)

include_directories(vendor/src)
build_module(${MODULE_NAME} vendor/src/lcrypto.c)
include_directories(${MODULE_NAME} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(${MODULE_NAME} ${OPENSSL_LIBRARIES})

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------
