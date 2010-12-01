
if(RUBYK_BUILD_DOXYGEN_DOCS AND DOXYGEN_FOUND)

set(DOXYGEN_LANGUAGE "English" CACHE STRING "Language used by doxygen")
mark_as_advanced(DOXYGEN_LANGUAGE)

set(CMAKE_DOXYGEN_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/pics")

set(module_names core imgproc calib3d features2d objdetect video highgui ml legacy contrib)
set(DOXYGEN_SOURCE_DIR)
set(doxy_html_dep_list)

foreach(m ${module_names})
    set(module_hdr_path "${CMAKE_SOURCE_DIR}/modules/${m}/include/opencv2/${m}")
    set(DOXYGEN_SOURCE_DIR ${DOXYGEN_SOURCE_DIR} ${module_hdr_path})
    file(GLOB module_hdrs "${module_hdr_path}/*.h*")
    set(doxy_html_dep_list ${doxy_html_dep_list} ${module_hdrs})
endforeach()

set(DOXYGEN_OUTPUT_DIR html)
string(REGEX REPLACE ";" " " CMAKE_DOXYGEN_INPUT_LIST "${DOXYGEN_SOURCE_DIR}")

configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
set(HTML_TARGET "DOXY_HTML" )

add_custom_target(${HTML_TARGET} ALL
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile ${doxy_html_dep_list})
exec_program(mkdir ARGS "-p \"${CMAKE_CURRENT_BINARY_DIR}/html/\"")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "${OPENCV_DOC_INSTALL_PATH}/html" COMPONENT main)

endif()

