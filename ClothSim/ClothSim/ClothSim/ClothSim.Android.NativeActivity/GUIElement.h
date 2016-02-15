#pragma once

/*
	An empty representation of an element of a GUI.
*/

#include "Common.h"
#include "Mesh.h"

#include <string>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtx\transform.hpp>
#include <map>

class GUIElement
{
	friend class Renderer;
protected:
	std::map<std::string, GUIElement*> m_children;

	std::string m_id;
	glm::mat4 m_transform;
	glm::mat4 m_rot;

	glm::vec2 m_position;
	glm::vec2 m_scale;

	Mesh* m_mesh;
	TextureID* m_textureIdle;

	float m_rotation;

	bool m_isEnabled;
	bool m_isVisible;
	bool m_isBlockable;
	bool m_isScaled;
	bool isClickInProgress;

	virtual void GenerateTransformMatrix();
public:
	GUIElement(const std::string*);
	GUIElement(const GUIElement*);
	~GUIElement();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();

	void SetPosition(glm::vec2);
	void SetScale(glm::vec2);
	void SetRotation(float r);
	void SetEnabled(bool val);
	void SetVisible(bool val);
	void SetBlockable(bool val);
	void SetScaled(bool val);

	void AddChild(GUIElement* ge);
	GUIElement* GetChild(const std::string* id);
	GUIElement* RemoveChild(const std::string* id);

	std::string* GetID();
	glm::mat4* GetTransformMatrix();
	glm::mat4* GetRotationMatrix();
	glm::vec2 GetPosition();
	glm::vec2 GetScale();
	float GetRotation();
	bool GetHoldInProgress();
	bool GetEnabled();
	bool GetVisible();
	bool GetBlockable();
	bool GetScaled();

	virtual unsigned int ExecuteClick(const glm::vec2* clickPos);
	virtual unsigned int ExecuteHold(const glm::vec2* clickPos);
	virtual void CleanupAfterHold();

	virtual void FlushDimensions();
};

