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

# Vulkan
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/Bin")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/Bin32")
    endif()
else()
    find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/lib")
endif()

find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS "$ENV{VULKAN_SDK}/include")
include_directories(${VULKAN_INCLUDE_DIR})

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

# Copy assets
set(ASSETS
        assets/textures/fatboy.png
        assets/GL45/FragmentShader.glsl
        assets/GL45/VertexShader.glsl
        assets/Vulkan/FragmentShader.glsl
        assets/Vulkan/VertexShader.glsl
    )

add_custom_target(AssetTarget ALL)

function(CopyAsset inputfile outdir)
    set(outputfile ${outdir}/${inputfile})
    
    add_custom_command(
        TARGET AssetTarget
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Copying ${inputfile}"
    )
endfunction()

foreach(item ${ASSETS})
    CopyAsset(${item} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()

set(SRCS
        src/Base/ConstantBuffer.cpp
        src/Base/Material.cpp
        src/Base/Mesh.cpp
        src/Base/Renderer.cpp
        src/Base/RenderState.cpp
        src/Base/Sampler2D.cpp
        src/Base/Texture2D.cpp
        src/Base/Transform.cpp
        src/Base/VertexBuffer.cpp
        src/OpenGL/ConstantBufferGL.cpp
        src/OpenGL/MaterialGL.cpp
        src/OpenGL/MeshGL.cpp
        src/OpenGL/OpenGLRenderer.cpp
        src/OpenGL/RenderStateGL.cpp
        src/OpenGL/Sampler2DGL.cpp
        src/OpenGL/Texture2DGL.cpp
        src/OpenGL/TransformGL.cpp
        src/OpenGL/VertexBufferGL.cpp
        src/Vulkan/ConstantBufferVulkan.cpp
        src/Vulkan/MaterialVulkan.cpp
        src/Vulkan/MeshVulkan.cpp
        src/Vulkan/RenderStateVulkan.cpp
        src/Vulkan/Sampler2DVulkan.cpp
        src/Vulkan/Texture2DVulkan.cpp
        src/Vulkan/VertexBufferVulkan.cpp
        src/Vulkan/VulkanRenderer.cpp
        src/main.cpp
        src/Technique.cpp
    )

set(HEADERS
        src/Base/ConstantBuffer.hpp
        src/Base/Material.hpp
        src/Base/Mesh.hpp
        src/Base/Renderer.hpp
        src/Base/RenderState.hpp
        src/Base/Sampler2D.hpp
        src/Base/Texture2D.hpp
        src/Base/Transform.hpp
        src/Base/VertexBuffer.hpp
        src/OpenGL/ConstantBufferGL.hpp
        src/OpenGL/MaterialGL.hpp
        src/OpenGL/MeshGL.hpp
        src/OpenGL/OpenGLRenderer.hpp
        src/OpenGL/RenderStateGL.hpp
        src/OpenGL/Sampler2DGL.hpp
        src/OpenGL/Texture2DGL.hpp
        src/OpenGL/TransformGL.hpp
        src/OpenGL/VertexBufferGL.hpp
        src/Vulkan/ConstantBufferVulkan.hpp
        src/Vulkan/MaterialVulkan.hpp
        src/Vulkan/MeshVulkan.hpp
        src/Vulkan/RenderStateVulkan.hpp
        src/Vulkan/Sampler2DVulkan.hpp
        src/Vulkan/Texture2DVulkan.hpp
        src/Vulkan/VertexBufferVulkan.hpp
        src/Vulkan/VulkanRenderer.hpp
        src/IA.h
        src/Technique.hpp
    )

create_directory_groups(${SRCS} ${HEADERS})

add_executable(Testbench ${SRCS} ${HEADERS})
target_link_libraries(Testbench glew SDL2main SDL2-static ${Vulkan_LIBRARY})
set_property(TARGET Testbench PROPERTY CXX_STANDARD 11)
set_property(TARGET Testbench PROPERTY CXX_STANDARD_REQUIRED ON)
