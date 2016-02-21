#include "stdafx.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Effect.h"
#include "Texture.h"

#include <d3dx9math.h>

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initialize()
{
	// hard-coded resource initialization
	///////////////

	// empty material
	std::string nEEffect = "SpriteEffect";
	std::string nETexture = "LE_WHITE";
	D3DXCOLOR nETexColor = D3DXCOLOR(0.2f, 0.5f, 0.8f, 1.0f);
	std::string neMaterial = "LE_EMPTY_WHITE";
	std::string neMaterialFakePath = ASSET_PATH + MATERIALS_PATH + neMaterial + MAT_SUFFIX;
	Effect* sprite = GetEffect(&nEEffect);

	Texture* eTex = new Texture();
	eTex->Initialize(&nETexture, 2U, 2U, &nETexColor);
	AddTexture(eTex);

	Material* eMat = new Material();
	eMat->Initialize(0, &neMaterialFakePath, &neMaterial);
	eMat->SetTextureDiffuse(eTex);
	eMat->SetEffect(sprite);
	AddMaterial(eMat);
}

void ResourceManager::Shutdown()
{
	for (std::map< uint32_t, Material*>::iterator it = c_materials.begin(); it != c_materials.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = nullptr;
	}
	for (std::map< uint32_t, Effect*>::iterator it = c_effects.begin(); it != c_effects.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = nullptr;
	}
	for (std::map< uint32_t, Texture*>::iterator it = c_textures.begin(); it != c_textures.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = nullptr;
	}
}

void ResourceManager::LoadFileFromAssets(char * data, uint32_t* size, const std::string * filePath, FileMode mode)
{
	std::string fPath = *filePath;
	data = nullptr;

	std::string fileData;
	int binary = (mode == FileMode::BINARY) ? std::ios::binary : 0;

	std::ifstream fileStream(fPath.c_str(), std::ios::in | binary);

	int length;
	char* ret;
	switch (mode)
	{
	case FileMode::BINARY:

		if (fileStream.is_open())
		{
			while (!fileStream.eof())
			{
				fileData += fileStream.get();
			}
		}
		else
			return;

		length = fileData.length();
		ret = new char[length];
		strcpy(ret, fileData.c_str());

		data = ret;
		*size = length;

		break;

	case FileMode::TEXT:

		if (fileStream.is_open())
		{
			std::string Line = "";
			while (std::getline(fileStream, Line))
			{
				fileData += "\n" + Line;
			}
			fileStream.close();
		}
		else
			return;

		length = fileData.length();
		ret = new char[length + 1];
		strcpy(ret, fileData.c_str());
		ret[length] = '\0';

		data = ret;
		*size = (length + 1);

		break;

	}
}

void ResourceManager::LoadWideTextFileFromAssets(std::wstring * data, const std::string * filePath)
{
	std::string fPath = *filePath;
	data->clear();

	std::wifstream fileStream(fPath.c_str(), std::ios::in);
	if (fileStream.is_open())
	{
		std::wstring Line = L"";
		while (std::getline(fileStream, Line))
		{
			*data += L"\n" + Line;
		}
		fileStream.close();
	}
	else
		return;

}

bool ResourceManager::AddEffect(Effect * const effect)
{
	bool inColl = c_effects.count(effect->GetHash());

	if (inColl)
	{
		return false;
	}
	else
	{
		c_effects.emplace(effect->GetHash(), effect);
	}
}

bool ResourceManager::AddMaterial(Material * const material)
{
	bool inColl = c_materials.count(material->GetHash());

	if (inColl)
	{
		return false;
	}
	else
	{
		c_materials.emplace(material->GetHash(), material);
	}
}

bool ResourceManager::AddTexture(Texture * const texture)
{
	bool inColl = c_textures.count(texture->GetHash());

	if (inColl)
	{
		return false;
	}
	else
	{
		c_textures.emplace(texture->GetHash(), texture);
	}
}

Effect * ResourceManager::GetEffect(const std::string * name)
{
	std::string fullFilePath = ASSET_PATH + EFFECTS_PATH + *name + FX_SUFFIX;
	uint32_t hash = std::hash<std::string>()(fullFilePath);

	std::map<uint32_t, Effect*>::iterator check = c_effects.find(hash);

	if (check != c_effects.end())
		return (*check).second;
	else
	{
		Effect* ef = new Effect();
		ef->Initialize(&fullFilePath, name);

		c_effects.emplace(ef->GetHash(), ef);
		return ef;
	}
}

Material * ResourceManager::GetMaterial(const std::string * name)
{
	std::string fullFilePath = ASSET_PATH + MATERIALS_PATH + *name + MAT_SUFFIX;
	uint32_t hash = std::hash<std::string>()(fullFilePath);

	std::map<uint32_t, Material*>::iterator check = c_materials.find(hash);

	if (check != c_materials.end())
		return (*check).second;
	else
	{
		Material* ef = new Material();
		ef->Initialize(0, &fullFilePath, name);

		c_materials.emplace(ef->GetHash(), ef);
		return ef;
	}
}

Texture * ResourceManager::GetTexture(const std::string * name, TextureExtension ext)
{
	std::string suffix;

	switch (ext)
	{
	case ResourceManager::BMP:
		suffix = BMP_SUFFIX;
		break;
	case ResourceManager::TGA:
		suffix = TGA_SUFFIX;
		break;
	case ResourceManager::PNG:
		suffix = PNG_SUFFIX;
		break;
	case ResourceManager::DDS:
		suffix = DDS_SUFFIX;
		break;
	default:
		break;

	}
	std::string fullFilePath = ASSET_PATH + TEXTURES_PATH + *name + suffix;
	uint32_t hash = std::hash<std::string>()(*name);

	std::map<uint32_t, Texture*>::iterator check = c_textures.find(hash);

	if (check != c_textures.end())
		return (*check).second;
	else
	{
		Texture* ef = new Texture();
		std::string fullFilePath = TEXTURES_PATH + *name + suffix;
		ef->Initialize(&fullFilePath, name);

		c_textures.emplace(ef->GetHash(), ef);
		return ef;
	}
}
