#pragma once

#include <cstdint>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

#include "System.h"

#ifdef _DEBUG

#define ASSERT_D(expression, message) \
if(!(expression)) { MessageBox(System::GetInstance()->GetHWND(), message, L"Assertion error!", MB_ICONERROR | MB_OK); exit(-1); } 

#define ASSERT(expression) ASSERT_D(expression, L"Assertion error!")

#define DEBUG(text) \
std::wstring d_text(text + L"\n"); \
OutputDebugString(d_text.c_str());

#else

#define ASSERT(expression, message)

#define ASSERT(expression)

#define DEBUG(text)

#endif