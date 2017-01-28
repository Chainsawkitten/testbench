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
        src/Base/ConstantBuffer.cpp
        src/Base/Material.cpp
        src/Base/Mesh.cpp
        src/Base/Renderer.cpp
        src/Base/RenderState.cpp
        src/Base/ResourceBinding.cpp
        src/Base/Sampler2D.cpp
        src/Base/Texture2D.cpp
        src/Base/Transform.cpp
        src/Base/VertexBuffer.cpp
        src/OpenGL/ConstantBufferGL.cpp
        src/OpenGL/MaterialGL.cpp
        src/OpenGL/MeshGL.cpp
        src/OpenGL/OpenGLRenderer.cpp
        src/OpenGL/RenderStateGL.cpp
        src/OpenGL/ResourceBindingGL.cpp
        src/OpenGL/Sampler2DGL.cpp
        src/OpenGL/Texture2DGL.cpp
        src/OpenGL/TransformGL.cpp
        src/OpenGL/VertexBufferGL.cpp
        src/Camera.cpp
        src/DepthStencil.cpp
        src/DrawInfo.cpp
        src/main.cpp
        src/MeshPart.cpp
        src/Pass.cpp
        src/RenderTarget.cpp
        src/Scene.cpp
        src/Technique.cpp
    )

set(HEADERS
        src/Base/ConstantBuffer.hpp
        src/Base/Material.hpp
        src/Base/Mesh.hpp
        src/Base//Renderer.hpp
        src/Base/RenderState.hpp
        src/Base/ResourceBinding.hpp
        src/Base/Sampler2D.hpp
        src/Base/Texture2D.hpp
        src/Base/Transform.hpp
        src/Base/VertexBuffer.hpp
        src/OpenGL/ConstantBufferGL.hpp
        src/OpenGL/MaterialGL.hpp
        src/OpenGL/MeshGL.hpp
        src/OpenGL/OpenGLRenderer.hpp
        src/OpenGL/RenderStateGL.hpp
        src/OpenGL/ResourceBindingGL.hpp
        src/OpenGL/Sampler2DGL.hpp
        src/OpenGL/Texture2DGL.hpp
        src/OpenGL/TransformGL.hpp
        src/OpenGL/VertexBufferGL.hpp
        src/Camera.hpp
        src/DepthStencil.hpp
        src/DrawInfo.hpp
        src/IA.h
        src/MeshPart.hpp
        src/Pass.hpp
        src/RenderTarget.hpp
        src/Scene.hpp
        src/Technique.hpp
    )

create_directory_groups(${SRCS} ${HEADERS})

add_executable(Testbench ${SRCS} ${HEADERS})
target_link_libraries(Testbench glew SDL2main SDL2-static)
set_property(TARGET Testbench PROPERTY CXX_STANDARD 11)
set_property(TARGET Testbench PROPERTY CXX_STANDARD_REQUIRED ON)
