cmake_minimum_required(VERSION 3.1.0)

project(Testbench)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -gdwarf-2 -O3 -static-libstdc++ -Wall")
endif()

# GLEW
set(BUILD_UTILS OFF CACHE BOOL "Don't build GLEW utility programs" FORCE)
add_subdirectory(externals/glew/build/cmake)
include_directories(externals/glew/include)

# SDL
set(SDL_SHARED OFF CACHE BOOL "Don't build shared SDL")
set(VIDEO_OPENGLES OFF CACHE BOOL "Don't include OpenGL ES support")
add_subdirectory(externals/SDL)
include_directories(externals/SDL/include)

# GLM
include_directories(externals/glm)

# STB
include_directories(externals/stb)

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

set(SRCS
        src/main.cpp
    )

set(HEADERS
        
    )

create_directory_groups(${SRCS} ${HEADERS})

add_executable(Testbench ${SRCS} ${HEADERS})
target_link_libraries(Testbench glew ${SDL2_LIBRARIES})
set_property(TARGET Testbench PROPERTY CXX_STANDARD 11)
set_property(TARGET Testbench PROPERTY CXX_STANDARD_REQUIRED ON)
