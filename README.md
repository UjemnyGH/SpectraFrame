# SpectraFrame

### About
SpectraFrame is an graphics engine designed with [Vulkan API](https://www.vulkan.org) in mind.
That`s it, I wanted to make graphics engine based on Vulkan API to learn it.

### Libraries
- [Vulkan](https://www.vulkan.org) as core of entire engine rendering (Not provided in vendor folder)
- [GLFW](https://www.glfw.org) for handling window and inputs (Not provided in vendor folder)
- [stb](https://github.com/nothings/stb) for loading images from virtually any image file format
- [OpenAL](https://www.openal.org) for 3 dimmensional audio (Not provided in vendor folder)

### About Vulkan API wrapper
As you seen there is nothing more for [Vulkan API](https://www.vulkan.org) than sole, clear  [Khronos Vulkan API](https://www.vulkan.org) library taken from [LunarG](https://vulkan.lunarg.com).
[Vulkan API](https://www.vulkan.org) wrapper is created by me with easier usability in mind, without much loss in [Vulkan](https://www.vulkan.org) customizability, but there is still a lot of to develop. Current state of wrapper is quite bare bone and handles only basic functionalities to create something on screen, yet still having more potential than older API like OpenGL.

### About engine
In current stage engine have my own math implemented, a handful amount of helper functions and objects and some rudimentary framegraph that I am still working on.

### What I am working on
Currently I am working on creating string framegraph system, physics (these gonna be heavily inspired by [DOOM 3](https://github.com/id-Software/DOOM-3-BFG) physics, I just think they are cool) and single file library for compiling GLSL shaders to SPIR-V in runtime. I am gonna try to write shader compiler library in [stb](https://github.com/nothings/stb) single file library writting described way, so this should be easy to use, implement and port to other languages and/or devices.
