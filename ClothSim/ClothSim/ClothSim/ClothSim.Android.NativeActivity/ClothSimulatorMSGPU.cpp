#include "pch.h"
#include "ClothSimulatorMSGPU.h"


ClothSimulatorMSGPU::ClothSimulatorMSGPU(SimObject * obj) : ClothSimulator(obj)
{
}

ClothSimulatorMSGPU::ClothSimulatorMSGPU(const ClothSimulatorMSGPU * c) : ClothSimulator(c)
{
}

ClothSimulatorMSGPU::~ClothSimulatorMSGPU()
{
}



inline unsigned int ClothSimulatorMSGPU::InitializeSim()
{
	unsigned int err = CS_ERR_NONE;

	m_kernelID = ResourceManager::GetInstance()->LoadKernel(&KERNEL_SIM_NAME);
	glGenTransformFeedbacks(1, &m_ktfID);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_ktfID);
	glTransformFeedbackVaryings(m_kernelID->id, KERNEL_SIM_OUTPUT_NAME_COUNT, KERNEL_SIM_OUTPUT_NAMES, GL_SEPARATE_ATTRIBS);
	glLinkProgram(m_kernelID->id);

	for (uint i = 0; i < 2; ++i)
	{
		m_texPosID[i] = glGetUniformBlockIndex(m_kernelID->id, KERNEL_SIM_INPUT_NAMES[0]);
		m_texPosLastID[i] = glGetUniformBlockIndex(m_kernelID->id, KERNEL_SIM_INPUT_NAMES[1]);
		glUniformBlockBinding(m_kernelID->id, m_texPosID[i], 1);
		glUniformBlockBinding(m_kernelID->id, m_texPosID[i], 0);
	}

	m_kernelID->uniformIDs->push_back(glGetUniformLocation(m_kernelID->id, "VertexCount"));
	m_kernelID->uniformIDs->push_back(glGetUniformLocation(m_kernelID->id, "EdgesWidthAll"));
	m_kernelID->uniformIDs->push_back(glGetUniformLocation(m_kernelID->id, "EdgesLengthAll"));
	m_kernelID->uniformIDs->push_back(glGetUniformLocation(m_kernelID->id, "DeltaTime"));
	m_kernelID->uniformIDs->push_back(glGetUniformLocation(m_kernelID->id, "Gravity"));

	return err;
}

inline unsigned int ClothSimulatorMSGPU::ShutdownSim()
{
	unsigned int err = CS_ERR_NONE;

	// shutdown kernels 

	// shutdown method-related data

	return err;
}

inline unsigned int ClothSimulatorMSGPU::UpdateSim(float gravity, float fixedDelta)
{
	unsigned int err = CS_ERR_NONE;

	glUseProgram(m_kernelID->id);

	glUniform1i(m_kernelID->uniformIDs->at(0), m_simData->m_vertexCount);
	glUniform1i(m_kernelID->uniformIDs->at(1), m_simData->m_edgesWidthAll);
	glUniform1i(m_kernelID->uniformIDs->at(2), m_simData->m_edgesLengthAll);
	glUniform1f(m_kernelID->uniformIDs->at(3), fixedDelta);
	glUniform1f(m_kernelID->uniformIDs->at(4), gravity);

	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosID[m_readID], m_vboPosID[m_readID]);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosLastID[m_readID], m_vboPosLastID[m_readID]);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_ktfID);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vboPosID[m_writeID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vboPosLastID[m_writeID]);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_simData->m_vertexCount);
	glEndTransformFeedback();

	//glFlush();
	glDisable(GL_RASTERIZER_DISCARD);

	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosID[m_readID], 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosLastID[m_readID], 0);

	return err;
}