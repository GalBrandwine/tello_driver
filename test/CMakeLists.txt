set(BINARY ${CMAKE_PROJECT_NAME}_test)


set(TEST_SOURCES
    TelloDriverTest.cpp
  ${CMAKE_SOURCE_DIR}/src/packet_parser/test/packet_test.cpp
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry/test/flight_data_test.cpp

  ${TELLO_TELEMETRY_SOURES}
)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

# Link againts our gtest submodule, not the one installed in '/usr/local'
# Gtest require pthread, so we need to link against this one also.
target_link_libraries (${BINARY} PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_BINARY_DIR}/lib/libgtestd.a pthread)

target_include_directories(${BINARY} PRIVATE
  ${CMAKE_SOURCE_DIR}/src/packet_parser
  ${CMAKE_SOURCE_DIR}/src/tello_telemetry
)