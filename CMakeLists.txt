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

if(UNIX AND NOT APPLE)
  pkg_check_modules(WAYLAND wayland-client)
  find_package(X11)
  if(WAYLAND_FOUND)
    message(STATUS "  Building for linux wayland")
    add_definitions(
      -DVK_USE_PLATFORM_WAYLAND_KHR
      -DGLFW_EXPOSE_NATIVE_WAYLAND
    )
  elseif(X11_FOUND)
    message(STATUS "  Building for linux x11")
    add_definitions(
      -DVK_USE_PLATFORM_XLIB_KHR
      -DGLFW_EXPOSE_NATIVE_X11
    )
  endif()
else()
  message(STATUS "  Building for windows")
  add_definitions(
    -DVK_USE_PLATFORM_WIN32_KHR
    -DGLFW_EXPOSE_NATIVE_WIN32
  )
endif()

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
  RenderingMod
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

target_link_libraries(${PROJECT_NAME}
  ${ENGINE_MODULES}
  ${VENDOR_LIBS}  
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/SpectraFrame/*
  ${VENDOR_INCLUDE}
)

if(UNIX AND NOT APPLE)
  if(WAYLAND_FOUND)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      VK_USE_PLATFORM_WAYLAND_KHR
      GLFW_EXPOSE_NATIVE_WAYLAND
    )
  elseif(X11_FOUND)
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

