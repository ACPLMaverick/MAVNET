#pragma once

class Mesh;
class Material;
class Camera;

namespace Lights
{
	class LightAmbient;
	class LightDirectional;
	class LightPoint;
}

#include "Shader.h"

#include <vector>
#include <map>

namespace Scenes
{
	class Scene
	{
		friend class Camera;
	protected:

#pragma region Protected

		Camera* _mainCamera = nullptr;

		// scene elems
		std::vector<Mesh*> _meshes;
		std::vector<Material*> _materials;
		std::vector<Lights::LightDirectional*> _lightsDirectional;
		std::vector<Lights::LightPoint*> _lightsPoint;

		Lights::LightAmbient* _lightAmbient;

		// resources
		std::map<std::wstring, Shader*> _shaders;

#pragma endregion

#pragma region Functions Protected

		virtual void SetupScene() = 0;
		template <typename T> T* GetResource(std::wstring name, std::map<std::wstring, T*>& dict)
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

		inline Shader* LoadShader(std::wstring& name) { return GetResource<Shader>(name, _shaders); }
		inline const Lights::LightAmbient* GetLightAmbient() const { return _lightAmbient; }
		inline const std::vector<Lights::LightDirectional*>& GetLightsDirectional() const { return _lightsDirectional; }
		inline const std::vector<Lights::LightPoint*>& GetLightsPoint() const { return _lightsPoint; }

#pragma endregion
	};


}