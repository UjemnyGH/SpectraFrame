file(GLOB_RECURSE FRAMEGRAPH_SOURCE_MOD
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(FramegraphMod STATIC ${FRAMEGRAPH_SOURCE_MOD})

target_include_directories(FramegraphMod PUBLIC ${PROJECT_SOURCE_DIR}/vendor/include)
target_link_libraries(FramegraphMod PUBLIC ${Vulkan_LIBRARIES} STB_LIBRARIES CoreMod MathMod)
