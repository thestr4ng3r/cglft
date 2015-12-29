cmake_minimum_required(VERSION 2.8)
project(cglft)

option(BUILD_STATIC "Build static library instead of shared." OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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



if(BUILD_STATIC)
	add_library(${TARGET} STATIC ${SOURCE_FILES})
else()
	add_library(${TARGET} SHARED ${SOURCE_FILES})
endif()



find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${TARGET} ${FREETYPE_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${TARGET} ${OPENGL_LIBRARIES})



install(TARGETS ${TARGET}
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION ${LIB_INSTALL_DIR}
		INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR})

install(FILES ${INCLUDE_FILES} DESTINATION ${INCLUDE_INSTALL_DIR})




# Config
set(CGLFT_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR})
set(CGLFT_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR})
set(CGLFT_LIBRARIES ${TARGET})

configure_file(cglftConfig.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/cglftConfig.cmake"
		@ONLY ESCAPE_QUOTES)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cglftConfig.cmake"
		DESTINATION ${LIB_INSTALL_DIR}/cmake/cglft)
