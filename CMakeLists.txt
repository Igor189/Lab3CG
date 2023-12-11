cmake_minimum_required (VERSION 3.8) 

# Enable Hot Reload for MSVC compilers if supported. 
if (POLICY CMP0141) 
  cmake_policy(SET CMP0141 NEW) 
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>") 
endif() 

project(Lab2
  VERSION "1.0.0"
  LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DUNICODE -D_UNICODE)

if (UNIX)
  add_definitions(-DNIX -D_UNIX_OS)
else()
  add_definitions(-DWIN -D_WINDOWS_OS)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (MSVC)
  add_compile_options(/utf-8)
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(Lab2 
  src/main.cpp
  src/FuncWidget.cpp
  src/FuncWidget.h
  src/LogicalOperations.h
  src/LogicalOperations.cpp
  src/MainWindow.cpp
  src/MainWindow.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
) 