cmake_minimum_required(VERSION 2.8)

project(yasg)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

add_executable(yasg src/Playground.cpp src/Engine.cpp main.cpp)

target_link_libraries(yasg SDL)
target_link_libraries(yasg SDL_image)
target_link_libraries(yasg SDL_ttf)
target_link_libraries(yasg GL)

install(TARGETS yasg RUNTIME DESTINATION bin)

add_subdirectory(src)