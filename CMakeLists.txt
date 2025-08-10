#PRIMER: FileCalculator :)))), first real project

project(file_calculator)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(file_calculator)
target_link_libraries(file_calculator m)
target_sources(file_calculator PRIVATE src/main.c src/file_calculator.c src/file_handler.c src/operations.c src/option_handler.c)

target_include_directories(file_calculator PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(file_calculator PRIVATE
  $<$<CONFIG:Debug>:-g -O0>
)
