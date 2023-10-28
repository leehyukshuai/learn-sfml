add_library(engine WindowContext.cpp)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(app Application.cpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})