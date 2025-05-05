#pragma once
#ifndef _SPECRTAFRAME_WINDOW_
#define _SPECRTAFRAME_WINDOW_

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux)
#define VK_USE_PLATFORM_XLIB_KHR
#define GLFW_EXPOSE_NATIVE_X11
#endif

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


namespace sf {
	class Window {
	private:
		GLFWwindow* mWindow;

		vk::SurfaceKHR mWindowSurface;

		vk::Instance mInstance;
		vk::PhysicalDevice mPhysicalDevice;
		vk::Device mDevice;

		bool mWindowResized;
		int mWidth;
		int mHeight;

		bool mCreateVulkanDebugger;

		static void _framebufferCallback(GLFWwindow* wnd, int width, int height);

	public:
		static Window* sWindowInstancePtr;

		static vkw::VkWrapCore& getVk();
		
		Window();

		~Window();

		virtual void start() {}

		virtual void update() {}

		virtual void lateUpdate() {}

		virtual void end() {}

		Window& useVulkanDebugger(bool useDebugger = false);

		Window& create(const char* title, const int width, const int height);

		Window& runLoop();

		Window& fullscreen();

		Window& windowed();

		Window& resize(const int width, const int height);

		bool isResized();

		uint32_t getWidth();

		uint32_t getHeight();

		void destroy();
	};
}

#endif