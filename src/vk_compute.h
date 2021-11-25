#pragma once

#include <vk_types.h>
#include <vector>

enum ComputeBufferType {
	UNIFORM,
	STORAGE,
	TEXTURE_STORAGE,
	TEXTURE_SAMPLED
};

struct ComputeBinding {
	VkDescriptorSetLayoutBinding binding;
	size_t size;
	VmaMemoryUsage memoryUsage;
	ComputeBufferType bufferType;
	AllocatedBuffer buffer;
	bool isExternal;
	VkDescriptorImageInfo imageInfo; //only for textures
};

struct ComputeInstance {
	// PIPELINE
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;

	// BUFFERS
	VkDescriptorSet descriptorSet;

	std::vector<ComputeBinding> bindings;
};

class VulkanCompute {
public:
	void init(VkDevice device, VmaAllocator allocator, VkQueue computeQueue, uint32_t computeQueueFamily);
	void create_buffer(ComputeInstance& computeInstance, ComputeBufferType bufferType, VmaMemoryUsage memoryUsage, size_t size);
	void add_buffer_binding(ComputeInstance& computeInstance, ComputeBufferType bufferType, AllocatedBuffer buffer);
	void add_texture_binding(ComputeInstance& computeInstance, ComputeBufferType bufferType, VkSampler sampler, VkImageView imageView);
	void build(ComputeInstance& computeInstance, VkDescriptorPool descriptorPool, const char* computeShader);
	void compute(ComputeInstance& computeInstance, int x, int y, int z);
	void destroy_compute_instance(ComputeInstance& computeInstance);
private:
	VkDevice _device;
	VkQueue _computeQueue;
	uint32_t _computeQueueFamily;
	CommandContext _computeContext;
	VmaAllocator _allocator;

	VkDescriptorType computeDescriptorTypes[4] = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	};

	VkBufferUsageFlagBits computeBufferUsageBits[2] = {
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
	};
};