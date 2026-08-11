cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(sandbox)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES 
	main.cpp
	LoadShaders.h
	LoadShaders.cpp
	triangle.vert
	triangle.frag)
	
add_executable(${PROJECT_NAME} ${SOURCES})

add_executable(texbake tools/texbake.cpp)

# adding packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(unofficial-libsquish CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad::glad)

target_include_directories(texbake PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(texbake PRIVATE unofficial::libsquish::squish)

add_custom_command(TARGET texbake POST_BUILD
    COMMAND $<TARGET_FILE:texbake> ../grad4k.jpg ../grad4k.tex)