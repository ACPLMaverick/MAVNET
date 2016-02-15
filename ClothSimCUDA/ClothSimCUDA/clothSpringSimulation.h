#pragma once

#include "Common.h"
#include "BoxAACollider.h"
#include "SphereCollider.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <thrust\device_vector.h>
#include <stdio.h>
#include <vector>
#include <stack>

/*
	Header file for Cloth Spring Simulation CUDA class.
*/

#define VERTEX_NEIGHBOURING_VERTICES 4
#define COLLISION_CHECK_WINDOW_SIZE 4
#define ALMOST_ZERO 0.000000001f

struct Vertex
{
	glm::vec3 force;
	glm::vec3 velocity;
	glm::vec3 prevPosition;

	unsigned int neighbours[VERTEX_NEIGHBOURING_VERTICES];
	float neighbourMultipliers[VERTEX_NEIGHBOURING_VERTICES];
	float springLengths[VERTEX_NEIGHBOURING_VERTICES];

	unsigned int id;
	float mass;
	float elasticity;
	float elasticityDamp;
	float dampCoeff;
	float lockMultiplier;
	float colliderMultiplier;

	Vertex()
	{
		force = glm::vec3(0.0f, 0.0f, 0.0f);
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		prevPosition = glm::vec3(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < VERTEX_NEIGHBOURING_VERTICES; ++i)
		{
			neighbours[i] = 0xFFFFFFFF;
			neighbourMultipliers[i] = 1.0f;
			springLengths[i] = 0.0f;
		}

		id = 0xFFFFFFFF;
		elasticityDamp = 0.0f;
		mass = 0.0f;
		dampCoeff = 0.0f;
		lockMultiplier = 1.0f;
		elasticity = 0.0f;
		colliderMultiplier = 0.0f;
	}
};

struct Spring
{
	// positive accounts for first vertex, negative for second one
	glm::vec3 force;

	unsigned int idFirst;
	unsigned int idSecond;

	float baseLength;
	float elasticity;
};

struct cBoxAAData
{
	glm::vec3 min;
	glm::vec3 max;
};

class clothSpringSimulation
{
private:
	const double MAX_DELTA = 33.3;
	const double FIXED_DELTA = 0.006f;

	cudaDeviceProp* m_deviceProperties;

	const float VERTEX_MASS = 1.0f;
	const float VERTEX_AIR_DAMP = 0.01f;
	const float SPRING_ELASTICITY = 50.00f;
	const float SPRING_BORDER_MULTIPLIER = 50.0f;
	const float SPRING_ELASTICITY_DAMP = -100.25f;
	const float VERTEX_COLLIDER_MULTIPLIER = 0.5f;
	const float CELL_OFFSET = 0.01f;
	
	double lastDelta;

	unsigned int m_vertexCount;
	unsigned int m_springCount;
	unsigned int m_vertexPositionSize;
	unsigned int m_vertexNormalSize;
	unsigned int m_vertexColorSize;
	unsigned int m_allEdgesWidth;
	unsigned int m_allEdgesLength;
	unsigned int m_boxColliderCount;
	unsigned int m_sphereColliderCount;
	unsigned int m_cTreeNodeDepth;

	Vertex* m_vertices;
	Spring* m_springs;
	cBoxAAData* m_globalBounds;
	
	float m_cellSize;

	glm::vec3* m_posPtr;
	glm::vec3* m_nrmPtr;
	glm::vec4* m_colPtr;

	// device memory pointers
	Vertex* i_vertexPtr;
	Spring* i_springPtr;
	glm::vec3* i_posPtr;
	glm::vec3* i_nrmPtr;
	glm::vec4* i_colPtr;
	float* i_wmPtr;
	BoxAAData* i_bcldPtr;
	SphereData* i_scldPtr;
	/////////////////

	inline cudaError_t CalculateForces(float gravity, double delta, int steps, 
		BoxAAData* boxColliders, SphereData* sphereColliders, glm::mat4* transform);
	inline void FreeMemory();
public:
	clothSpringSimulation();
	~clothSpringSimulation();

	unsigned int ClothSpringSimulationInitialize(
		unsigned int vertexPositionSize, 
		unsigned int vertexNormalSize, 
		unsigned int vertexColorSize,
		unsigned int edgesWidth,
		unsigned int edgesLength,
		unsigned int boxColliderCount,
		unsigned int sphereColliderCount,
		glm::vec3* vertexPositionPtr, 
		glm::vec3* vertexNormalPtr, 
		glm::vec4* vertexColorPtr
		);
	unsigned int ClothSpringSimulationUpdate(float gravity, double delta, int steps, 
		BoxAAData* boxColliders, SphereData* sphereColliders, glm::mat4* transform);
	unsigned int ClothSpringSimulationShutdown();
};

