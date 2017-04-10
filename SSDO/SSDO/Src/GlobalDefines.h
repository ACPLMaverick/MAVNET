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

#define ASSERT_X(func) ASSERT(func == S_OK)

#define DEBUG(text) \
std::wstring d_text(text + L"\n"); \
OutputDebugString(d_text.c_str());

#else

#define ASSERT_D(expression, message) expression

#define ASSERT(expression) expression

#define ASSERT_X(func) func

#define DEBUG(text)

#endif