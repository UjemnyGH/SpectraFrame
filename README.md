# SpectraFrame

### About
SpectraFrame is an graphics engine designed with [Vulkan API](https://www.vulkan.org) in mind.
That`s it, I wanted to make graphics engine based on Vulkan API to learn it.

### Libraries
- [Vulkan](https://www.vulkan.org) as core of entire engine rendering
- [GLFW](https://www.glfw.org) for handling window and inputs 
- [glm](https://github.com/g-truc/glm) cool math library, but possibly I make my own in future, for now it stays
- [stb](https://github.com/nothings/stb) for loading images from virtually any image file format

### About Vulkan API wrapper
As you seen there is nothing more for [Vulkan API](https://www.vulkan.org) than sole, clear  [Khronos Vulkan API](https://www.vulkan.org) library taken from [LunarG](https://vulkan.lunarg.com).
[Vulkan API](https://www.vulkan.org) wrapper is created by me with easier usability in mind, without much loss in [Vulkan](https://www.vulkan.org) customizability, but there is still a lot of to develop. Current state of wrapper is quite bare bone and handles only basic functionalities to create something on screen, yet still having more potential than older API like OpenGL.