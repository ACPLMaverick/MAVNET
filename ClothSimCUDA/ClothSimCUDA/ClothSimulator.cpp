#include "ClothSimulator.h"


ClothSimulator::ClothSimulator(SimObject* obj, int steps) : Component(obj)
{
	m_steps = steps;
}

ClothSimulator::ClothSimulator(const ClothSimulator* c) : Component(c)
{

}

ClothSimulator::~ClothSimulator()
{
}



unsigned int ClothSimulator::Initialize()
{
	unsigned int err = CS_ERR_NONE;

	// acquiring mesh and collider

	if (
		m_obj->GetMesh(0) != nullptr
		)
	{
		m_meshPlane = (MeshGLPlane*)m_obj->GetMesh(0);
	}
	else return CS_ERR_CLOTHSIMULATOR_MESH_OBTAINING_ERROR;

	// initializing CUDA
	m_simulator = new clothSpringSimulation();
	VertexData* clothData = m_meshPlane->GetVertexDataPtr();
	err = m_simulator->ClothSpringSimulationInitialize(
		sizeof(clothData->data->positionBuffer[0]), 
		sizeof(clothData->data->normalBuffer[0]),
		sizeof(clothData->data->colorBuffer[0]),
		m_meshPlane->GetEdgesWidth() + 2,
		m_meshPlane->GetEdgesLength() + 2,
		PhysicsManager::GetInstance()->GetBoxCollidersData()->size(),
		PhysicsManager::GetInstance()->GetSphereCollidersData()->size(),
		clothData->data->positionBuffer,
		clothData->data->normalBuffer,
		clothData->data->colorBuffer
		);

	return err;
}

unsigned int ClothSimulator::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	err = m_simulator->ClothSpringSimulationShutdown();
	delete m_simulator;

	return err;
}



unsigned int ClothSimulator::Update()
{
	unsigned int err = CS_ERR_NONE;

	VertexData* clothData = m_meshPlane->GetVertexDataPtr();
	BoxAAData* boxData = nullptr;
	SphereData* sphereData = nullptr;
	if (PhysicsManager::GetInstance()->GetBoxCollidersData()->size() != 0)
		boxData = &(PhysicsManager::GetInstance()->GetBoxCollidersData()->at(0));
	if (PhysicsManager::GetInstance()->GetSphereCollidersData()->size() != 0)
		sphereData = &(PhysicsManager::GetInstance()->GetSphereCollidersData()->at(0));

	glm::mat4* wm = &glm::mat4();

	if (m_obj->GetTransform() != nullptr)
		wm = m_obj->GetTransform()->GetWorldMatrix();

	err = m_simulator->ClothSpringSimulationUpdate(
		PhysicsManager::GetInstance()->GetGravity(),
		Timer::GetInstance()->GetFixedDeltaTime(),
		m_steps,
		boxData,
		sphereData,
		wm
		);

	return err;
}
unsigned int ClothSimulator::Draw()
{
	unsigned int err = CS_ERR_NONE;

	return err;
}