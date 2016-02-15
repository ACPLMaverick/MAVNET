#pragma once

#include "Common.h"
#include "Singleton.h"
#include "Settings.h"
#include "System.h"
#include "ResourceManager.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

class System;
class ResourceManager;

enum DrawMode { BASIC, WIREFRAME, BASIC_WIREFRAME };

#ifndef PLATFORM_WINDOWS
const string SN_BASIC = "Basic";
const string SN_WIREFRAME = "Wireframe";
const string SN_BASICWIREFRAME = "BasicWireframe";
const string SN_FONT = "Font";
#endif

/*
* Here specify the attributes of the desired configuration.
* Below, we select an EGLConfig with at least 8 bits per color
* component compatible with on-screen windows
*/

// dont forget about vsync here
#ifndef PLATFORM_WINDOWS

const EGLint attribs[] = {
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	EGL_BLUE_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_RED_SIZE, 8,
	EGL_ALPHA_SIZE, 8,
	EGL_DEPTH_SIZE, 24,
	//		EGL_STENCIL_SIZE, 8,
	EGL_NONE
};
const EGLint attribsContext[] =
{
	EGL_CONTEXT_CLIENT_VERSION, 3,
	EGL_NONE
};

#endif // !PLATFORM_WINDOWS

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;

protected:
	Renderer();

#ifdef PLATFORM_WINDOWS
	const string SN_BASIC = "Basic";
	const string SN_WIREFRAME = "Wireframe";
	const string SN_BASICWIREFRAME = "BasicWireframe";
	const string SN_FONT = "Font";
#endif

	DrawMode m_mode;

	//GLFWwindow* m_window;
	ShaderID* m_shaderID;

	ShaderID* m_basicShader;
	ShaderID* m_wireframeShader;
	ShaderID* m_basicWireframeShader;
	ShaderID* m_fontShader;

#ifdef PLATFORM_WINDOWS
	GLFWwindow* m_window;
	const string ASSET_PATH = "../ClothSim/ClothSim.Android.Packaging/assets/";
#endif

	bool m_initialized;
	bool m_resizeNeeded;

	float m_screenRatio;

	static inline char* LoadShaderFromAssets(const string* path);
	static inline char * LoadKernelFromAssets(const string * path);
	inline void ResizeViewport();
public:
	Renderer(const Renderer*);
	~Renderer();

	unsigned int Initialize();
	unsigned int Shutdown();
	unsigned int Run();

	void Flush();

	void SetDrawMode(DrawMode mode);
	void SetCurrentShader(ShaderID* id);

	ShaderID* GetCurrentShaderID();
	DrawMode GetDrawMode();
	float GetScreenRatio();

	bool GetInitialized();

	static void LoadKernel(const string* filePath, const string* newName, KernelID* data);
	static void LoadShaders(const string*, const string*, const string*, ShaderID*);
	static void ShutdownShader(ShaderID*);
	static void LoadTexture(const string*, TextureID*);
	static void LoadTexture(const string*, const unsigned char*, int, int, int, int, TextureID*);
	static void ShutdownTexture(TextureID*);

#ifndef PLATFORM_WINDOWS
	static void AHandleResize(ANativeActivity* activity, ANativeWindow* window);
#else
	GLFWwindow* GetWindowPtr();
#endif // !PLATFORM_WINDOWS
};

