cmake_minimum_required(VERSION 3.16)
project(MazeMVVM LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Quick Qml QuickControls2)

add_executable(MazeMVVM
    main.cpp
    qml.qrc
    models/cell.cpp
    models/cell.hpp
    models/mazeModel.cpp
    models/mazeModel.hpp
    viewmodels/mazeViewModel.cpp
    viewmodels/mazeViewModel.hpp
)
target_link_libraries(MazeMVVM
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2
)
target_include_directories(MazeMVVM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels
)
set_target_properties(MazeMVVM PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
