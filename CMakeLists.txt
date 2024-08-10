# This will set the minimum version of CMake required to generate the projects
# CMakeLists has its own language and features and as more and more versions are 
# added it will include new keywords and options
cmake_minimum_required(VERSION 3.18) 

#navigate to ./build
#cmake -G"Visual Studio 15 2017 Win64" ..
#cmake --build .

# This will give our project a name
project(glacier)

# This instructs CMake that we want to generate an executable file from this project
# Other options would be add_library but this is a topic for another time
# You can notice that we also pass on the path to our cpp file
add_executable(glacier)

target_sources(glacier  PRIVATE src/main.cpp
                        src/camera.cpp
                        src/element.cpp
                        src/glad.c
                        src/ray_trace.cpp
                        src/shader_stage.cpp
                        src/shader.cpp
                        src/terrain.cpp
                        src/texture.cpp
                        src/triangles.cpp
                        src/imgui/imgui_demo.cpp
                        src/imgui/imgui_draw.cpp
                        src/imgui/imgui_impl_glfw.cpp
                        src/imgui/imgui_impl_opengl3.cpp
                        src/imgui/imgui_tables.cpp
                        src/imgui/imgui_widgets.cpp
                        src/imgui/imgui.cpp
                        )

message(STATUS ${PROJECT_SOURCE_DIR})

set(MY_RESOURCE_FILES 
                        src/vertex.glsl
                        src/fragment.glsl
                        src/ray_trace_viewer.glsl
                        src/scattering_media.glsl
                        src/simple_fs_shader.glsl
                        src/terrain.glsl
                        src/test_fullscreen_shader.glsl

                        src/N34W119.hgt
                        src/N32W111.hgt
                    )

#file(GLOB_RECURSE
#                        MY_RESOURCE_FILES
#                        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
#                        CONFIGURE_DEPENDS
#
#                        ${CMAKE_CURRENT_SOURCE_DIR}/resources/**.ttf    # adapt these to your needs
#                        ${CMAKE_CURRENT_SOURCE_DIR}/resources/**.svg
#                    )

#FOREACH(MY_RESOURCE_FILE ${MY_RESOURCE_FILES})
#    add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy 
#            ${PROJECT_SOURCE_DIR}/${MY_RESOURCE_FILE}
#            $<TARGET_FILE_DIR:${PROJECT_NAME}>
#        DEPENDS ${PROJECT_SOURCE_DIR}/${MY_RESOURCE_FILE}
#    )
#ENDFOREACH()

FOREACH(MY_RESOURCE_FILE ${MY_RESOURCE_FILES})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MY_RESOURCE_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/${MY_RESOURCE_FILE}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MY_RESOURCE_FILE}
  )
ENDFOREACH()

list(TRANSFORM MY_RESOURCE_FILES PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)

target_sources(glacier PRIVATE ${MY_RESOURCE_FILES})

target_include_directories(glacier
    PRIVATE
        # where the library itself will look for its internal headers
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/imgui
        ${PROJECT_SOURCE_DIR}/../include
)

target_link_libraries(glacier
    PRIVATE
        #libs
        ${PROJECT_SOURCE_DIR}/../lib/glfw3.lib
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)