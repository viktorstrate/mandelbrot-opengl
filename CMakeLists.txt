cmake_minimum_required(VERSION 3.10)

project(mandlebrot)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.2 REQUIRED)

set(VENDOR_SRCS vendor/src/glad.c)

set(
    SOURCES
        src/main.cpp
        src/setup.cpp
        src/shaders.cpp
)

#set(
#    HEADERS
#        src/Game.h
#        src/graphics/Shader.h
#        src/Mesh.h
#        src/Model.h
#        src/graphics/Rendering.h
#        src/graphics/Texture.h
#        src/graphics/VertexArray.h
#        src/graphics/VertexBuffer.h
#        src/GameObject.h
#        src/Component.h
#        src/components/TransformComponent.h
#        src/components/CameraComponent.h
#)

include_directories(src/)
include_directories(vendor/includes)

# target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${VENDOR_SRCS})

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL ${CMAKE_DL_LIBS})

