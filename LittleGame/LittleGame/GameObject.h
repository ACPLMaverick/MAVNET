#pragma once

// It is a collection of meshes, materials, behaviours and other necessary data.

#include <vector>

class Component;
class Transform;
class Mesh;

class GameObject
{
private:

#pragma region CollectionsHierarchy

	std::vector<Component*> m_components;
	std::vector<Mesh*> m_meshes;

	std::vector<GameObject*> m_children;
	GameObject* m_parent = nullptr;

	Transform* m_transform = nullptr;

#pragma endregion

#pragma region CollectionModification

	std::vector<Component*> m_componentsToAdd;
	std::vector<std::vector<Component*>::iterator> m_componentsToRemove;
	std::vector<GameObject*> m_childrenToAdd;
	std::vector<std::vector<GameObject*>::iterator> m_childrenToRemove;
	bool m_flagComponentsToAdd = false;
	bool m_flagComponentsToRemove = false;
	bool m_flagChildrenToAdd = false;
	bool m_flagChildrenToRemove = false;

#pragma endregion

#pragma region MyData

	uint32_t m_uID = (uint32_t)-1;
	std::string m_name = "NoNameGameObject";

#pragma endregion

public:
	GameObject();
	~GameObject();

	virtual void Initialize(uint32_t uid, const std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

#pragma region AccessorsCollections

	Component* const GetComponent(uint32_t id);
	Component* const GetComponent(const std::string* name);
	GameObject* const GetChild(uint32_t id);
	GameObject* const GetChild(const std::string* name);
	GameObject* const GetParent();
	Mesh* const GetMesh(uint32_t id);
	Mesh* const GetMesh(const std::string* name);
	const std::vector<Mesh*>* GetMeshCollection();
	Transform* const GetTransform();

	void AddTransform(Transform* const transform);
	void AddComponent(Component* const component);
	void AddChild(GameObject* const child);
	void AddMesh(Mesh* const mesh);
	
	void SetParent(GameObject* const parent);

	Component* const RemoveComponent(uint32_t id);
	Component* const RemoveComponent(const std::string* name);
	Component* const RemoveComponent(const Component* ptr);
	GameObject* const RemoveChild(uint32_t id);
	GameObject* const RemoveChild(const std::string* name);
	GameObject* const RemoveChild(const GameObject* ptr);
	Mesh* const RemoveMesh(uint32_t id);
	Mesh* const RemoveMesh(const std::string* name);
	Mesh* const RemoveMesh(const Mesh* ptr);

#pragma endregion

#pragma region Accessors

	uint32_t GetUID() { return m_uID; }
	const std::string* GetName() { return &m_name; }

#pragma endregion

};

