cmake_minimum_required(VERSION 3.1)
project(cglft)

option(CGLFT_BUILD_STATIC "Build static library instead of shared." OFF)

set(CMAKE_CXX_STANDARD 11)


set(TARGET cglft)
set(CGLFT_VERSION 1.0)
set(INCLUDE_INSTALL_DIR include/cglft)
set(LIB_INSTALL_DIR lib)

set(INCLUDE_FILES
		font_face.h
		glyph.h
		font_shader.h
		font_renderer.h)

set(SOURCE_FILES
		${INCLUDE_FILES}
		font_face.cpp
		font_shader.cpp
		font_renderer.cpp)


if(CGLFT_BUILD_STATIC)
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE SHARED)
endif()


add_library(${TARGET} ${LIB_TYPE} ${SOURCE_FILES})


include(GenerateExportHeader)
generate_export_header(${TARGET})
include_directories(${PROJECT_BINARY_DIR})


find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${TARGET} ${FREETYPE_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${TARGET} ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(${TARGET} ${GLEW_LIBRARY})


install(TARGETS ${TARGET} EXPORT cglft-target
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION ${LIB_INSTALL_DIR}
		INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR})
install(EXPORT cglft-target DESTINATION ${LIB_INSTALL_DIR}/cmake/cglft)

install(FILES ${INCLUDE_FILES} DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES ${PROJECT_BINARY_DIR}/${TARGET}_export.h DESTINATION ${INCLUDE_INSTALL_DIR})


# Config
set(CGLFT_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR}")
set(CGLFT_LIBRARY_DIRS "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
set(CGLFT_LIBRARIES ${TARGET})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cglft-config.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/cglft-config.cmake"
		@ONLY ESCAPE_QUOTES)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cglft-config.cmake"
		DESTINATION ${LIB_INSTALL_DIR}/cmake/cglft)
