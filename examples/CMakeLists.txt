include_directories(include)
message(STATUS "Compiling examples: ${CMAKE_PROJECT_NAME}")

add_executable(simple_connection
  simple_connection.cpp
)
add_dependencies(simple_connection ${CMAKE_PROJECT_NAME})
target_link_libraries(simple_connection PUBLIC  ${CMAKE_PROJECT_NAME})


add_executable(simple_takeoff
simple_takeoff.cpp
)
add_dependencies(simple_takeoff ${CMAKE_PROJECT_NAME})
target_link_libraries(simple_takeoff PUBLIC  ${CMAKE_PROJECT_NAME})