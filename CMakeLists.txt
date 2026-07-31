cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(sandbox)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES 
	main.cpp
	LoadShaders.h
	LoadShaders.cpp
	cloud.vert
	cloud.frag)
	
add_executable(${PROJECT_NAME} ${SOURCES})

# adding packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)

find_path(LINMATH_INCLUDE_DIRS "linmath.h/linmath.h")

target_include_directories(${PROJECT_NAME} PRIVATE 
	${LINMATH_INCLUDE_DIRS} ${Stb_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE 
	glfw glad::glad)