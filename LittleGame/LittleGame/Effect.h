#pragma once

/*
	A simple DX9 effect class. It encapsulates all effect actions, settings and such.
*/

#include <d3dx9.h>

#include "GameResource.h"

class Texture;

class Effect : public GameResource
{
protected:

	ID3DXEffect* m_effect;

#pragma region ParamHandles

	D3DXHANDLE h_matTransform;
	D3DXHANDLE h_matWorld;
	D3DXHANDLE h_matView;
	D3DXHANDLE h_matProj;
	D3DXHANDLE h_matWorldInvTrans;
	D3DXHANDLE h_diffMap;
	D3DXHANDLE h_diffCol;
	D3DXHANDLE h_specCol;
	D3DXHANDLE h_ambCol;
	D3DXHANDLE h_transp;

#pragma endregion

public:
	Effect();
	~Effect();

	virtual void Initialize(const std::string* filePath, const std::string* name = nullptr);
	virtual void Shutdown();

#pragma region Accessors

	ID3DXEffect* const GetDX9Effect() { return m_effect; }
	const std::string* GetName() { return &m_name; }
	const std::string* GetFilePath() { return &m_name; }

#pragma endregion

#pragma region AccessParams

	virtual const void SetMatTransform(const D3DXMATRIX* mat) const;
	virtual const void SetMatWorld(const D3DXMATRIX* mat) const;
	virtual const void SetMatView(const D3DXMATRIX* mat) const;
	virtual const void SetMatProj(const D3DXMATRIX* mat) const;
	virtual const void SetMatWorldInvTrans(const D3DXMATRIX* mat) const;
	virtual const void SetMapDiffuse(const Texture* map) const;
	virtual const void SetColorDiffuse(const D3DXCOLOR* col) const;
	virtual const void SetColorSpecular(const D3DXCOLOR* col) const;
	virtual const void SetColorAmbient(const D3DXCOLOR* col) const;
	virtual const void SetTransparency(const float transp) const;

#pragma endregion

};

