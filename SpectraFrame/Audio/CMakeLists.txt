file(GLOB_RECURSE AUDIO_SOURCE_MOD
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(AudioMod STATIC ${AUDIO_SOURCE_MOD})

target_include_directories(AudioMod PUBLIC ${OpenAL_INCLUDE_DIRS})
target_link_libraries(AudioMod PUBLIC ${OpenAL_LIBRARIES} CoreMod)