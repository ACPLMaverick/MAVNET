
#include "clothSpringSimulation.h"

__device__ inline void CUDAVec3Min(const glm::vec3* vec1, const glm::vec3* vec2, glm::vec3* ret)
{
	ret->x = glm::min(vec1->x, vec2->x);
	ret->y = glm::min(vec1->y, vec2->y);
	ret->z = glm::min(vec1->z, vec2->z);
}

__device__ inline void CUDAVec3Max(const glm::vec3* vec1, const glm::vec3* vec2, glm::vec3* ret)
{
	ret->x = glm::max(vec1->x, vec2->x);
	ret->y = glm::max(vec1->y, vec2->y);
	ret->z = glm::max(vec1->z, vec2->z);
}

__device__ inline float CUDAVec3LengthSquared(const glm::vec3* vec)
{
	return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

__device__ inline void CUDASolveBoxAACollision(const glm::vec3* boxMin, const glm::vec3* boxMax, const glm::vec3* center, const float radius, const float multiplier, glm::vec3* ret)
{
	glm::vec3 cls, closest;

	CUDAVec3Max(center, boxMin, &cls);
	CUDAVec3Min(&cls, boxMax, &closest);

	float distance = CUDAVec3LengthSquared(&(closest - *center));


	if (distance < (radius * radius))
	{
		closest = *center - closest;
		*ret += normalize(closest) * (radius - sqrt(distance)) * multiplier;
	}
}

__device__ inline void CUDASolveSphereCollision(const glm::vec3* sphereCenter, const float sphereRadius, const glm::vec3* center, const float radius, const float multiplier, glm::vec3* ret)
{
	glm::vec3 diff = *center - *sphereCenter;
	float diffLength = CUDAVec3LengthSquared(&diff);

	if (diffLength < (radius + sphereRadius) * (radius + sphereRadius) && diffLength != 0.0f)
	{
		diff = glm::normalize(diff);
		diff = diff * ((radius + sphereRadius) - glm::sqrt(diffLength)) * multiplier;

		*ret += diff;
	}
}

__device__ inline float CUDASign(float s)
{
	return ((float)signbit(s)) * 2.0f - 1.0f;
}


__global__ void CalculateSpringsKernel(
	Vertex* vertPtr, 
	Spring* springPtr, 
	glm::vec3* posPtr, 
	glm::vec3* nrmPtr, 
	glm::vec4* colPtr, 
	const float grav,
	const int N
	)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	int v_cur = (i * gridDim.y * blockDim.y) + j;

	if (v_cur >= N)
		return;

	posPtr[springPtr[v_cur].idFirst].y = posPtr[springPtr[v_cur].idFirst].y - 0.00055f;
	//posPtr[springPtr[v_cur].idSecond].y -= 0.00055f;
}

__global__ void CalculateForcesKernel(
	Vertex* vertPtr, 
	glm::vec3* posPtr, 
	glm::vec3* nrmPtr, 
	glm::vec4* colPtr, 
	const float grav, 
	const float delta,
	const unsigned int N
	)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	int v_cur = (i * gridDim.y * blockDim.y) + j;

	if (v_cur >= N)
		return;
	
		vertPtr[v_cur].force = glm::vec3(0.0f, 0.0f, 0.0f);
		int id = vertPtr[v_cur].id;

		// calculate elasticity force for each neighbouring vertices
		for (int i = 0; i < VERTEX_NEIGHBOURING_VERTICES; ++i)
		{
			
			glm::vec3 mPos = posPtr[vertPtr[v_cur].id];
			glm::vec3 nPos = posPtr[vertPtr[v_cur].neighbours[i]];

			// direction of the force
			glm::vec3 f = mPos - nPos;
			glm::vec3 n = glm::normalize(f);
			float fLength = glm::length(f);
			float spring = fLength - vertPtr[v_cur].springLengths[i];
			glm::vec3 springiness = -vertPtr[vertPtr[v_cur].neighbours[i]].elasticity * spring * n * vertPtr[v_cur].neighbourMultipliers[i];

			glm::vec3 dV = vertPtr[v_cur].velocity - vertPtr[vertPtr[v_cur].neighbours[i]].velocity;
			float damp = vertPtr[vertPtr[v_cur].neighbours[i]].elasticityDamp * (glm::dot(dV, f) / fLength);
			glm::vec3 damping = damp * n * vertPtr[v_cur].neighbourMultipliers[i];

			float sL = glm::length(springiness);
			float dL = glm::length(damping);
			damping = (damping / glm::max(dL, 0.00000000000000001f)) * glm::min(sL, dL);
			
			vertPtr[v_cur].force += (springiness + damping);
		}


		// calculate gravity force
		vertPtr[v_cur].force +=
			vertPtr[v_cur].mass * glm::vec3(0.0f, -grav / 100.0f, 0.0f);

		// calculate air damp force
		vertPtr[v_cur].force +=
			-vertPtr[v_cur].dampCoeff * vertPtr[v_cur].velocity;

		// ?calculate repulsive force?

		// check hooks
		vertPtr[v_cur].force *= vertPtr[v_cur].lockMultiplier;

		// calculate acceleration and use Verelet integration to calculate position
		glm::vec3 newPos;
		glm::vec3 acc = vertPtr[v_cur].force / vertPtr[v_cur].mass;


		newPos = 2.0f * posPtr[id] - vertPtr[v_cur].prevPosition + acc * delta * delta;

		// update velocity
		vertPtr[v_cur].velocity = (newPos - vertPtr[v_cur].prevPosition) / delta;

		vertPtr[v_cur].prevPosition = posPtr[id];
		posPtr[id] = newPos;
}

__global__ void CalculateExternalCollisionsKernel(
	Vertex* vertPtr,
	glm::vec3* posPtr,
	BoxAAData* boxColliders,
	SphereData* sphereColliders,
	float* worldMatrix,
	const int boxColliderCount,
	const int sphereColliderCount,
	const int N
	)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	int v_cur = (i * gridDim.y * blockDim.y) + j;

	if (v_cur >= N)
		return;

	/// calculate vertex position in world space
	glm::vec4 v = glm::vec4(posPtr[v_cur], 1.0f);
	glm::vec4 r;

	// matrix by vector multiplication
	r.x = v.x * (worldMatrix)[0] + v.y * (worldMatrix)[4] + v.z * (worldMatrix)[8] + v.w * (worldMatrix)[12];
	r.y = v.x * (worldMatrix)[1] + v.y * (worldMatrix)[5] + v.z * (worldMatrix)[9] + v.w * (worldMatrix)[13];
	r.z = v.x * (worldMatrix)[2] + v.y * (worldMatrix)[6] + v.z * (worldMatrix)[10] + v.w * (worldMatrix)[14];
	r.w = v.x * (worldMatrix)[3] + v.y * (worldMatrix)[7] + v.z * (worldMatrix)[11] + v.w * (worldMatrix)[15];

	glm::vec3 wPos;
	wPos.x = r.x;
	wPos.y = r.y;
	wPos.z = r.z;
	////////////////////
	/*
	float radius = 0.0f;
	float divisor = 0.0f;
	for (int i = 0; i < VERTEX_NEIGHBOURING_VERTICES; ++i)
	{
		radius += vertPtr[v_cur].springLengths[i] * vertPtr[v_cur].neighbourMultipliers[i];
		divisor += vertPtr[v_cur].neighbourMultipliers[i];
	}*/
	float radius = vertPtr[v_cur].colliderMultiplier;

	glm::vec3 colVec;
	// solve for boxes
	for (int i = 0; i < boxColliderCount; ++i)
	{
		CUDASolveBoxAACollision(&boxColliders[i].min, &boxColliders[i].max, &wPos, radius, 1.0f, &colVec);
	}

	// solve for spheres
	for (int i = 0; i < sphereColliderCount; ++i)
	{
		CUDASolveSphereCollision(&sphereColliders[i].center, sphereColliders[i].radius, &wPos, radius, 1.0f, &colVec);
	}

	posPtr[v_cur] += colVec;
}

__global__ void CalculateInternalCollisionsSpatialSubdivisionKernel(
	Vertex* vertPtr,
	glm::vec3* posPtr,
	cBoxAAData globalCol,
	float cellSize,
	const int N
	)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	int v_cur = (i * gridDim.y * blockDim.y) + j;

	if (v_cur >= N)
		return;

	// UTL, UTR, UBR, UBL, DTL, DTR, DBR, DBL
	cBoxAAData mCol = globalCol;
	glm::vec3 diff = mCol.max - mCol.min;
	cBoxAAData children[8];
	glm::vec3 mCenter = posPtr[v_cur];
	while (
		diff.x > cellSize &&
		diff.y > cellSize &&
		diff.z > cellSize
		)
	{
		glm::vec3 center;
		center.x = mCol.min.x + mCol.max.x / 2.0f;
		center.y = mCol.min.y + mCol.max.y / 2.0f;
		center.z = mCol.min.z + mCol.max.z / 2.0f;
		//// generate children

		// UTL
		children[0].min = glm::vec3(mCol.min.x, center.y, mCol.min.z);
		children[0].max = glm::vec3(center.x, mCol.max.y, center.z);

		// UTR
		children[1].min = glm::vec3(center.x, center.y, mCol.min.z);
		children[1].max = glm::vec3(mCol.max.x, mCol.max.y, center.z);

		// UBR
		children[2].min = glm::vec3(center.x, center.y, center.z);
		children[2].max = glm::vec3(mCol.max.x, mCol.max.y, mCol.max.z);

		// UBL
		children[3].min = glm::vec3(mCol.min.x, center.y, center.z);
		children[3].max = glm::vec3(center.x, mCol.max.y, mCol.max.z);

		// DTL
		children[4].min = glm::vec3(mCol.min.x, mCol.min.y, mCol.min.z);
		children[4].max = glm::vec3(center.x, center.y, center.z);

		// DTR
		children[5].min = glm::vec3(center.x, mCol.min.y, mCol.min.z);
		children[5].max = glm::vec3(mCol.max.x, center.y, center.z);

		// DBR
		children[6].min = glm::vec3(center.x, mCol.min.y, center.z);
		children[6].max = glm::vec3(mCol.max.x, center.y, mCol.max.z);

		// DBL
		children[7].min = glm::vec3(mCol.min.x, mCol.min.y, center.z);
		children[7].max = glm::vec3(center.x, center.y, mCol.max.z);

		/////////////

		//// pick one
		for (int i = 0; i < 8; ++i)
		{
			if (mCenter.x >= children[i].min.x &&
				mCenter.y >= children[i].min.y &&
				mCenter.z >= children[i].min.z &&
				mCenter.x <= children[i].max.x &&
				mCenter.y <= children[i].max.y &&
				mCenter.z <= children[i].max.z
				)
			{
				mCol = children[i];
				break;
			}
		}

		diff = mCol.max - mCol.min;
	}


	float radius = 0.0f;
	float divisor = 0.0f;
	for (int i = 0; i < VERTEX_NEIGHBOURING_VERTICES; ++i)
	{
		radius += vertPtr[v_cur].springLengths[i] * vertPtr[v_cur].neighbourMultipliers[i];
		divisor += vertPtr[v_cur].neighbourMultipliers[i];
	}
	radius = radius / divisor * vertPtr[v_cur].colliderMultiplier;

	for (int i = 0; i < N; ++i)
	{
		glm::vec3 colVec;

		glm::vec3 oCenter = posPtr[i];
		if (
			oCenter.x >= mCol.min.x &&
			oCenter.y >= mCol.min.y &&
			oCenter.z >= mCol.min.z &&
			oCenter.x <= mCol.max.x &&
			oCenter.y <= mCol.max.y &&
			oCenter.z <= mCol.max.z &&
			i != v_cur
			)
		{
			CUDASolveSphereCollision(&posPtr[i], radius, &mCenter, radius, 0.5f, &colVec);
		}

		posPtr[v_cur] += colVec;
	}
}

__global__ void CalculateInternalCollisionsSimpleKernel(
	Vertex* vertPtr,
	glm::vec3* posPtr,
	const int N
	)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	int v_cur = (i * gridDim.y * blockDim.y) + j;

	if (v_cur >= N)
		return;

	glm::vec3 colVec;
	glm::vec3 mCenter = posPtr[v_cur];
	glm::vec3 oCenter;
	float radius = 0.0f;
	float divisor = 0.0f;
	for (int i = 0; i < VERTEX_NEIGHBOURING_VERTICES; ++i)
	{
		radius += vertPtr[v_cur].springLengths[i] * vertPtr[v_cur].neighbourMultipliers[i];
		divisor += vertPtr[v_cur].neighbourMultipliers[i];
	}
	radius = radius / divisor * vertPtr[v_cur].colliderMultiplier * 0.1f;

	for (int i = 0; i < N; ++i)
	{
		if (i == v_cur)
			continue;

		CUDASolveSphereCollision(&posPtr[i], radius, &mCenter, radius, 0.5f, &colVec);
	}

	posPtr[v_cur] += colVec;
}

__global__ void CalculateInternalCollisionsNeighboursOnlyKernel(
	Vertex* vertPtr,
	glm::vec3* posPtr,
	const unsigned int edgesLength,
	const int N
	)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	int v_cur = (i * gridDim.y * blockDim.y) + j;

	if (v_cur != 0)
		return;

	unsigned int nCount = (2 * COLLISION_CHECK_WINDOW_SIZE + 1)*(2 * COLLISION_CHECK_WINDOW_SIZE + 1) - 1;
	unsigned int nIds[(2 * COLLISION_CHECK_WINDOW_SIZE + 1)*(2 * COLLISION_CHECK_WINDOW_SIZE + 1) - 1];

	unsigned int w = 0;
	for (int i = -COLLISION_CHECK_WINDOW_SIZE; i <= COLLISION_CHECK_WINDOW_SIZE; ++i)
	{
		for (int j = -COLLISION_CHECK_WINDOW_SIZE; j <= COLLISION_CHECK_WINDOW_SIZE; ++j)
		{
			if (i == 0 && j == 0)
				continue;

			nIds[w] = abs((i * (int)edgesLength + j + v_cur) % N);
			++w;
		}
	}

	glm::vec3 colVec = glm::vec3(0.0f, 0.0f, 0.0f);
	float radius = 0.0f;
	/*
	float divisor = 0.0f;
	for (int i = 0; i < VERTEX_NEIGHBOURING_VERTICES; ++i)
	{
		radius += vertPtr[v_cur].springLengths[i] * vertPtr[v_cur].neighbourMultipliers[i];
		divisor += vertPtr[v_cur].neighbourMultipliers[i];
	}*/
	radius = vertPtr[v_cur].colliderMultiplier * 0.1f;

	for (int i = 0; i < nCount; ++i)
	{
		CUDASolveSphereCollision(&posPtr[nIds[i]], radius, &posPtr[v_cur], radius, 0.5f, &colVec);
	}

	posPtr[v_cur] += colVec;
}

__global__ void CalculateNormalsKernel(
	Vertex* vertPtr,  
	glm::vec3* posPtr, 
	glm::vec3* nrmPtr, 
	const int N
	)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	int v_cur = (i * gridDim.y * blockDim.y) + j;

	if (v_cur >= N)
		return;

	Vertex* vert = &(vertPtr[v_cur]);
	glm::vec3 normal = glm::vec3();

	for (int i = 0; i < VERTEX_NEIGHBOURING_VERTICES; ++i)
	{
		glm::vec3 diff1 = posPtr[vert->id] - posPtr[vert->neighbours[i]];
		glm::vec3 diff2 = posPtr[vert->id] - posPtr[vert->neighbours[(i + 1) % VERTEX_NEIGHBOURING_VERTICES]];

		normal += (glm::cross(diff1, diff2) * vert->neighbourMultipliers[i] * vert->neighbourMultipliers[(i + 1) % VERTEX_NEIGHBOURING_VERTICES]);
	}

	normal = glm::normalize(normal);
	normal.z = -normal.z;
	nrmPtr[vert->id] = normal;
}

//////////////////////////////////////////////////////

clothSpringSimulation::clothSpringSimulation()
{
}

clothSpringSimulation::~clothSpringSimulation()
{

}

unsigned int clothSpringSimulation::ClothSpringSimulationInitialize(
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
	)
{
	cudaError_t cudaStatus;

	// save data given to function
	m_vertexPositionSize = vertexPositionSize;
	m_vertexNormalSize = vertexNormalSize;
	m_vertexColorSize = vertexColorSize;
	m_allEdgesWidth = edgesWidth;
	m_allEdgesLength = edgesLength;
	m_vertexCount = m_allEdgesLength * m_allEdgesWidth;
	m_springCount = (m_allEdgesLength - 1) * (m_allEdgesWidth) + (m_allEdgesWidth - 1) * m_allEdgesLength;
	m_boxColliderCount = boxColliderCount;
	m_sphereColliderCount = sphereColliderCount;
	m_posPtr = vertexPositionPtr;
	m_nrmPtr = vertexNormalPtr;
	m_colPtr = vertexColorPtr;

	lastDelta = FIXED_DELTA;

	m_globalBounds = new cBoxAAData;

	// generate vertex and spring arrays, to help with computations

	m_vertices = new Vertex[m_vertexCount];

	glm::vec3 baseLength = glm::vec3(
		abs(m_posPtr[0].x - m_posPtr[m_vertexCount - 1].x) / (float)(m_allEdgesWidth - 1),
		0.0f,
		abs(m_posPtr[0].z - m_posPtr[m_vertexCount - 1].z) / (float)(m_allEdgesLength - 1)
		);

	m_cellSize = glm::max(baseLength.x, baseLength.z) * 1.5f;

	for (int i = 0; i < m_vertexCount; ++i)
	{
		m_vertices[i].id = i;
		m_vertices[i].mass = VERTEX_MASS;
		m_vertices[i].lockMultiplier = 1.0f;
		m_vertices[i].prevPosition = m_posPtr[i];
		m_vertices[i].dampCoeff = VERTEX_AIR_DAMP;

		m_vertices[i].elasticity = SPRING_ELASTICITY;
		m_vertices[i].elasticityDamp = SPRING_ELASTICITY_DAMP;
		m_vertices[i].colliderMultiplier = VERTEX_COLLIDER_MULTIPLIER;

		if (i < m_allEdgesLength ||
			i >= (m_vertexCount - m_allEdgesLength) ||
			i % m_allEdgesLength == 0 ||
			i % m_allEdgesLength == (m_allEdgesLength - 1)
			)
		{
			m_vertices[i].elasticity *= SPRING_BORDER_MULTIPLIER;
			//m_vertices[i].elasticityDamp *= SPRING_BORDER_MULTIPLIER;
		}

		// calculating neighbouring vertices ids and spring lengths

		// upper
		m_vertices[i].neighbours[0] = (i - 1) % m_vertexCount;
		if (i % m_allEdgesLength)
		{
			m_vertices[i].neighbourMultipliers[0] = 1.0f;
			m_vertices[i].springLengths[0] = baseLength.z;
		}	
		else
		{
			m_vertices[i].neighbourMultipliers[0] = 0.0f;
			m_vertices[i].springLengths[0] = 0.0f;
		}

		// left
		m_vertices[i].neighbours[1] = (i - m_allEdgesLength) % m_vertexCount;
		if (i >= m_allEdgesLength)
		{
			m_vertices[i].neighbourMultipliers[1] = 1.0f;
			m_vertices[i].springLengths[1] = baseLength.x;
		}
		else
		{
			m_vertices[i].neighbourMultipliers[1] = 0.0f;
			m_vertices[i].springLengths[1] = 0.0f;
		}

		// lower
		m_vertices[i].neighbours[2] = (i + 1) % m_vertexCount;
		if (i % m_allEdgesLength != (m_allEdgesLength - 1))
		{
			m_vertices[i].neighbourMultipliers[2] = 1.0f;
			m_vertices[i].springLengths[2] = baseLength.z;
		}
		else
		{
			m_vertices[i].neighbourMultipliers[2] = 0.0f;
			m_vertices[i].springLengths[2] = 0.0f;
		}

		// right
		m_vertices[i].neighbours[3] = (i + m_allEdgesLength) % m_vertexCount;
		if (i < (m_vertexCount - m_allEdgesLength))
		{
			m_vertices[i].neighbourMultipliers[3] = 1.0f;
			m_vertices[i].springLengths[3] = baseLength.x;
		}
		else
		{
			m_vertices[i].neighbourMultipliers[3] = 0.0f;
			m_vertices[i].springLengths[3] = 0.0f;
		}
	}

	// hard-coded locks
	m_vertices[0].lockMultiplier = 0.0f;
	m_vertices[(m_vertexCount - m_allEdgesLength)].lockMultiplier = 0.0f;
	/*
	m_springs = new Spring[m_springCount];

	for (int i = 0, s = 0; i < m_vertexCount; ++i)
	{
		// do I create "lower" spring?
		if (i % m_allEdgesLength != (m_allEdgesLength - 1))
		{
			m_springs[s].idFirst = i;
			m_springs[s].idSecond = i + 1;

			m_springs[s].baseLength = glm::length(m_posPtr[i] - m_posPtr[i + 1]);
			m_springs[s].elasticity = SPRING_ELASTICITY;

			++s;
		}

		// do I create "right" spring?
		if (i < (m_vertexCount - m_allEdgesLength))
		{
			m_springs[s].idFirst = i;
			m_springs[s].idSecond = i + m_allEdgesLength;

			m_springs[s].baseLength = glm::length(m_posPtr[i] - m_posPtr[i + 1]);
			m_springs[s].elasticity = SPRING_ELASTICITY;

			++s;
		}
	}
	*/

	// that fucking tree


	//////////////////////////////

	// Get Device info
	m_deviceProperties = new cudaDeviceProp;
	cudaStatus = cudaGetDeviceProperties(m_deviceProperties, 0);
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaGetDeviceProperties failed!  Do you have a CUDA-capable GPU installed?");
		FreeMemory();
		return cudaStatus;
	}

	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		FreeMemory();
		return cudaStatus;
	}

	// Allocate GPU buffers for six vectors (2 input for helpers, 3 input for buffers) and one float (gravity)
	cudaStatus = cudaMalloc((void**)&i_vertexPtr, m_vertexCount * sizeof(Vertex));
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaMalloc for vertex helper buffer failed!");
		FreeMemory();
		return cudaStatus;
	}

	//cudaStatus = cudaMalloc((void**)&i_springPtr, m_springCount * sizeof(Spring));
	//if (cudaStatus != cudaSuccess) {
	//	printf("CUDA: cudaMalloc for spring helper buffer failed!");
	//	FreeMemory();
	//	return cudaStatus;
	//}

	cudaStatus = cudaMalloc((void**)&i_posPtr, m_vertexCount * m_vertexPositionSize);
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaMalloc for position buffer failed!");
		FreeMemory();
		return cudaStatus;
	}

	cudaStatus = cudaMalloc((void**)&i_nrmPtr, m_vertexCount * m_vertexNormalSize);
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaMalloc for normal buffer failed!");
		FreeMemory();
		return cudaStatus;
	}

	cudaStatus = cudaMalloc((void**)&i_colPtr, m_vertexCount * m_vertexColorSize);
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaMalloc for color buffer failed!");
		FreeMemory();
		return cudaStatus;
	}

	cudaStatus = cudaMalloc((void**)&i_bcldPtr, m_boxColliderCount * sizeof(BoxAACollider));
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaMalloc for box colliders failed!");
		FreeMemory();
		return cudaStatus;
	}

	cudaStatus = cudaMalloc((void**)&i_scldPtr, m_sphereColliderCount * sizeof(SphereCollider));
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaMalloc for box colliders failed!");
		FreeMemory();
		return cudaStatus;
	}

	cudaStatus = cudaMalloc((void**)&i_wmPtr, sizeof(glm::mat4));
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaMalloc for world matrix failed!");
		FreeMemory();
		return cudaStatus;
	}

	// copy helper buffers to device memory
	cudaStatus = cudaMemcpy(i_vertexPtr, m_vertices, m_vertexCount * sizeof(Vertex), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return cudaStatus;
	}

	//cudaStatus = cudaMemcpy(i_springPtr, m_springs, m_springCount * sizeof(Spring), cudaMemcpyHostToDevice);
	//if (cudaStatus != cudaSuccess) {
	//	fprintf(stderr, "cudaMemcpy failed!");
	//	FreeMemory();
	//	return cudaStatus;
	//}
}

unsigned int clothSpringSimulation::ClothSpringSimulationUpdate(float gravity, double delta, int steps,
	BoxAAData* boxColliders, SphereData* sphereColliders, glm::mat4* transform)
{
	// Update bounds
	m_globalBounds->min = glm::vec3();
	m_globalBounds->max = glm::vec3();

	for (int i = 0; i < m_vertexCount; ++i)
	{
		glm::vec3 c = m_posPtr[i];
		if (
			c.x < m_globalBounds->min.x ||
			c.y < m_globalBounds->min.y ||
			c.z < m_globalBounds->min.z
			)
		{
			m_globalBounds->min = c;
		}

		if (
			c.x > m_globalBounds->max.x ||
			c.y > m_globalBounds->max.y ||
			c.z > m_globalBounds->max.z
			)
		{
			m_globalBounds->max = c;
		}
	}
	m_globalBounds->min -= glm::vec3(CELL_OFFSET, CELL_OFFSET, CELL_OFFSET);
	m_globalBounds->max += glm::vec3(CELL_OFFSET, CELL_OFFSET, CELL_OFFSET);

	// Calculate forces
	cudaError_t cudaStatus = CalculateForces(gravity, delta, steps, boxColliders, sphereColliders, transform);
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: AddWithCuda failed!");
		return CS_ERR_CLOTHSIMULATOR_CUDA_FAILED;
	}

	/*
	for (int i = 0; i < 4; ++i)
	{
		printf("%f %f %f | %f %f %f || ", m_vertices[i].force.x, m_vertices[i].force.y, m_vertices[i].force.z, 
			m_vertices[i].velocity.x, m_vertices[i].velocity.y, m_vertices[i].velocity.z);
	}
	printf("\n");
	*/
	//printf("%f %f %f \n", m_vertices[59].velocity.x, m_vertices[59].velocity.y, m_vertices[59].velocity.z);
	

	return CS_ERR_NONE;
}

unsigned int clothSpringSimulation::ClothSpringSimulationShutdown()
{
	cudaError_t cudaStatus;

	delete m_deviceProperties;
	delete m_vertices;
	delete m_globalBounds;

	FreeMemory();

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		printf("CUDA: cudaDeviceReset failed!");
		return 1;
	}
}


/////////////////////////////////////////////////////


/////////////////////////////////////////////////////


inline cudaError_t clothSpringSimulation::CalculateForces(float gravity, double delta, int steps, 
	BoxAAData* boxColliders, SphereData* sphereColliders, glm::mat4* transform)
{
	cudaError_t status;

	// copy vertex data and simulation variables to device memory

	status = cudaMemcpy(i_vertexPtr, m_vertices, m_vertexCount * sizeof(Vertex), cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	/*
	status = cudaMemcpy(i_springPtr, m_springs, m_springCount * sizeof(Spring), cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}
	*/

	status = cudaMemcpy(i_posPtr, m_posPtr, m_vertexCount * m_vertexPositionSize, cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	status = cudaMemcpy(i_nrmPtr, m_nrmPtr, m_vertexCount * m_vertexNormalSize, cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	status = cudaMemcpy(i_colPtr, m_colPtr, m_vertexCount * m_vertexColorSize, cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	// copy colliders

	status = cudaMemcpy(i_bcldPtr, boxColliders, m_boxColliderCount * sizeof(BoxAACollider), cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	status = cudaMemcpy(i_scldPtr, sphereColliders, m_sphereColliderCount * sizeof(SphereCollider), cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	// and world matrix -_-
	status = cudaMemcpy(i_wmPtr, transform, sizeof(glm::mat4), cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}
	///////////////


	// launch kernel
	int p = m_deviceProperties->warpSize;
	int sX = (m_allEdgesWidth - 1) * m_allEdgesLength;
	int sY = (m_allEdgesLength - 1) * m_allEdgesWidth;
	dim3 gridVerts((m_allEdgesWidth + p - 1) / p, (m_allEdgesLength + p - 1) / p, 1);
	//dim3 gridSprings((sX + p - 1) / p, (sY + p - 1) / p, 1);
	dim3 blockVerts(p, p, 1);
	//dim3 blockSprings(p, p, 1);

	double nDelta = delta / 25000.0;
	
	for (int i = 1; i <= steps; ++i)
	{
		CalculateForcesKernel << < gridVerts, blockVerts >> > (i_vertexPtr, i_posPtr, i_nrmPtr, i_colPtr, gravity, nDelta / steps, m_vertexCount);
		CalculateExternalCollisionsKernel << < gridVerts, blockVerts >> > (i_vertexPtr, i_posPtr, i_bcldPtr, i_scldPtr, 
			i_wmPtr, m_boxColliderCount, m_sphereColliderCount, m_vertexCount);
		//CalculateInternalCollisionsSimpleKernel << < gridVerts, blockVerts >> > (i_vertexPtr, i_posPtr, m_vertexCount);
		//CalculateInternalCollisionsSpatialSubdivisionKernel << < gridVerts, blockVerts >> > (i_vertexPtr, i_posPtr, *m_globalBounds, m_cellSize, m_vertexCount);
		CalculateInternalCollisionsNeighboursOnlyKernel << < gridVerts, blockVerts >> > (i_vertexPtr, i_posPtr, m_allEdgesLength, m_vertexCount);
		CalculateNormalsKernel << < gridVerts, blockVerts >> > (i_vertexPtr, i_posPtr, i_nrmPtr, m_vertexCount);
	}
	lastDelta = delta;

	// Check for any errors launching the kernel
	status = cudaGetLastError();
	if (status != cudaSuccess) {
		fprintf(stderr, "AddKernel launch failed: %s\n", cudaGetErrorString(status));
		FreeMemory();
		return status;
	}

	// copy calculated data out of device memory


	status = cudaMemcpy(m_vertices, i_vertexPtr, m_vertexCount * sizeof(Vertex), cudaMemcpyDeviceToHost);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	/*
	status = cudaMemcpy(m_springs, i_springPtr, m_springCount * sizeof(Spring), cudaMemcpyDeviceToHost);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}
	*/

	status = cudaMemcpy(m_posPtr, i_posPtr, m_vertexCount * m_vertexPositionSize, cudaMemcpyDeviceToHost);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	status = cudaMemcpy(m_nrmPtr, i_nrmPtr, m_vertexCount * m_vertexNormalSize, cudaMemcpyDeviceToHost);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	status = cudaMemcpy(m_colPtr, i_colPtr, m_vertexCount * m_vertexColorSize, cudaMemcpyDeviceToHost);
	if (status != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		FreeMemory();
		return status;
	}

	return status;
}


void clothSpringSimulation::FreeMemory()
{
	cudaFree(i_vertexPtr);
	cudaFree(i_springPtr);
	cudaFree(i_posPtr);
	cudaFree(i_nrmPtr);
	cudaFree(i_colPtr);
	cudaFree(i_bcldPtr);
	cudaFree(i_scldPtr);
}