#pragma once

// a container for all game elements, also govern their update and draw cycle
// This is an abstract class. InitializeScene can be implemented to set
// objects which will be contained here

#include <vector>

class GameObject;
class Camera;

class Scene
{
protected:

#pragma region Elements

	std::vector<GameObject*> m_gameObjects;
	std::vector<Camera*> m_cameras;

#pragma endregion

#pragma region Variables

	std::string m_name = "NoNameScene";
	uint32_t m_uID = (uint32_t)-1;
	uint32_t m_currentCamera = 0;

	std::vector<GameObject*> m_gameObjectsToAdd;
	std::vector<std::vector<GameObject*>::iterator> m_gameObjectsToRemove;
	bool m_flagToAddGameObject = false;
	bool m_flagToRemoveGameObject = false;

#pragma endregion

#pragma region MethodsInternal

	virtual void InitializeScene() = 0;

#pragma endregion
public:
	Scene();
	~Scene();

#pragma region MethodsMain

	void Initialize(uint32_t uID, std::string* name);
	void Shutdown();
	void Update();
	void Draw();

#pragma endregion

#pragma region Accessors

	uint32_t GetUID() { return m_uID; }
	const std::string* GetName() { return &m_name; }
	Camera* const GetCurrentCamera() { return (m_cameras.size() > 0 ? m_cameras[m_currentCamera] : nullptr); }

	GameObject* const GetGameObject(uint32_t uid);
	GameObject* const GetGameObject(std::string* name);
	Camera* const GetCamera(uint32_t uid);
	Camera* const GetCamera(std::string* name);

	void AddGameObject(GameObject* const gameObject);
	void AddCamera(Camera* const camera);

	GameObject* const RemoveGameObject(uint32_t uid);
	GameObject* const RemoveGameObject(std::string* name);
	Camera* const RemoveCamera(uint32_t uid);
	Camera* const RemoveCamera(std::string* name);

#pragma endregion
};

