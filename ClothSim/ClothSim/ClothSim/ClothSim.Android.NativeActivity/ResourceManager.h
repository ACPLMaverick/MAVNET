#pragma once

/*
	It governs loading, keeping in memory and freeing any external resources the simulation may use.
*/

#include "Common.h"
#include "Singleton.h"

#include <string>
#include <map>

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

private:
	std::map<std::string, TextureID*> m_texturesNames;
	std::map<std::string, ShaderID*> m_shadersNames;
	std::map<std::string, KernelID*> m_kernelNames;

	std::map<unsigned int, TextureID*> m_texturesIds;
	std::map<unsigned int, ShaderID*> m_shadersIds;
	std::map<unsigned int, KernelID*> m_kernelIds;

	// helpers
	unsigned char* h_whiteTexture;
	TextureID** h_textures;

	ResourceManager();
	TextureID* LoadTextureData(const std::string*);
	ShaderID* LoadShaderData(const std::string*, const std::string*);
	KernelID* LoadKernelData(const std::string*);
public:
	ResourceManager(const ResourceManager*);
	~ResourceManager();

	unsigned int Initialize();
	unsigned int Shutdown();

	TextureID* LoadTexture(const std::string*);
	KernelID* LoadKernel(const std::string*);
	ShaderID* LoadShader(const std::string*);
	ShaderID* LoadShader(const std::string*, const std::string*);

	TextureID* GetTexture(const std::string*);
	TextureID* GetTexture(unsigned int);
	TextureID* GetTextureWhite();
	ShaderID* GetShader(const std::string*);
	ShaderID* GetShader(unsigned int);
	KernelID* GetKernel(const std::string*);
	KernelID* GetKernel(unsigned int);
};

