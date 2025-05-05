file(GLOB_RECURSE SCENE_SOURCE_MOD
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(SceneMod STATIC ${SCENE_SOURCE_MOD})

target_include_directories(SceneMod PUBLIC ${PROJECT_SOURCE_DIR}/vendor/include)
target_link_libraries(SceneMod PUBLIC ${Vulkan_LIBRARIES} STB_LIBRARIES ${LUA_LIBRARIES} ${OPENAL_LIBRARIES} CoreMod FramegraphMod MathMod)
