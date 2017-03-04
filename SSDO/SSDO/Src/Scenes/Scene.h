#pragma once

class Mesh;
class Material;
class Camera;

#include <vector>

namespace Scenes
{
	class Scene
	{
	protected:

#pragma region Protected

		Camera* _mainCamera = nullptr;

		std::vector<Mesh*> _meshes;
		std::vector<Material*> _materials;

#pragma endregion

#pragma region Functions Protected

		virtual void SetupScene() = 0;

#pragma endregion

	public:

#pragma region Functions Public

		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Shutdown();
		virtual void Update();
		virtual void Draw() const;

#pragma endregion
	};


}