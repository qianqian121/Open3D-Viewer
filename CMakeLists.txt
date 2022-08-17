cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(open3d_viewer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/local/lib/cmake/Open3D")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
#message(STATUS ${CMAKE_MODULE_PATH})

#list(APPEND CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/lib/cmake")
set(CMAKE_INSTALL_PREFIX "/usr/local/lib/cmake/Open3D")
#find_package(Open3D REQUIRED)
find_package(Open3D CONFIG REQUIRED)
message("Open3D_DIR: ${Open3D_DIR}")
#find_package(Open3D CONFIG REQUIRED HINTS ${CMAKE_INSTALL_PREFIX}/lib/cmake/Open3D)
message(STATUS ${CMAKE_INSTALL_PREFIX})
set(open3d_install_prefix "/usr/local")
set(Open3D_DIR "/usr/local/lib/cmake/Open3D")
#message(STATUS ${open3d_install_prefix})
#message(STATUS ${open3d_install_prefix}/lib/cmake/Open3D)
set(CMAKE_PREFIX_PATH ${open3d_install_prefix}/lib/cmake/)
#message(STATUS ${CMAKE_PREFIX_PATH})
message(STATUS ${CMAKE_INSTALL_PREFIX})
#find_package(Open3D HINTS ${open3d_install_prefix}/lib/cmake/Open3D)
#find_package(Open3D HINTS ${CMAKE_INSTALL_PREFIX}/lib/CMake CONFIG)
#find_package(Open3D CONFIG HINTS ${open3d_install_prefix})
#find_package(Open3D HINTS ${CMAKE_PREFIX_PATH}/lib/CMake)
if (Open3D_FOUND)
    message(STATUS "Found Open3D at ${Open3D_LIBRARY_DIRS}")
    set(Open3D_FOUND ${Open3D_FOUND})
    set(Open3D_INCLUDE_DIRS ${Open3D_INCLUDE_DIRS}/open3d)
#    set(Open3D_INCLUDE_DIRS ${Open3D_INCLUDE_DIRS})
    set(Open3D_LIBRARY_DIRS ${Open3D_LIBRARY_DIRS})
    set(Open3D_LIBRARIES ${Open3D_LIBRARIES})
    set(Open3D_C_FLAGS ${Open3D_C_FLAGS})
    set(Open3D_CXX_FLAGS ${Open3D_CXX_FLAGS})
    set(Open3D_EXE_LINKER_FLAGS ${Open3D_EXE_LINKER_FLAGS})
else ()
    message(FATAL_ERROR "Open3D build was not successful")
endif ()
message(Open3D_INCLUDE_DIRS ${Open3D_INCLUDE_DIRS})
message(Open3D_LIBRARY_DIRS ${Open3D_LIBRARY_DIRS})
message(Open3D_LIBRARIES ${Open3D_LIBRARIES})
message(Open3D_CXX_FLAGS ${Open3D_CXX_FLAGS})
message(STATUS "Open3D installed to: ${Open3D_LIBRARY_DIRS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${Open3D_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Open3D_CXX_FLAGS} -O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${Open3D_EXE_LINKER_FLAGS}")
include_directories(${Open3D_INCLUDE_DIRS})

link_directories(${Open3D_LIBRARY_DIRS})
add_executable(open3d-viewer
        main.cc
        auto_visualize.cc
        )
target_include_directories(open3d-viewer PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(open3d-viewer ${Open3D_LIBRARIES} dl)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

# link_directories must be before add_executable
link_directories(${Open3D_LIBRARY_DIRS})

add_executable(TestVisualizer TestVisualizer.cpp)

target_link_libraries(TestVisualizer ${Open3D_LIBRARIES})

target_include_directories(TestVisualizer PUBLIC ${Open3D_INCLUDE_DIRS})

# Hot fix windows dll not found issue, assumming we're using the Release build
option(BUILD_SHARED_LIBS "Whether Open3D was build as shared library" OFF)
if(WIN32 AND BUILD_SHARED_LIBS)
    message("Will copy Open3D.dll to ${CMAKE_CURRENT_BINARY_DIR}/Release")
    add_custom_command(TARGET TestVisualizer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_INSTALL_PREFIX}/bin/Open3D.dll
            ${CMAKE_CURRENT_BINARY_DIR}/Release)
endif()


#if (UNIX)
#    if (APPLE)
#        add_subdirectory("macos")
#    else()
#        add_subdirectory("debian")
#    endif (APPLE)
#endif (UNIX)
