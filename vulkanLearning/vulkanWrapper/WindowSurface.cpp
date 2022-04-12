#include "WindowSurface.h"

namespace FF::Wrapper {

	Wrapper::WindowSurface::WindowSurface(Instance::Inst_Ptr instance, Window::Ptr window):
		mInstance(instance), mWindow(window)
	{
		if (glfwCreateWindowSurface(instance->getInstance(), window->getGlfwWindow(), nullptr, &mSurface) != VK_SUCCESS) {

			throw std::runtime_error("Error: failed create window surface.");
		}
	}

	Wrapper::WindowSurface::~WindowSurface()
	{
		vkDestroySurfaceKHR(mInstance->getInstance(), mSurface, nullptr);
		mInstance.reset();
	}
}
