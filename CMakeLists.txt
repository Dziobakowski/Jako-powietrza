cmake_minimum_required(VERSION 3.10)
project(wxWeatherApp)

set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(wxWeatherApp main.cpp MainFrame.cpp PlotPanel.cpp Curl.cpp)

target_link_libraries(wxWeatherApp PRIVATE ${wxWidgets_LIBRARIES} curl jsoncpp)
