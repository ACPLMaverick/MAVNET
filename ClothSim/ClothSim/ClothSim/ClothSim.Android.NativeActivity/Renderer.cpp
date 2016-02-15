#include "Renderer.h"
#ifndef PLATFORM_WINDOWS

#include <SOIL2.h>

#else

#include "Settings.h"

#endif // !PLATFORM_WINDOWS


Renderer::Renderer()
{
	m_initialized = false;
}

Renderer::Renderer(const Renderer*)
{
}

Renderer::~Renderer()
{
}


unsigned int Renderer::Initialize()
{
	if (m_initialized)
		return CS_ERR_NONE;
	unsigned int err = CS_ERR_NONE;

#ifdef PLATFORM_WINDOWS

	if (!glfwInit()) return CS_ERR_WINDOW_FAILED;

	glfwWindowHint(GLFW_SAMPLES, CSSET_GLFW_SAMPLES_VALUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Engine* engine = System::GetInstance()->GetEngineData();
	m_window = glfwCreateWindow(CSSET_WINDOW_WIDTH_DEFAULT, CSSET_WINDOW_HEIGHT_DEFAULT, CSSET_WINDOW_NAME, nullptr, nullptr);
	engine->width = CSSET_WINDOW_WIDTH_DEFAULT;
	engine->height = CSSET_WINDOW_HEIGHT_DEFAULT;
	m_screenRatio = (float)engine->height / (float)engine->width;

	if (m_window == nullptr)
	{
		glfwTerminate();
		return CS_ERR_WINDOW_FAILED;
	}

	glfwMakeContextCurrent(m_window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) return CS_ERR_WINDOW_FAILED;

	glfwSwapInterval(CSSET_VSYNC_ENALBED);
#else

	// initialize OpenGL ES and EGL

	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	Engine* engine = System::GetInstance()->GetEngineData();

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, attribsContext);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	m_screenRatio = (float)w / (float)h;

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;

	eglSwapInterval(engine->display, CSSET_VSYNC_ENALBED);

	AConfiguration_setNavigation(engine->app->config, ACONFIGURATION_NAVHIDDEN_YES);

#endif
	// Shaders Loading

	m_basicShader = ResourceManager::GetInstance()->LoadShader(&SN_BASIC);
	m_wireframeShader = ResourceManager::GetInstance()->LoadShader(&SN_WIREFRAME);
	m_basicWireframeShader = ResourceManager::GetInstance()->LoadShader(&SN_BASICWIREFRAME);
	m_fontShader = ResourceManager::GetInstance()->LoadShader(&SN_FONT);
	m_shaderID = m_basicShader;

	m_mode = BASIC;

	//////////// options go here

	glClearColor(CSSET_CLEAR_COLORS[0], CSSET_CLEAR_COLORS[1], CSSET_CLEAR_COLORS[2], CSSET_CLEAR_COLORS[3]);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	if (CSSET_BACKFACE_CULLING)
	{
		glEnable(GL_CULL_FACE);
	}
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DITHER);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	/*
	glEnable(GL_STENCIL);
	glStencilFunc(GL_LEQUAL, 0, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/////////////////////////

	m_initialized = true;
	return err;
}

unsigned int Renderer::Shutdown()
{
	if (!m_initialized)
		return CS_ERR_NONE;

	unsigned int err = CS_ERR_NONE;
#ifdef PLATFORM_WINDOWS

	if (m_window != nullptr)
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

#else
	Engine* engine = System::GetInstance()->GetEngineData();

	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;

#endif

	m_initialized = false;
	return err;
}

unsigned int Renderer::Run()
{
	unsigned int err = CS_ERR_NONE;
#ifndef PLATFORM_WINDOWS
	Engine* engine = System::GetInstance()->GetEngineData();

	if (engine->display == NULL || engine->context == NULL || engine->surface == NULL) 
	{
		// No display.
		return CS_ERR_UNKNOWN;
	}
#endif

	if (m_resizeNeeded)
	{
		ResizeViewport();
		m_resizeNeeded = false;
	}	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Drawing models with basic shader

	if (m_mode == BASIC)
	{
		m_shaderID = m_basicShader;
		glUseProgram(m_shaderID->id);
	}
	
	else if (m_mode == WIREFRAME) // Drawing wireframe
	{
		m_shaderID = m_wireframeShader;
		glUseProgram(m_shaderID->id);
	}

	else if (m_mode == BASIC_WIREFRAME) // Drawing wireframe with fill (no specular)
	{
		m_shaderID = m_basicWireframeShader;
		glUseProgram(m_shaderID->id);
	}

	System::GetInstance()->GetCurrentScene()->DrawObjects();

	// Drawing GUI
	m_shaderID = m_fontShader;
	glUseProgram(m_shaderID->id);
	System::GetInstance()->GetCurrentScene()->DrawGUI();

#ifdef PLATFORM_WINDOWS
	
	glfwSwapBuffers(m_window);
	glfwPollEvents();

#else

	EGLBoolean res = eglSwapBuffers(engine->display, engine->surface);
	if (res != EGL_TRUE)
	{
		LOGW("eglSwapBuffers error occured! Shutting down.");
		System::GetInstance()->Stop();
	}

#endif // PLATFORM_WINDOWS

	return err;
}

void Renderer::Flush()
{
	glFlush();
}

void Renderer::SetDrawMode(DrawMode mode)
{
	m_mode = mode;
}



void Renderer::SetCurrentShader(ShaderID* id)
{
	m_shaderID = id;
	glUseProgram(id->id);
}



ShaderID* Renderer::GetCurrentShaderID()
{
	return m_shaderID;
}


DrawMode Renderer::GetDrawMode()
{
	return m_mode;
}

float Renderer::GetScreenRatio()
{
	return m_screenRatio;
}

bool Renderer::GetInitialized()
{
	return m_initialized;
}



void Renderer::LoadKernel(const string * filePath, const string * newName, KernelID * data)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


	// Read vs code from file
	char *vertexShaderCode;
	vertexShaderCode = LoadKernelFromAssets(filePath);

	string fragmentShaderCode =
#ifdef PLATFORM_WINDOWS
		"#version 330 core  \n"
#else
		"#version 300 es	\n"
#endif
		"out vec4 color;	\n"
		"void main() { color = vec4(1.0f, 1.0f, 1.0f, 1.0f);}\0";

	GLint result = 500;
	int infoLogLength = 10;

	// Compile Vertex Shader
	LOGI("Compiling shader : %s\n", filePath->c_str());
	char const * VertexSourcePointer = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> VertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
	LOGW("%s : %s\n", filePath->c_str(), &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	LOGI("Compiling dummy fragment shader.\n");
	char const * FragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> FragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	LOGW("Dummy fragment shader: : %s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	LOGI("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> ProgramErrorMessage(glm::max(infoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
	LOGW("%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	delete[] vertexShaderCode;

	data->id = ProgramID;
	data->name = *newName;
}

void Renderer::LoadShaders(const string* vertexFilePath, const string* fragmentFilePath, const string* newName, ShaderID* n)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


	// Read vs code from file
	char *vertexShaderCode, *fragmentShaderCode;
	vertexShaderCode = LoadShaderFromAssets(vertexFilePath);
	fragmentShaderCode = LoadShaderFromAssets(fragmentFilePath);

	GLint result = 500;
	int infoLogLength = 10;

	// Compile Vertex Shader
	LOGI("Compiling shader : %s\n", vertexFilePath->c_str());
	char const * VertexSourcePointer = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> VertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
	LOGW("%s : %s\n", vertexFilePath->c_str(), &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	LOGI("Compiling shader : %s\n", fragmentFilePath->c_str());
	char const * FragmentSourcePointer = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> FragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	LOGW("%s : %s\n", fragmentFilePath->c_str(), &FragmentShaderErrorMessage[0]);

	// Link the program
	LOGI("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> ProgramErrorMessage(glm::max(infoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
	LOGW("%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	delete[] vertexShaderCode;
	delete[] fragmentShaderCode;

	n->id = ProgramID;
	n->name = (*newName);

	n->id_worldViewProj = glGetUniformLocation(ProgramID, "WorldViewProj");
	n->id_world = glGetUniformLocation(ProgramID, "World");
	n->id_worldInvTrans = glGetUniformLocation(ProgramID, "WorldInvTrans");
	n->id_eyeVector = glGetUniformLocation(ProgramID, "EyeVector");
	n->id_lightDir = glGetUniformLocation(ProgramID, "LightDir");
	n->id_lightDiff = glGetUniformLocation(ProgramID, "LightDiff");
	n->id_lightSpec = glGetUniformLocation(ProgramID, "LightSpec");
	n->id_lightAmb = glGetUniformLocation(ProgramID, "LightAmb");
	n->id_gloss = glGetUniformLocation(ProgramID, "Gloss");
	n->id_specular = glGetUniformLocation(ProgramID, "Specular");
	n->id_highlight = glGetUniformLocation(ProgramID, "Highlight");
}

char* Renderer::LoadShaderFromAssets(const string * path)
{
	string prefix = "shaders/";
	string suffix = ".glsl";
	string fPath = prefix + *path + suffix;

	// fuck you, assetmanager
#ifdef PLATFORM_WINDOWS

	fPath = Renderer::GetInstance()->ASSET_PATH + fPath;

	string sCode;
	ifstream vertexShaderStream(fPath.c_str(), ios::in);
	if (vertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(vertexShaderStream, Line))
		{
			if (Line == "#version 300 es")
			{
				Line = "#version 330 core";
			}
			sCode += "\n" + Line;
		}
		vertexShaderStream.close();
	}

	int length = sCode.length();
	char* ret = new char[length + 1];
	strcpy(ret, sCode.c_str());
	ret[length] = '\0';

	return ret;

#else

	AAssetManager* mgr = System::GetInstance()->GetEngineData()->app->activity->assetManager;

	AAsset* shaderAsset = AAssetManager_open(mgr, fPath.c_str(), AASSET_MODE_UNKNOWN);
	unsigned int length = AAsset_getLength(shaderAsset);

	char * code = new char[length + 1];

	AAsset_read(shaderAsset, (void*)code, length);
	code[length] = '\0';
	return code;

#endif
}

char* Renderer::LoadKernelFromAssets(const string * path)
{
#ifdef PLATFORM_WINDOWS
	string prefix = "kernelsPC/";
#else
	string prefix = "kernels/";
#endif
	string suffix = ".glsl";
	string fPath = prefix + *path + suffix;
#ifdef PLATFORM_WINDOWS

	fPath = Renderer::GetInstance()->ASSET_PATH + fPath;

	string sCode;
	ifstream vertexShaderStream(fPath.c_str(), ios::in);
	if (vertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(vertexShaderStream, Line))
		{
			sCode += "\n" + Line;
		}
		vertexShaderStream.close();
	}

	int length = sCode.length();
	char* ret = new char[length + 1];
	strcpy(ret, sCode.c_str());
	ret[length] = '\0';

	return ret;

#else

	AAssetManager* mgr = System::GetInstance()->GetEngineData()->app->activity->assetManager;

	AAsset* shaderAsset = AAssetManager_open(mgr, fPath.c_str(), AASSET_MODE_UNKNOWN);
	unsigned int length = AAsset_getLength(shaderAsset);

	char * code = new char[length + 1];

	AAsset_read(shaderAsset, (void*)code, length);
	code[length] = '\0';
	return code;

#endif
}

inline void Renderer::ResizeViewport()
{
	Engine* engine = System::GetInstance()->GetEngineData();
	glViewport(0, 0, engine->width, engine->height);
}

void Renderer::ShutdownShader(ShaderID* sid)
{
	glDeleteProgram(sid->id);
}

void Renderer::LoadTexture(const string* filePath, TextureID* id)
{
#ifdef PLATFORM_WINDOWS

	string nPath = Renderer::GetInstance()->ASSET_PATH + *filePath;

	id->name = *filePath;
	id->id = SOIL_load_OGL_texture(nPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);

#else

	AAssetManager* mgr = System::GetInstance()->GetEngineData()->app->activity->assetManager;
	AAsset* textureAsset = AAssetManager_open(mgr, filePath->c_str(), AASSET_MODE_UNKNOWN);
	unsigned int length = AAsset_getLength(textureAsset);

	unsigned char * bitmap = new unsigned char[length];
	AAsset_read(textureAsset, (void*)bitmap, length);

	id->name = *filePath;
	id->id = SOIL_load_OGL_texture_from_memory(bitmap, length, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);

	++length;

#endif

}

void Renderer::LoadTexture(const string* name, const unsigned char* data, int dataLength, int width, int height, int channels, TextureID* id)
{
	id->name = *name;
	
	glGenTextures(1, (GLuint*)&(id->id));
	glBindTexture(GL_TEXTURE_2D, (GLuint)(id->id));
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data
		);
}

void Renderer::ShutdownTexture(TextureID* id)
{
	glDeleteTextures(1, (GLuint*)&(id->id));
}

#ifndef PLATFORM_WINDOWS

void Renderer::AHandleResize(ANativeActivity * activity, ANativeWindow * window)
{
	int32_t w, h;
	w = ANativeWindow_getWidth(window);
	h = ANativeWindow_getHeight(window);

	Engine* engine = System::GetInstance()->GetEngineData();
	engine->width = w;
	engine->height = h;

	Renderer::GetInstance()->m_resizeNeeded = true;

	if (System::GetInstance()->GetCurrentScene() != nullptr)
		System::GetInstance()->GetCurrentScene()->FlushDimensions();

	LOGI("Renderer: Resize scheduled %dx%d", w, h);
}

#else

GLFWwindow * Renderer::GetWindowPtr()
{
	return m_window;
}

#endif // !PLATRORM_WINDOWS

