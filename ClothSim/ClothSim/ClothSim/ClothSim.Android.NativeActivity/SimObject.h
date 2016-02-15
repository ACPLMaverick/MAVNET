#pragma once

/*
	This class represents a single logical object in the simulation.
*/

#include "Common.h"
#include "Component.h"
#include "Mesh.h"
#include "Transform.h"
#include "Collider.h"

#include <string>
#include <vector>

using namespace std;

class Mesh;
class Transform;
class Collider;

class SimObject
{
protected:
	unsigned int m_id;
	string m_name;
	bool m_visible;
	bool m_enabled;
	bool m_collidersDirty;

	vector<Component*> m_components;

	vector<Mesh*> m_meshes;
	// behaviourComponent collection here
	// collider collection here
	vector<Collider*> m_colliders;

	Transform* m_transform;
	// physicalObject here?
public:
	SimObject();
	SimObject(const SimObject*);
	~SimObject();

	unsigned int Initialize(string);
	unsigned int Shutdown();

	unsigned int Update();
	unsigned int Draw();

	void UpdateColliders();
	void UpdateCollidersFast();

	void AddComponent(Component*);
	void AddMesh(Mesh*);
	void AddCollider(Collider*);
	void SetTransform(Transform*);
	void SetVisible(bool);
	void SetEnabled(bool);

	void RemoveComponent(Component*);
	void RemoveMesh(Mesh*);
	void RemoveCollider(Collider*);
	void RemoveComponent(int);
	void RemoveMesh(int);
	void RemoveTransform();

	Component* GetComponent(unsigned int);
	Mesh* GetMesh(unsigned int);
	Collider* GetCollider(unsigned int);
	std::vector<Collider*>* GetColliders();
	Transform* GetTransform();
	bool GetVisible();
	bool GetEnabled();
	string* GetName();
	unsigned int GetId();
};

