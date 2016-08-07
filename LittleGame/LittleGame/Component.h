#pragma once

class GameObject;

class Component
{
protected:

	std::string m_name = "NoNameComponent";
	uint32_t m_uID;
	GameObject* m_obj;

public:
	Component();
	~Component();

	virtual void Initialize(uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update() = 0;
	virtual void LateUpdate();
	virtual void Draw() = 0;

#pragma region Accessors

	uint32_t GetUID() { return m_uID; }
	const std::string* GetName() { return &m_name; }
	GameObject* GetMyGameObject() { return m_obj; }

#pragma endregion
};

