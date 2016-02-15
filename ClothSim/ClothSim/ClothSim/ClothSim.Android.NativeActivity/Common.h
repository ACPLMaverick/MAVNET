#pragma once
/*
Header file for common includes and switches
*/

#include <stdio.h>
#include <string>
#include <vector>
#include <glm\glm\glm.hpp>

// PLATFORM BUILD SWITCH
//#define PLATFORM_WINDOWS

#ifdef PLATFORM_WINDOWS

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\SOIL2.h>

typedef unsigned int uint;

#endif

// error defines
#define CS_ERR_NONE 0
#define CS_ERR_UNKNOWN 1
#define CS_ERR_EGL_INITIALIZE_FAILED 2
#define CS_ERR_WINDOW_FAILED 3
#define CS_ANDROID_ERROR 4
#define CS_ERR_RESOURCE_READ_ERROR 5
#define CS_ERR_CLOTHCOLLIDER_MESH_OBTAINING_ERROR 6
#define CS_ERR_CLOTHSIMULATOR_MESH_OBTAINING_ERROR 7
#define CS_ERR_CLOTHSIMULATOR_COLLIDER_OBTAINING_ERROR 8
#define CS_ERR_CLOTHSIMULATOR_CUDA_FAILED 9
#define CS_ERR_SHUTDOWN_PENDING 10
#define CS_ERR_ACTION_BAD_PARAM 11
//////////////////

// other helpful defines

#ifndef PLATFORM_WINDOWS

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ClothSim.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "ClothSim.NativeActivity", __VA_ARGS__))

#else

#define LOGI(...) (printf(__VA_ARGS__))
#define LOGW(...) (LOGI(__VA_ARGS__))

#endif

// structure definitions
struct TextureID
{
	int id;
	std::string name;
};

struct ShaderID
{
	int id;
	std::string name;

	int id_worldViewProj;
	int id_world;
	int id_worldInvTrans;
	int id_eyeVector;
	int id_lightDir;
	int id_lightDiff;
	int id_lightSpec;
	int id_lightAmb;
	int id_gloss;
	int id_specular;
	int id_highlight;
};

struct KernelID
{
	std::vector<int>* uniformBufferIDs;
	std::vector<int>* uniformIDs;
	std::string name;
	int id;

	KernelID()
	{
		id = -1;
		uniformBufferIDs = new std::vector<int>();
		uniformIDs = new std::vector<int>();
	}

	~KernelID()
	{
		delete uniformBufferIDs;
		delete uniformIDs;
	}
};

struct CollisonTestResult
{
	glm::vec3 colVector;
	bool ifCollision;

	CollisonTestResult()
	{
		colVector = glm::vec3(0.0f, 0.0f, 0.0f);
		ifCollision = false;
	}
};

/////////////////

// functions

void Vec3Min(const glm::vec3* vec1, const glm::vec3* vec2, glm::vec3* ret);
void Vec3Max(const glm::vec3* vec1, const glm::vec3* vec2, glm::vec3* ret);
float Vec3LengthSquared(const glm::vec3*);
void DoubleToStringPrecision(double, int, std::string*);

/////////////////