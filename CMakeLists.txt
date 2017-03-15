project(MrHash)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
 
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(hashlibrary)
 
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} hashlibrary)
