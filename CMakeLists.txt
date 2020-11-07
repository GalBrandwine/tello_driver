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

add_library(TelloDriver 
  src/TelloDriver.cpp
  ${TELLO_TELEMETRY_SOURES}
)

target_include_directories(TelloDriver PUBLIC
  ${CMAKE_SOURCE_DIR}/src/packet_parser
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry
)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/lib/spdlog/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

file(COPY  ${PROJECT_SOURCE_DIR}/lib/spdlog DESTINATION ${PROJECT_BINARY_DIR}/lib)
target_include_directories(TelloDriver PUBLIC ${PROJECT_BINARY_DIR}/lib/spdlog/include)

# add_custom_target( unZip ALL)
# add_custom_command(TARGET unZip PRE_BUILD
#    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/abc/
#    COMMAND ${CMAKE_COMMAND} -E tar xzf {CMAKE_SOURCE_DIR}/abc.zip
# WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# DEPENDS ${CMAKE_SOURCE_DIR}/abc.zip
# COMMENT "Unpacking abc.zip"
# VERBATIM)

add_custom_target( unZip ALL)
add_custom_command(
  TARGET unZip PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/lib/asio-1.18.0
  COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_SOURCE_DIR}/lib/asio-1.18.0.tar.bz2
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib/
  DEPENDS ${PROJECT_SOURCE_DIR}/lib/asio-1.18.0.tar.bz2
  COMMENT "Unpacking asio-1.18.0.tar.bz2"
  VERBATIM
)




# Force CMake to take libraries from build, and not from system.
set(NO_DEFAULT_PATH true)
target_include_directories(TelloDriver PUBLIC ${PROJECT_BINARY_DIR}/lib/asio-1.18.0/include)
# asio requires pthread
target_link_libraries(TelloDriver pthread)

add_subdirectory(examples)
add_subdirectory(test)
add_subdirectory(lib/googletest)

enable_testing()