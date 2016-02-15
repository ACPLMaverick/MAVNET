#pragma once

// It is a collection of meshes, materials, behaviours and other necessary data.

#include <vector>

class Component;
class Transform;

class GameObject
{
private:

#pragma region CollectionsHierarchy

	std::vector<Component*> m_components;

	std::vector<GameObject*> m_children;
	GameObject* m_parent = nullptr;

	Transform* m_transform = nullptr;

#pragma endregion

#pragma region MyData

	uint32_t m_uID = (uint32_t)-1;
	std::string m_name = "NoNameGameObject";

#pragma endregion

public:
	GameObject();
	~GameObject();

	virtual void Initialize(uint32_t uid, const std::string* name);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

#pragma region AccessorsCollections

	const Component* GetComponent(uint32_t id);
	const Component* GetComponent(const std::string* name);
	const GameObject* GetChild(uint32_t id);
	const GameObject* GetChild(const std::string* name);
	const GameObject* GetParent();
	const Transform* GetTransform();

	void AddComponent(Component* const component);
	void AddChild(GameObject* const child);
	
	void SetParent(GameObject* const parent);

	const Component* RemoveComponent(uint32_t id);
	const Component* RemoveComponent(const std::string* name);
	const Component* RemoveComponent(const Component* ptr);
	const GameObject* RemoveChild(uint32_t id);
	const GameObject* RemoveChild(const std::string* name);
	const GameObject* RemoveChild(const GameObject* ptr);

#pragma endregion

#pragma region Accessors

	uint32_t GetUID() { return m_uID; }
	const std::string* GetName() { return &m_name; }

#pragma endregion

};

