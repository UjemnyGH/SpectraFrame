file(GLOB_RECURSE RENDERING_SOURCE_MOD
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(RenderingMod STATIC ${RENDERING_SOURCE_MOD})

target_include_directories(RenderingMod PUBLIC ${PROJECT_SOURCE_DIR}/vendor/include)
target_link_libraries(RenderingMod PUBLIC ${Vulkan_LIBRARIES} STB_LIBRARIES CoreMod MathMod)
