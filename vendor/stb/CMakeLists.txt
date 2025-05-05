add_library(STB_LIBRARIES STATIC stb_implementation.cpp)
target_include_directories(STB_LIBRARIES PUBLIC ${PROJECT_SOURCE_DIR}/vendor/include)
