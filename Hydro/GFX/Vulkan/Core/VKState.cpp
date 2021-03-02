#include "VKState.h"

#include <set>

#include "Vulkan.h"

using namespace Hydro;

//TODO - should this *not* be hardcoded? Idk
const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_NV_RAY_TRACING_EXTENSION_NAME
};

VKState::VKState(Window* window_){
	CreateInstance(window_);

	#ifdef _DEBUG
	CreateDebugMessenger();
	#endif //_DEBUG

	surface = window_->VK_CreateSurface(instance.get());
	SelectPhysicalDevice();
	CreateLogicalDevice();
}

VKState::~VKState(){
	instance->destroySurfaceKHR(surface);

	#ifdef _DEBUG
	DestroyDebugUtilsMessengerEXT(*instance, debugMessenger, nullptr);
	//instance->destroyDebugUtilsMessengerEXT(debugMessenger); //This causes a linker error for some reason
	#endif //_DEBUG

	//Device and Instance are destroyed automatically
}

void VKState::CreateInstance(Window* window_){
	#ifdef _DEBUG
	if(!CheckValidationLayerSupport()){
		throw new std::runtime_error("Could not request VK validation layers!");
	}
	#endif //_DEBUG

	auto extensions = GetRequiredExtensions(window_);

	auto appInfo = vk::ApplicationInfo(
		window_->Name().c_str(),
		VK_MAKE_VERSION(0, 0, 1), //TODO - Get game version somehow
		"Hydro",
		VK_MAKE_VERSION(0, 0, 1), //TODO - Get engine version somehow
		VK_API_VERSION_1_0
	);

	auto createInfo = vk::InstanceCreateInfo(
		vk::InstanceCreateFlags(),
		&appInfo,
		0, nullptr, //Layers
		static_cast<uint32_t>(extensions.size()), extensions.data() //Extentsions
	);

	#ifdef _DEBUG
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();

	vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	PopulateDebugMessengerCreateInfo(debugCreateInfo);
	createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	#endif //_DEBUG

	try{
		instance = vk::createInstanceUnique(createInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create VK Instance! VK Error:" + std::string(err.what()));
	}
}

void VKState::SelectPhysicalDevice(){
	auto devices = instance->enumeratePhysicalDevices();

	if(devices.empty()){
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	for(const auto& device : devices){
		if(IsDeviceSuitable(device)){
			physicalDevice = device;
			break;
		}
	}

	if(!physicalDevice){
		throw std::exception("Failed to find GPUs with Vulkan support!");
	}
}

void VKState::CreateLogicalDevice(){
	QueueFamilyIndices indices = Vulkan::FindQueueFamilies(physicalDevice, surface);

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for(uint32_t queueFamily : uniqueQueueFamilies){
		queueCreateInfos.push_back(vk::DeviceQueueCreateInfo(
			vk::DeviceQueueCreateFlags(),
			queueFamily,
			1, //Queue Count
			&queuePriority
		));
	}

	auto deviceFeatures = vk::PhysicalDeviceFeatures();
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE;

	auto createInfo = vk::DeviceCreateInfo(
		vk::DeviceCreateFlags(),
		static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(),
		0, nullptr,
		static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
		&deviceFeatures
	);

	#ifdef _DEBUG
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
	#endif //_DEBUG

	try{
		device = physicalDevice.createDeviceUnique(createInfo);
	}catch(vk::SystemError err){
		throw std::runtime_error("Failed to create logical device! VK Error: " + std::string(err.what()));
	}

	graphicsQueue = device->getQueue(indices.graphicsFamily.value(), 0);
	presentQueue = device->getQueue(indices.presentFamily.value(), 0);
}

std::vector<const char*> VKState::GetRequiredExtensions(Window* window_){
	std::vector<const char*> sdlExtensions = window_->VK_GetExtensions();
	std::vector<const char*> extensions(sdlExtensions.data(), sdlExtensions.data() + sdlExtensions.size());

	#ifdef _DEBUG
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	#endif //_DEBUG

	return extensions;
}

bool VKState::DeviceSupportsExtensions(const vk::PhysicalDevice& device_){
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for(const auto& e : device_.enumerateDeviceExtensionProperties()){
		requiredExtensions.erase(std::string(e.extensionName));
	}

	return requiredExtensions.empty();
}

bool VKState::IsDeviceSuitable(const vk::PhysicalDevice& device_){
	QueueFamilyIndices indices = Vulkan::FindQueueFamilies(device_, surface);
	bool extensionsSupported = DeviceSupportsExtensions(device_);

	bool swapChainAdequate = false;
	if(extensionsSupported){
		SwapChainSupportDetails swapChainSupport = Vulkan::QuerySwapChainSupport(device_, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.IsComplete() && extensionsSupported && swapChainAdequate && device_.getFeatures().samplerAnisotropy; //TODO - Aniso support should be optional
}

#ifdef _DEBUG
bool VKState::CheckValidationLayerSupport(){
	for(const char* layerName : validationLayers){
		bool layerFound = false;
		for(const auto& layerProperties : vk::enumerateInstanceLayerProperties()){
			if(strcmp(layerName, layerProperties.layerName) == 0){
				layerFound = true;
				break;
			}
		}

		if(!layerFound){
			return false;
		}
	}

	return true;
}

void VKState::CreateDebugMessenger(){
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	PopulateDebugMessengerCreateInfo(createInfo);

	if(CreateDebugUtilsMessengerEXT(*instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS){
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void VKState::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo){
	createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
		vk::DebugUtilsMessengerCreateFlagsEXT(),
		/*vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |*/ vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		debugCallback
	);
}

VkResult VKState::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger){
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if(func != nullptr){
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VKState::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator){
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if(func != nullptr){
		func(instance, debugMessenger, pAllocator);
	}
}

#endif //_DEBUG