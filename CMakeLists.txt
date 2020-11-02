cmake_minimum_required(VERSION 3.10)
project(TelloDriver VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 14)

set(TELLO_TELEMETRY_SOURES
  ${CMAKE_SOURCE_DIR}/src/packet_parser/protocol.cpp
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry/flight_data/FlightData.cpp
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry/log_data/LogData.cpp
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry/log_data/LogNewMvoFeedback.cpp
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry/log_data/LogNewImuAttiFeedback.cpp
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry/TelloTelemetry.cpp
)

include_directories(
  include
)

add_library(TelloDriver src/TelloDriver.cpp)

target_include_directories(TelloDriver PRIVATE
  ${CMAKE_SOURCE_DIR}/src/packet_parser
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry
)

## Download and unpack spdlog at configure time
configure_file(CMakeLists.txt.in ${CMAKE_SOURCE_DIR}/lib/spdlogdownload/CMakeLists.txt @ONLY) 
add_subdirectory(lib/spdlogdownload)

# Wait for spdlog to download and compile.
add_dependencies(TelloDriver spdlog_download)


target_include_directories(TelloDriver PUBLIC ${CMAKE_BINARY_DIR}/external/include)
# target_link_libraries(TelloDriver PUBLIC ${CMAKE_BINARY_DIR}/external/lib)

add_subdirectory(examples)
add_subdirectory(test)
add_subdirectory(lib/googletest)

enable_testing()