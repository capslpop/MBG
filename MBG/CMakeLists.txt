# CMakeList.txt : CMake project for MBG, include source and define
# project specific logic here.
#

find_package(Catch2 CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (MBG "Tests/tests.cpp" "MBG/OpenGL/Window.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MBG PROPERTY CXX_STANDARD 20)
endif()

# If we are in debug mode declate that
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(_DEBUG)
endif()

find_package(imgui REQUIRED)
target_link_libraries(MBG PRIVATE imgui::imgui)

find_package (glfw3 REQUIRED)
target_link_libraries (MBG PRIVATE glfw)

find_package (glm REQUIRED)
target_link_libraries (MBG PRIVATE glm::glm)

find_package(glad REQUIRED)
target_link_libraries(MBG PRIVATE glad::glad)

find_package(stb REQUIRED)
target_include_directories(MBG PRIVATE ${Stb_INCLUDE_DIR})


# Link Catch2
target_link_libraries(MBG PRIVATE Catch2::Catch2WithMain)

# Enable CTest (so you can use `ctest`)
include(CTest)
include(Catch)
catch_discover_tests(MBG)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(MBG copy_assets)

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_shaders.cmake
)
add_dependencies(MBG copy_shaders)
