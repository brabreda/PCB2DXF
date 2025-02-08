cmake_minimum_required(VERSION 3.23)
project(PerfScope LANGUAGES CXX)
set(PROJECT_NAME PCB2DXF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# enable for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

# Specify the source files
file(GLOB_RECURSE SOURCES src/*.cpp)
# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(OpenCV 4.10 REQUIRED)

# If you need to link any libraries, you can specify them here
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
