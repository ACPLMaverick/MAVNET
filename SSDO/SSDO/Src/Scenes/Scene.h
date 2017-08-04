#pragma once

class Camera;
class Object;
class Controller;
class Text;

namespace Lights
{
	class LightAmbient;
	class LightDirectional;
	class LightPoint;
}

namespace Postprocesses
{
	class Postprocess;
}

#include "GlobalDefines.h"

#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Font.h"

#include <vector>
#include <map>

namespace Scenes
{
	class Scene
	{
		friend class Controller;
		friend class Camera;
	protected:

#pragma region Protected

		Controller* _controller = nullptr;

		Camera* _mainCamera = nullptr;

		Shader* _textShader = nullptr;

		// scene elems
		std::vector<Object*> _objects;
		std::vector<Lights::LightDirectional*> _lightsDirectional;
		std::vector<Lights::LightPoint*> _lightsPoint;
		std::vector<Postprocesses::Postprocess*> _postprocesses;
		std::vector<Text*> _texts;

		Lights::LightAmbient* _lightAmbient;

		// resources
		std::map<const std::wstring, Shader*> _shaders;
		std::map<const std::wstring, Mesh*> _meshes;
		std::map<const std::wstring, Material*> _materials;
		std::map<const std::wstring, Font*> _fonts;

#pragma endregion

#pragma region Functions Protected

		virtual void SetupScene() = 0;
		template <typename T> T* GetResource(const std::wstring name, std::map<const std::wstring, T*>& dict)
		{
			T* ret = nullptr;
			if ((ret = dict[name]) != nullptr)
			{
				return ret;
			}
			else
			{
				ret = T::CreateResource(name);
				dict.emplace(name, ret);
				return ret;
			}
		}

#pragma endregion

	public:

#pragma region Functions Public

		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Shutdown();
		virtual void Update();
		virtual void Draw() const;

		inline const Camera* GetMainCamera() const { return _mainCamera; }

		inline Shader* LoadShader(const std::wstring& name) { return GetResource<Shader>(name, _shaders); }
		inline Mesh* LoadMesh(const std::wstring& name) { return GetResource<Mesh>(name, _meshes); }
		inline Material* LoadMaterial(const std::wstring& name) { return GetResource<Material>(name, _materials); }
		inline Font* LoadFont(const std::wstring& name) { return GetResource<Font>(name, _fonts); }

		inline const Lights::LightAmbient* GetLightAmbient() const { return _lightAmbient; }
		inline const std::vector<Lights::LightDirectional*>& GetLightsDirectional() const { return _lightsDirectional; }
		inline const std::vector<Lights::LightPoint*>& GetLightsPoint() const { return _lightsPoint; }
		inline const std::vector<Postprocesses::Postprocess*>& GetPostprocesses() const { return _postprocesses; }
		inline const std::vector<Text*>& GetTexts() const { return _texts; }

		inline const Shader* GetTextShader() const { return _textShader; }

#pragma endregion
	};


}