cmake_minimum_required(VERSION 3.14)

project(hikeyeQt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 "${CMAKE_BINARY_DIR}/../bin")

target_include_directories(${PROJECT_NAME} PRIVATE src src/graphics)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::OpenGL)
