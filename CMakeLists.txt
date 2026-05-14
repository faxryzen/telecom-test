cmake_minimum_required(VERSION 3.10)
project(TelecomBot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src src/bots src/simulator src/terra)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "src/bots/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(output
  src/main.cpp
  ${LIB_SOURCES}
)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
  find_package(Boost CONFIG QUIET COMPONENTS unit_test_framework)
  if(Boost_FOUND)
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(tests
        ${TEST_SOURCES}
        ${LIB_SOURCES}
    )
    target_link_libraries(tests PRIVATE Boost::unit_test_framework)
    target_compile_definitions(tests PRIVATE BOOST_TEST_DYN_LINK)
    enable_testing()
    add_test(NAME boost_tests COMMAND tests)
  else()
    message(STATUS "Boost not found - tests disabled")
  endif()
endif()

file(GLOB DUNGEON_FILES "${CMAKE_SOURCE_DIR}/descriptions/*.txt")

add_custom_command(TARGET output POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DUNGEON_FILES}
    $<TARGET_FILE_DIR:output>
)