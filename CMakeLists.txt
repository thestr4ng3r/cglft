cmake_minimum_required(VERSION 2.8)
project(cglft)

option(BUILD_STATIC "Build static library instead of shared." OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(TARGET cglft)

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
		LIBRARY DESTINATION lib
		INCLUDES DESTINATION include/cglft)

install(FILES ${INCLUDE_FILES} DESTINATION include/cglft)