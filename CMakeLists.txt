cmake_minimum_required(VERSION 3.5)
project(Babylon)
set(CMAKE_CXX_STANDARD 14) # C++14 beacuse it's 2017...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_MACOSX_RPATH 1)

find_package(Boost 1.61 REQUIRED) # Boost >= v1.61 is required, but not shipped in the repo due to its massive size

if(MSVC)
	add_compile_options(/MP)
endif()


# Add external libraries
add_subdirectory(external)
# Build Babylon
add_subdirectory(source)
# Build examples
add_subdirectory(examples)
