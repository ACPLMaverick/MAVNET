#include "stdafx.h"
#include "SSDOBase.h"
#include "System.h"
#include "Scenes\Scene.h"

namespace Postprocesses
{
	SSDOBase::SSDOBase()
	{
		_shaders.push_back(System::GetInstance()->GetScene()->LoadShader(std::wstring(L"SSDOBase_Base")));
	}


	SSDOBase::~SSDOBase()
	{
	}

	void SSDOBase::Update()
	{
	}

	void SSDOBase::SetPass(const Camera & camera, int32_t passIndex) const
	{
		Postprocess::SetPass(camera, passIndex);

		// TBD
	}
}
