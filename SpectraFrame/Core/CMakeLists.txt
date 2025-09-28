file(GLOB_RECURSE CORE_SOURCE_MOD
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(CoreMod STATIC ${CORE_SOURCE_MOD})

target_include_directories(CoreMod PUBLIC ${Vulkan_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/vendor/include)
target_link_libraries(CoreMod PUBLIC ${Vulkan_LIBRARIES} ${GLFW_LIBRARIES} STB_LIBRARIES AudioMod)
