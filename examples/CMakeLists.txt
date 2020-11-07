include_directories(include)
add_executable(simple_connection
  simple_connection.cpp
)
message(STATUS "Example is linknig to: ${CMAKE_PROJECT_NAME}")
add_dependencies(simple_connection ${CMAKE_PROJECT_NAME})
target_link_libraries(simple_connection PUBLIC  ${CMAKE_PROJECT_NAME})
