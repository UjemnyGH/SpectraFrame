file(GLOB_RECURSE MATH_SOURCE_MOD
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(MathMod STATIC ${MATH_SOURCE_MOD})

target_include_directories(MathMod PUBLIC ${PROJECT_SOURCE_DIR}/vendor/include)
target_link_libraries(MathMod PUBLIC CoreMod)
