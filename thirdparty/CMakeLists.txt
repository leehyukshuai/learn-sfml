set(BUILD_SHARED_LIBS ON)
set(IMGUI_SFML_BUILD_EXAMPLES ON)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_subdirectory(imgui-sfml)
