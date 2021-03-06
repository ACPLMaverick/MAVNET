#pragma once

#include "Singleton.h"

class Texture;
class Material;
class Mesh;

#include <map>
#include <fstream>
#include <string>

class ResourceManager :
	public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;
protected:

#pragma region Collections

	std::map<uint32_t, Material*> c_materials;
	std::map<uint32_t, Texture*> c_textures;

#pragma endregion

	ResourceManager();

public:

#pragma region Constants

	const std::string ASSET_PATH = "./Assets/";
	const std::string TEXTURES_PATH = "Textures/";
	const std::string MATERIALS_PATH = "Materials/";

	const std::string MAT_SUFFIX = ".lemat";
	const std::string BMP_SUFFIX = ".bmp";
	const std::string TGA_SUFFIX = ".tga";
	const std::string PNG_SUFFIX = ".png";
	const std::string DDS_SUFFIX = ".dds";

#pragma endregion

#pragma region Enums

	enum FileMode
	{
		BINARY,
		TEXT
	};

	enum TextureExtension
	{
		BMP,
		TGA,
		PNG,
		DDS
	};

#pragma endregion

	~ResourceManager();

	void Initialize();
	void Shutdown();

#pragma region Utility

	void LoadFileFromAssets(char** data, uint32_t* size, const std::string* filePath, FileMode mode);	// this function will be abstract
	void LoadWideTextFileFromAssets(std::wstring* data, const std::string* filePath);	// this function will be abstract

#pragma endregion

#pragma region Accessors

	bool AddMaterial(Material* const material);
	bool AddTexture(Texture* const texture);

	Material* GetMaterial(const std::string* name);
	Texture* GetTexture(const std::string* name, TextureExtension ext);

#pragma endregion
};

