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
find_library(Vulkan_LIBRARY vulkan-1)
find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
        "$ENV{VULKAN_SDK}/include")
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
        src/Camera.cpp
        src/ConstantBuffer.cpp
        src/ConstantBufferGL.cpp
        src/DepthStencil.cpp
        src/DrawInfo.cpp
        src/main.cpp
        src/Material.cpp
        src/MaterialGL.cpp
        src/Mesh.cpp
        src/MeshGL.cpp
        src/MeshPart.cpp
        src/OpenGLRenderer.cpp
        src/Pass.cpp
        src/Renderer.cpp
        src/RenderState.cpp
        src/RenderStateGL.cpp
        src/RenderTarget.cpp
        src/ResourceBinding.cpp
        src/ResourceBindingGL.cpp
        src/Sampler2D.cpp
        src/Sampler2DGL.cpp
        src/Scene.cpp
        src/Technique.cpp
        src/Texture2D.cpp
        src/Texture2DGL.cpp
        src/Transform.cpp
        src/TransformGL.cpp
        src/VertexBuffer.cpp
        src/VertexBufferGL.cpp
    )

set(HEADERS
        src/Camera.h
        src/ConstantBuffer.h
        src/ConstantBufferGL.h
        src/DepthStencil.h
        src/DrawInfo.h
        src/IA.h
        src/Material.h
        src/MaterialGL.h
        src/Mesh.h
        src/MeshGL.h
        src/MeshPart.h
        src/OpenGLRenderer.h
        src/Pass.h
        src/Renderer.h
        src/RenderState.h
        src/RenderStateGL.h
        src/RenderTarget.h
        src/ResourceBinding.h
        src/ResourceBindingGL.h
        src/Sampler2D.h
        src/Sampler2DGL.h
        src/Scene.h
        src/Technique.h
        src/Texture2D.h
        src/Texture2DGL.h
        src/Transform.h
        src/TransformGL.h
        src/VertexBuffer.h
        src/VertexBufferGL.h
    )

create_directory_groups(${SRCS} ${HEADERS})

add_executable(Testbench ${SRCS} ${HEADERS})
target_link_libraries(Testbench glew SDL2main SDL2-static ${Vulkan_LIBRARIES})
set_property(TARGET Testbench PROPERTY CXX_STANDARD 11)
set_property(TARGET Testbench PROPERTY CXX_STANDARD_REQUIRED ON)
