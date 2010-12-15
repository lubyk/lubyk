
if(RUBYK_BUILD_DOXYGEN_DOCS AND DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc)
  set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)

  file (GLOB MODULES ${RUBYK_SOURCE_DIR}/modules/*)
  foreach(MODULE ${MODULES})
    file(GLOB MODULE_DUB_HEADERS ${MODULE}/include/dub/*.h)
    list(APPEND DOXYGEN_INPUT_LIST ${MODULE_DUB_HEADERS})
  endforeach(MODULE ${MODULES})

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)

  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${DOXYGEN_INPUT} ${CMAKE_BINARY_DIR}/Doxyfile
    )
  add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUTPUT})
endif()

