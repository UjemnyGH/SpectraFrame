cmake_minimum_required(VERSION 3.16)
project(SpectraFrame VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenAL REQUIRED)
find_package(Lua REQUIRED)

add_subdirectory(vendor)
add_subdirectory(SpectraFrame)

message(STATUS "  Vulkan:       ${Vulkan_FOUND}")
message(STATUS "  GLFW:         ${GLFW_FOUND}")
message(STATUS "  OpenAL:       ${OpenAL_FOUND}")
message(STATUS "  Lua:          ${Lua_FOUND}")
message(STATUS "  Mono:         ${Mono_FOUND}, no mono implemented rn")

set(ENGINE_MODULES
  SceneMod
  MathMod
  CoreMod
  FramegraphMod
)

set(VENDOR_INCLUDE
  ${GLFW_INCLUDE_DIRS}
  ${Vulkan_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIRS}
  ${LUA_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/vendor/include
)

set(VENDOR_LIBS
  ${GLFW_LIBRARIES}
  ${Vulkan_LIBRARIES}
  ${OPENAL_LIBRARIES}
  ${LUA_LIBRARIES}
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/SpectraFrame/main.cpp)

target_link_libraries(${PROJECT_NAME}
  ${ENGINE_MODULES}
  ${VENDOR_LIBS}  
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/SpectraFrame/*
  ${VENDOR_INCLUDE}
)
