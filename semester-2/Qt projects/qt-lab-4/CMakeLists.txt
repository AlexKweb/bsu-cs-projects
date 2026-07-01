cmake_minimum_required(VERSION 3.16)

project(qt-lab-4 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(qt-lab-4
  problem1.cpp
  problem2.cpp
  problem3.cpp
  problem4.cpp
  problem5.cpp
  for_task_1.txt
  for_task_2.txt
  for_task_3.txt
  for_task_4.txt
  for_task_5.txt


)
target_link_libraries(qt-lab-4 PRIVATE Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS qt-lab-4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
