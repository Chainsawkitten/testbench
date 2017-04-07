#include "MaterialVulkan.hpp"

#include <iostream>
#include <fstream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

int MaterialVulkan::materialIndex = 0;

MaterialVulkan::MaterialVulkan(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorPool descriptorPool) {
    this->device = device;
    this->physicalDevice = physicalDevice;
    this->swapChainExtent = swapChainExtent;
    this->renderPass = renderPass;
    this->descriptorPool = descriptorPool;
    
    shaderExtensions[ShaderType::VS] = "vert";
    shaderExtensions[ShaderType::GS] = "geom";
    shaderExtensions[ShaderType::PS] = "frag";
    shaderExtensions[ShaderType::CS] = "comp";
    
    shaderStageFlags[ShaderType::VS] = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStageFlags[ShaderType::GS] = VK_SHADER_STAGE_GEOMETRY_BIT;
    shaderStageFlags[ShaderType::PS] = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStageFlags[ShaderType::CS] = VK_SHADER_STAGE_COMPUTE_BIT;
    
    createBuffer(sizeof(float) * 4, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &diffuseBuffer, &diffuseBufferMemory);
}

MaterialVulkan::~MaterialVulkan() {
    vkDestroyBuffer(device, diffuseBuffer, nullptr);
    vkFreeMemory(device, diffuseBufferMemory, nullptr);
    
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    
    // Clean up descriptor set layouts.
    for (VkDescriptorSetLayout layout : descriptorSetLayouts)
        vkDestroyDescriptorSetLayout(device, layout, nullptr);
    
    // Clean up shader modules.
    for (auto& it : shaderModules)
        vkDestroyShaderModule(device, it.second, nullptr);
}

void MaterialVulkan::setShader(const std::string& shaderFileName, ShaderType type) {
    if (shaderFileNames.find(type) != shaderFileNames.end())
        removeShader(type);
    
    shaderFileNames[type] = shaderFileName;
}

void MaterialVulkan::removeShader(ShaderType type) {
    UNIMPLEMENTED
}

int MaterialVulkan::compileMaterial(std::string& errString) {
    // Compile shaders.
    if (compileShader(ShaderType::VS, errString) < 0) {
        std::cerr << errString << std::endl;
        return -1;
    }
    
    if (compileShader(ShaderType::PS, errString) < 0) {
        std::cerr << errString << std::endl;
        return -1;
    }
    
    // Create shader stages.
    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = createShaderStage(ShaderType::VS);
    VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = createShaderStage(ShaderType::PS);
    
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo};
    
    // Vertex input.
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    
    // Input assembly.
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    // Viewport.
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = swapChainExtent.width;
    viewport.height = swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    // Scissor.
    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    
    // Viewport state info.
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    
    // Rasterizer.
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    if (materialIndex++ < 1) {
        rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
    } else {
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    }
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    
    // Multisampling.
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    
    // Color blending.
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    
    // Descriptor set layout.
    createDescriptorSetLayouts();
    
    // Pipeline layout.
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        std::cerr << "Failed to create pipeline layout." << std::endl;
        return -1;
    }
    
    // Create pipeline.
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        std::cerr << "Failed to create graphics pipeline." << std::endl;
        return -1;
    }
    
    return 0;
}

int MaterialVulkan::enable() {
    UNIMPLEMENTED
    return 0;
}

void MaterialVulkan::disable() {
    UNIMPLEMENTED
}

void MaterialVulkan::setDiffuse(Color c) {
    UNIMPLEMENTED
}

void MaterialVulkan::updateConstantBuffer(const void* data, size_t size, unsigned int location) {
    // Copy data from data to mapped memory.
    void* mappedMemory;
    vkMapMemory(device, diffuseBufferMemory, 0, size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(device, diffuseBufferMemory);
}

void MaterialVulkan::addConstantBuffer(std::string name, unsigned int location) {
    // Intentionally left unimplemented.
}

VkPipeline MaterialVulkan::getPipeline() const {
    return graphicsPipeline;
}

VkPipelineLayout MaterialVulkan::getPipelineLayout() const {
    return pipelineLayout;
}

VkDescriptorSet MaterialVulkan::getDiffuseDescriptorSet() const {
    return diffuseDescriptorSet;
}

int MaterialVulkan::compileShader(ShaderType type, std::string& errString) {
    // Read shader file into string.
    std::string shaderText = readFile(shaderFileNames[type]);
    
    // Add defines.
    std::string outShaderText = "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";
    for (const std::string& define : shaderDefines[type])
        outShaderText += define;
    outShaderText += shaderText;
    
    // Output to temp file.
    std::ofstream outShaderFile("temp." + shaderExtensions[type]);
    outShaderFile << outShaderText;
    outShaderFile.close();
    
    // Compile to SPIR-V.
    /// @todo Don't use a system call. Seriously...
    system(("glslangValidator.exe -V temp." + shaderExtensions[type]).c_str());
    
    // Read binary shader.
    std::vector<char> binaryShader = readFile2(shaderExtensions[type] + ".spv");
    
    // Create shader module.
    createShaderModule(type, binaryShader);
    
    return 0;
}

void MaterialVulkan::createShaderModule(ShaderType type, const std::vector<char>& source) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = (uint32_t*) source.data();
    
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModules[type]) != VK_SUCCESS) {
        std::cerr << "Failed to create shader module." << std::endl;
        exit(-1);
    }
}

VkPipelineShaderStageCreateInfo MaterialVulkan::createShaderStage(ShaderType type) {
    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = shaderStageFlags[type];
    createInfo.module = shaderModules[type];
    createInfo.pName = "main";
    
    return createInfo;
}

std::string MaterialVulkan::readFile(const std::string& filename) {
    std::ifstream shaderFile(filename);
    std::string text;
    if (shaderFile.is_open()) {
        text = std::string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
        shaderFile.close();
    } else {
        std::cerr << "Cannot find file: " << filename << std::endl;
        exit(-1);
    }
    
    return text;
}

std::vector<char> MaterialVulkan::readFile2(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Cannot find file: " << filename << std::endl;
        exit(-1);
    }
    
    std::size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    
    return buffer;
}

void MaterialVulkan::createDescriptorSetLayouts() {
    VkDescriptorSetLayout layout;
    
    // Position storage buffer.
    VkDescriptorSetLayoutBinding layoutBinding = {};
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        std::cerr << "Failed to create descriptor set layout." << std::endl;
        exit(-1);
    }
    
    descriptorSetLayouts.push_back(layout);
    
    // Transform uniform buffer.
    layoutBinding.binding = 5;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        std::cerr << "Failed to create descriptor set layout." << std::endl;
        exit(-1);
    }
    
    descriptorSetLayouts.push_back(layout);
    
    // Normal storage buffer.
    layoutBinding.binding = 1;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        std::cerr << "Failed to create descriptor set layout." << std::endl;
        exit(-1);
    }
    
    descriptorSetLayouts.push_back(layout);
    
    // Texture coordinate storage buffer.
    layoutBinding.binding = 2;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        std::cerr << "Failed to create descriptor set layout." << std::endl;
        exit(-1);
    }
    
    descriptorSetLayouts.push_back(layout);
    
    // Diffuse uniform buffer.
    layoutBinding.binding = 6;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        std::cerr << "Failed to create descriptor set layout." << std::endl;
        exit(-1);
    }
    
    descriptorSetLayouts.push_back(layout);
    
    createUniformDescriptorSet(layout);
    
    // Texture.
    if (materialIndex == 3) {
        layoutBinding.binding = 0;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        
        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
            std::cerr << "Failed to create descriptor set layout." << std::endl;
            exit(-1);
        }
        
        descriptorSetLayouts.push_back(layout);
    }
}

void MaterialVulkan::createUniformDescriptorSet(VkDescriptorSetLayout layout) {
    // Allocate descriptor set.
    VkDescriptorSetAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = descriptorPool;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = &layout;
    
    if (vkAllocateDescriptorSets(device, &allocateInfo, &diffuseDescriptorSet) != VK_SUCCESS) {
        std::cerr << "Failed to allocate descriptor set" << std::endl;
        exit(-1);
    }
    
    // Update descriptor set.
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = diffuseBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(float) * 4;
    
    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = diffuseDescriptorSet;
    descriptorWrite.dstBinding = 6;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    
    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}

void MaterialVulkan::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
    // Create buffer.
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
        std::cerr << "Failed to create buffer." << std::endl;
        exit(-1);
    }
    
    // Get information about device memory.
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);
    
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    // Find suitable memory type.
    uint32_t memoryType;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (memoryRequirements.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties )
            memoryType = i;
    }
    
    // Allocate buffer memory.
    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = memoryType;
    
    if (vkAllocateMemory(device, &allocateInfo, nullptr, bufferMemory) != VK_SUCCESS) {
        std::cerr << "Failed to allocate buffer memory." << std::endl;
        exit(-1);
    }
    
    vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}
