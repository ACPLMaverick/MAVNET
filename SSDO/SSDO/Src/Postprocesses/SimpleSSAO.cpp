#include "stdafx.h"
#include "SimpleSSAO.h"
#include "Shader.h"
#include "System.h"
#include "Scenes\Scene.h"

namespace Postprocesses
{
	SimpleSSAO::SimpleSSAO()
	{
		_shaders.push_back(System::GetInstance()->GetScene().LoadShader(std::wstring(L"SimpleSSAO_Base")));
		_shaders.push_back(System::GetInstance()->GetScene().LoadShader(std::wstring(L"SimpleSSAO_BlurMerge")));
	}

	SimpleSSAO::~SimpleSSAO()
	{
	}

	void SimpleSSAO::Update()
	{
	}
}