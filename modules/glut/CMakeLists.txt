include_directories(src)
find_package(GLUT REQUIRED)

file(GLOB MODULE_SOURCES ${MODULE}/src/*.cpp ${MODULE}/src/${PLAT}/*.cpp ${MODULE}/src/*.c ${MODULE}/src/${PLAT}/*.c)

build_module("glut" ${MODULE_SOURCES})

target_link_libraries("glut" ${GLUT_LIBRARIES})