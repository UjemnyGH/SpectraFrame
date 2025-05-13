#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "sfWindow.h"
#include "sfLogger.h"

sf::Window* sf::Window::sWindowInstancePtr = nullptr;

void sf::Window::_framebufferCallback(GLFWwindow* wnd, int width, int height) {
	((Window*)glfwGetWindowUserPointer(wnd))->resize(width, height);
}

sf::Window::Window() {
	mWindow = nullptr;
	mWindowSurface = VK_NULL_HANDLE;

	mWindowResized = false;
	mWidth = 0;
	mHeight = 0;

	if (sWindowInstancePtr) {
		SF_CLOG("ERR: Cannot create another sf::Window instance!");
		
		abort();
	}

	sWindowInstancePtr = this;
}

sf::Window::~Window() {
	destroy();
}

sf::Window& sf::Window::useVulkanDebugger(bool useDebugger) {
	mCreateVulkanDebugger = useDebugger;

	return *this;
}

sf::Window& sf::Window::create(const char* title, const int width, const int height) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!mWindow)
		SF_CLOG("ERR: Cannot create window!");

	glfwSetWindowUserPointer(mWindow, this);

	mWidth = width;
	mHeight = height;

  Vulkan::getVk().applicationInfo
		// Support for dynamic rendering, CANNOT BE LOWER THAN 1.3 TO GUARANTEE EXISTANCE OF RENDER PASS 2 AND SYNCHRONIZATION 2
		.setApiVersion(VK_API_VERSION_1_3)
		.setPApplicationName(title)
		.setPEngineName("SpectraFrame")
		.setEngineVersion(VK_MAKE_API_VERSION(0, 0, 0, 1));

  Vulkan::getVk()
#ifdef _WIN32
		.graphicsRequiredExtensions(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__linux)
		.graphicsRequiredExtensions(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif

	if (mCreateVulkanDebugger)
		Vulkan::getVk().enableDebugMessenger();

  Vulkan::getVk().createInstance();

	glfwCreateWindowSurface(Vulkan::instance(), mWindow, nullptr, (VkSurfaceKHR*)&mWindowSurface);

  Vulkan::getVk()
		.setSurface(mWindowSurface)
    .pickGPU()
		.createDevice();

	start();

	return *this;
}

sf::Window& sf::Window::runLoop() {
	while (!glfwWindowShouldClose(mWindow)) {
		update();

		glfwPollEvents();

		lateUpdate();

		mWindowResized = false;
	}

	return *this;
}

sf::Window& sf::Window::fullscreen() {
	int width = 0;
	int height = 0;

	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), nullptr, nullptr, &width, &height);
	glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);

	mWindowResized = true;

	return *this;
}

sf::Window& sf::Window::windowed() {
	glfwSetWindowMonitor(mWindow, nullptr, 0, 0, mWidth, mHeight, GLFW_DONT_CARE);

	mWindowResized = true;

	return *this;
}

sf::Window& sf::Window::resize(const int width, const int height) {
	mWindowResized = true;

	mWidth = width;
	mHeight = height;

	glfwSetWindowSize(mWindow, mWidth, mHeight);

	return *this;
}

bool sf::Window::isResized() {
	return mWindowResized;
}

uint32_t sf::Window::getWidth() {
	return mWidth;
}

uint32_t sf::Window::getHeight() {
	return mHeight;
}

void sf::Window::destroy() {
  Vulkan::waitForIdle();

	end();

  Vulkan::getVk().destroy();

	glfwDestroyWindow(mWindow);
	glfwTerminate();
}
