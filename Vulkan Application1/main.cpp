#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string>

class EngineHandler {
private:
	const unsigned int WINDOW_WT = 800;
	const unsigned int WINDOW_HT = 600;
	const char* WINDOW_NAME = "Bored";
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_device_simulation",
		"VK_LAYER_LUNARG_monitor",
		"VK_LAYER_LUNARG_object_tracker",
		"VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_LUNARG_screenshot",
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_GOOGLE_threading",
		"VK_LAYER_GOOGLE_unique_objects",
		"VK_LAYER_LUNARG_vktrace",
		"VK_LAYER_NV_optimus",
		"VK_LAYER_RENDERDOC_Capture",
		"VK_LAYER_VALVE_steam_overlay"
	};
#ifdef NDEBUG 
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	GLFWwindow* window;
	VkInstance instance;

	
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void initWindow() {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WINDOW_WT, WINDOW_HT, WINDOW_NAME, nullptr, nullptr);

	}
	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		std::cout << "entered main loop" << std::endl;
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
		std::cout << "exit main loop" << std::endl;
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	// Creates the vulkan instance
	void createInstance() {
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("Validation layers requested, but not all available!");
		}

		VkApplicationInfo appinfo = {};
		appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appinfo.pApplicationName = WINDOW_NAME;
		appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appinfo.pEngineName = "NineCore";
		appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appinfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createinfo = {};
		createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createinfo.pApplicationInfo = &appinfo;
		if (enableValidationLayers) {
			createinfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createinfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createinfo.enabledLayerCount = 0;
		}
		//auto extensions = getRequiredExtensions();
		//createinfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		//createinfo.ppEnabledExtensionNames = extensions.data();
		
		unsigned int glfw_extensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfw_extensionCount);
		createinfo.enabledExtensionCount = glfw_extensionCount;
		createinfo.ppEnabledExtensionNames = glfwExtensions;
		createinfo.enabledLayerCount = 0;
		//*/
		VkResult res = vkCreateInstance(&createinfo, nullptr, &instance);
		if (res != VK_SUCCESS) {
			throw std::runtime_error("Failed to create vulkan instance!");
		}

	}

	// Checks for validation layers availability
	bool checkValidationLayerSupport() {
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> available_layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, available_layers.data());

		bool status = true;

		for (const char* layerName : validationLayers) {
			bool layerFound = false;
			for (const auto& layerProperties : available_layers) {
				if (std::string(layerName).compare(std::string(layerProperties.layerName))  == 0) {
					layerFound = true;
					break;
				}
			}
			if (!layerFound) {
				std::cout << "layer " << layerName << " not found!" << std::endl;
			}
			status &= layerFound;
		}
		if (!status) {
			for (const auto& layerName : available_layers) {
				std::cout << "\t" << layerName.layerName << " available" << std::endl;
			}
		}
		return status;
	}

	// Gets the required extentions
	std::vector<const char*> getRequiredExtensions() {
		std::vector<const char*> extensions;
		unsigned int glfwExtensionsCount = 0;
		const char** glfwExtension;

		glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
		for (unsigned int i = 0; i < glfwExtensionsCount; i++) {
			extensions.push_back(glfwExtension[i]);
		}
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}
		return extensions;
	}


};

int main() {
	int exit_status = EXIT_SUCCESS;
	EngineHandler app;

	try {
		app.run();

	}
	catch (const std::runtime_error& rt_err) {
		std::cout << rt_err.what() << std::endl;
		exit_status = EXIT_FAILURE;
	}
	std::cin;
	return exit_status;
}