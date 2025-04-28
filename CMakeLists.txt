cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(sandbox)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES 
	main.cpp
	LoadShaders.h
	LoadShaders.cpp
	triangle.vert
	triangle.frag)
	
add_executable(${PROJECT_NAME} ${SOURCES})

# adding packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad::glad)