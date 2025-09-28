#
# Most of the libraries are build while making buil;d files, here's the list:
# OpenAL-Soft - kcat/openal-soft.git; master branch
# Lua - lua/lua.git; v5.4.7 branch
# GLFW - glfw/glfw.git; 3.4 branch
# ImGui - ocornut/imgui.git; docking branch
#
# You need to have Vulkan preinstalled, you can download SDK from there https://vulkan.lunarg.com
#

cmake_minimum_required(VERSION 3.30)
project(SpectraFrame VERSION 1.0)

# I need features from >= C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Find Vulkan as a only required, preinstalled library
find_package(Vulkan REQUIRED)
# Use this instead if preinstalled, this would greatly increse build speed
#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
#find_package(OpenAL REQUIRED)
#find_package(Lua REQUIRED)

# Fetch OpenAL-Soft, Lua, GLFW and ImGui, Vulkan must be preinstalled
include(FetchContent)
FetchContent_Declare(
  OpenAL
  GIT_REPOSITORY https://github.com/kcat/openal-soft.git
  GIT_TAG master
)
FetchContent_MakeAvailable(OpenAL)

# I like using this old style of adding libraries in CMake, so this sets up everything for this
set(OpenAL_INCLUDE_DIRS "${OpenAL_SOURCE_DIR}/include")
set(OpenAL_LIBRARIES OpenAL)

FetchContent_Declare(
  Lua54
  GIT_REPOSITORY https://github.com/lua/lua.git
  GIT_TAG v5.4.7
)
FetchContent_MakeAvailable(Lua54)

# Make Lua into library
set(Lua_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/lua54-src")
add_library(Lua STATIC
    ${Lua_SOURCE_DIR}/lapi.c
    ${Lua_SOURCE_DIR}/lcode.c
    ${Lua_SOURCE_DIR}/ldebug.c
    ${Lua_SOURCE_DIR}/ldo.c
    ${Lua_SOURCE_DIR}/ldump.c
    ${Lua_SOURCE_DIR}/lfunc.c
    ${Lua_SOURCE_DIR}/linit.c
    ${Lua_SOURCE_DIR}/llex.c
    ${Lua_SOURCE_DIR}/lmem.c
    ${Lua_SOURCE_DIR}/lobject.c
    ${Lua_SOURCE_DIR}/lopcodes.c
    ${Lua_SOURCE_DIR}/lparser.c
    ${Lua_SOURCE_DIR}/lstate.c
    ${Lua_SOURCE_DIR}/lstring.c
    ${Lua_SOURCE_DIR}/ltable.c
    ${Lua_SOURCE_DIR}/ltm.c
    ${Lua_SOURCE_DIR}/lundump.c
    ${Lua_SOURCE_DIR}/lvm.c
    ${Lua_SOURCE_DIR}/lzio.c
)
target_include_directories(Lua PUBLIC ${Lua_SOURCE_DIR})

set(Lua_INCLUDE_DIRS "${Lua_SOURCE_DIR}")
set(Lua_LIBRARIES Lua)

FetchContent_Declare(
  GLFW
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(GLFW)

set(GLFW_INCLUDE_DIRS "${glfw_SOURCE_DIR}/include")
set(GLFW_LIBRARIES glfw)

# Add ImGui
FetchContent_Declare(
  ImGui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
)
FetchContent_MakeAvailable(ImGui)

# Make ImGui into library
set(ImGui_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/imgui-src")
add_library(ImGui STATIC 
  ${ImGui_SOURCE_DIR}/imgui_demo.cpp
  ${ImGui_SOURCE_DIR}/imgui_draw.cpp
  ${ImGui_SOURCE_DIR}/imgui_tables.cpp
  ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
  ${ImGui_SOURCE_DIR}/imgui.cpp
  # ImGui backend
  ${ImGui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${ImGui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

target_link_libraries(ImGui PUBLIC
  ${Vulkan_LIBRARIES}
  ${GLFW_LIBRARIES}
)

target_include_directories(ImGui PUBLIC 
  ${ImGui_SOURCE_DIR}
  ${ImGui_SOURCE_DIR}/backends

  ${Vulkan_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
)

set(ImGui_INCLUDE_DIRS "${ImGui_SOURCE_DIR} ${ImGui_SOURCE_DIR}/backends")
set(ImGui_LIBRARIES ImGui)

# Choose platform
if(UNIX AND NOT APPLE)
  set(SESSION_TYPE $ENV{XDG_SESSION_TYPE})

  pkg_check_modules(WAYLAND wayland-client)
  find_package(X11)
  if(WAYLAND_FOUND AND SESSION_TYPE STREQUAL "wayland")
    add_definitions(
      -DVK_USE_PLATFORM_WAYLAND_KHR
      -DGLFW_EXPOSE_NATIVE_WAYLAND
    )
  elseif(X11_FOUND AND SESSION_TYPE STREQUAL "x11")
    add_definitions(
      -DVK_USE_PLATFORM_XLIB_KHR
      -DGLFW_EXPOSE_NATIVE_X11
    )
  endif()

  message(STATUS "  Building for linux ${SESSION_TYPE}")
else()
  message(STATUS "  Building for windows")
  add_definitions(
    -DVK_USE_PLATFORM_WIN32_KHR
    -DGLFW_EXPOSE_NATIVE_WIN32
  )
endif()

# Add vendor subdirectory (STB libraries)
add_subdirectory(vendor)
# Add engine components subdirectory
add_subdirectory(SpectraFrame)

# Debug messages, works only for prebuild CMake libraries
message(STATUS "  Vulkan:       ${Vulkan_FOUND}")
#message(STATUS "  GLFW:         ${GLFW_FOUND}")
#message(STATUS "  OpenAL:       ${OpenAL_FOUND}")
#message(STATUS "  Lua:          ${Lua_FOUND}")
#message(STATUS "  Mono:         ${Mono_FOUND}, no mono implemented rn")

# Add used engine modules, entire engine is divided into modules, 1 module is 1 SpectraFrame subfolder
set(ENGINE_MODULES
  AudioMod
  SceneMod
  #MathMod
  CoreMod
  RenderingMod
)

# Debug messages
message(STATUS "  GLFW:     ${glfw_SOURCE_DIR}")
message(STATUS "  OpenAL:   ${OpenAL_SOURCE_DIR}")
message(STATUS "  Lua:      ${Lua_SOURCE_DIR}")
message(STATUS "  ImGui:    ${ImGui_SOURCE_DIR}")

# Include paths of vendor libraries
set(VENDOR_INCLUDE
  ${GLFW_INCLUDE_DIRS}
  ${Vulkan_INCLUDE_DIRS}
  ${OpenAL_INCLUDE_DIRS}
  ${Lua_INCLUDE_DIRS}
  ${ImGui_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/vendor/include
)

# Vendor library paths
set(VENDOR_LIBS
  ${GLFW_LIBRARIES}
  ${Vulkan_LIBRARIES}
  ${OpenAL_LIBRARIES}
  ${Lua_LIBRARIES}
  ${ImGui_LIBRARIES}
  ${STB_LIBRARIES}
)

# Link all libraries
target_link_libraries(${PROJECT_NAME}
  ${ENGINE_MODULES}
  ${VENDOR_LIBS}  
)

# Add all include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/SpectraFrame/*
  ${VENDOR_INCLUDE}
)

# Choose platform once again to be 100% sure
if(UNIX AND NOT APPLE)
  set(SESSION_TYPE $ENV{XDG_SESSION_TYPE})

  if(WAYLAND_FOUND AND SESSION_TYPE STREQUAL "wayland")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      VK_USE_PLATFORM_WAYLAND_KHR
      GLFW_EXPOSE_NATIVE_WAYLAND
    )
  elseif(X11_FOUND AND SESSION_TYPE STREQUAL "x11")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      VK_USE_PLATFORM_XLIB_KHR
      GLFW_EXPOSE_NATIVE_X11
    ) 
  endif()
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    VK_USE_PLATFORM_WIN32_KHR
    GLFW_EXPOSE_NATIVE_WIN32
  )
endif()

