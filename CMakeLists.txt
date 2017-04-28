cmake_minimum_required (VERSION 2.6)



project (DracoBufferEncoder)

set (DracoBufferEncoder_VERSION_MAJOR 0)
set (DracoBufferEncoder_VERSION_MINOR 1)
set (DRACO_SOURCE_DIR ${PROJECT_SOURCE_DIR}/submodules/draco)


# include AWD cpp library
add_subdirectory( ${DRACO_SOURCE_DIR} )



# configure a header file to pass some of the CMake settings
# to the source code

configure_file (
  "${PROJECT_SOURCE_DIR}/include/ExporterConfig.h.in"
  "${PROJECT_BINARY_DIR}/include/ExporterConfig.h"
)


#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DARWIN_SOURCE")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DARWIN_SOURCE")


set( ENCODER_SOURCES
  src/main.cxx
  src/kaitaistream.cpp
  src/awdbuf.cpp
)


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include")


include_directories("${PROJECT_SOURCE_DIR}/submodules/tclap/include")

include_directories( "${DRACO_SOURCE_DIR}"  )
set( LINK_LIBRARIES draco_compression_point_cloud_encoder )


add_executable(encoder 
  ${ENCODER_SOURCES}
)

target_link_libraries( encoder PUBLIC draco )


#
# Tests
#

enable_testing()
add_test( test encoder )