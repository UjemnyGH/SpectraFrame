# modules
add_subdirectory(Audio)
add_subdirectory(Core)
add_subdirectory(Rendering)
#add_subdirectory(Math)
add_subdirectory(Scene)

add_executable(${PROJECT_NAME} 
  main.cpp
  testScene.cpp
)
