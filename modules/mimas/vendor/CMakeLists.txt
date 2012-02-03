project(glew)
set(target "glew")


set(GLEW_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/libglew.a")
add_custom_command(
  OUTPUT ${GLEW_OUTPUT}
  COMMAND make
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/glew
)
add_custom_target("glew_make" ALL DEPENDS ${GLEW_OUTPUT})

