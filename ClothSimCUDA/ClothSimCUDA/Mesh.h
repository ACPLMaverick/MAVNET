#pragma once

/*
This class represents an abstraction of a single drawable mesh.
*/

#include "Common.h"
#include "SimObject.h"
#include "Component.h"

#include <vector>

class SimObject;
class Component;

class Mesh : public Component
{
protected:
	TextureID* m_texID;
	float m_gloss;
	
	virtual void GenerateVertexData() = 0;
public:
	Mesh(SimObject* obj);
	Mesh(const Mesh*);
	~Mesh();

	virtual unsigned int Initialize() = 0;
	virtual unsigned int Shutdown() = 0;

	virtual unsigned int Update() = 0;
	virtual unsigned int Draw() = 0;

	virtual void SetTextureID(TextureID*) final;
	virtual void SetGloss(float);
	virtual TextureID* GetTextureID() final;
	virtual float GetGloss() final;
};

