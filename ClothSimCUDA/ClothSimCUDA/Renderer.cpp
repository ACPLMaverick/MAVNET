#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::Renderer(const Renderer*)
{
}

Renderer::~Renderer()
{
}


unsigned int Renderer::Initialize()
{
	unsigned int err = CS_ERR_NONE;

	if (!glfwInit()) return CS_ERR_GLFW_INITIALIZE_FAILED;

	glfwWindowHint(GLFW_SAMPLES, CSSET_GLFW_SAMPLES_VALUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(CSSET_WINDOW_WIDTH, CSSET_WINDOW_HEIGHT, CSSET_WINDOW_NAME, nullptr, nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		return CS_ERR_WINDOW_FAILED;
	}

	glfwMakeContextCurrent(m_window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) return CS_ERR_GLEW_INITIALIZE_FAILED;

	// Shaders Loading
	
	ResourceManager::GetInstance()->LoadShader(&SN_BASIC);
	ResourceManager::GetInstance()->LoadShader(&SN_BASIC, &SN_WIREFRAME);
	ResourceManager::GetInstance()->LoadShader(&SN_FONT);
	m_shaderID = ResourceManager::GetInstance()->GetShader((unsigned int)0);

	m_mode = BASIC;

	//////////// options go here

	glClearColor(CSSET_CLEAR_COLORS[0], CSSET_CLEAR_COLORS[1], CSSET_CLEAR_COLORS[2], CSSET_CLEAR_COLORS[3]);
	glEnable(GL_DEPTH_TEST);

	if (CSSET_BACKFACE_CULLING)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	glEnable(GL_STENCIL);
	glStencilFunc(GL_LEQUAL, 0, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(8.0f);
	glPointSize(5.0f);
	
	glDepthFunc(GL_LEQUAL);
	
	glfwSwapInterval(CSSET_VSYNC_ENALBED);

	/////////////////////////

	return err;
}

unsigned int Renderer::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	if (m_window != nullptr)
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

	return err;
}

unsigned int Renderer::Run()
{
	unsigned int err = CS_ERR_NONE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Drawing models with basic shader
	if (m_mode == BASIC || m_mode == BASIC_WIREFRAME)
	{
		m_shaderID = ResourceManager::GetInstance()->GetShader(&SN_BASIC);
		glUseProgram(m_shaderID->id);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		System::GetInstance()->GetCurrentScene()->Draw();
	}

	// Drawing wireframe
	if (m_mode == WIREFRAME || m_mode == BASIC_WIREFRAME)
	{
		m_shaderID = ResourceManager::GetInstance()->GetShader(&SN_WIREFRAME);
		glUseProgram(m_shaderID->id);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		System::GetInstance()->GetCurrentScene()->Draw();

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		System::GetInstance()->GetCurrentScene()->Draw();
	}

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return err;
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


GLFWwindow* Renderer::GetWindow()
{
	return m_window;
}

DrawMode Renderer::GetDrawMode()
{
	return m_mode;
}



void Renderer::LoadShaders(const string* vertexFilePath, const string* fragmentFilePath, const string* newName, ShaderID* n)
{
	string extension = ".glsl";

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read vs code from file
	string vertexShaderCode;
	ifstream vertexShaderStream((*vertexFilePath + extension).c_str(), ios::in);
	if (vertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(vertexShaderStream, Line))
			vertexShaderCode += "\n" + Line;
		vertexShaderStream.close();
	}

	string fragmentShaderCode;
	ifstream fragmentShaderStream((*fragmentFilePath + extension).c_str(), ios::in);
	if (fragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(fragmentShaderStream, Line))
			fragmentShaderCode += "\n" + Line;
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertexFilePath->c_str());
	char const * VertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> VertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragmentFilePath->c_str());
	char const * FragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> FragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> ProgramErrorMessage(glm::max(infoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

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
	n->id_highlight = glGetUniformLocation(ProgramID, "Highlight");
}

void Renderer::ShutdownShader(ShaderID* sid)
{
	glDeleteProgram(sid->id);
}

void Renderer::LoadTexture(const string* filePath, TextureID* id)
{
	id->name = *filePath;
	id->id = SOIL_load_OGL_texture((*filePath).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
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