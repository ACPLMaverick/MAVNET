#include "ClothSimulator.h"

ClothSimulator::ClothSimulator(SimObject* obj) : Component(obj)
{
	// here go the "constants"
	KERNEL_NRM_INPUT_NAMES[0] = "InPos";
	KERNEL_NRM_OUTPUT_NAMES[0] = "OutNormal";
	KERNEL_COL_INPUT_NAMES[0] = "InPos";
	KERNEL_COL_INPUT_NAMES[1] = "InBaaCols";
	KERNEL_COL_INPUT_NAMES[2] = "InSCols";
	KERNEL_COL_OUTPUT_NAMES[0] = "OutPos";
	KERNEL_COL_OUTPUT_NAMES[1] = "OutPosLast";
	KERNEL_MSPOS_INPUT_NAMES[0] = "InPos";
	KERNEL_MSPOS_INPUT_NAMES[1] = "InPosLast";
	KERNEL_MSPOS_OUTPUT_NAMES[0] = "OutPos";
	KERNEL_MSPOS_OUTPUT_NAMES[1] = "OutPosLast";
	KERNEL_PBPOS_INPUT_NAMES[0] = "InPos";
	KERNEL_PBPOS_INPUT_NAMES[1] = "InPosLast";
	KERNEL_PBPOS_OUTPUT_NAMES[0] = "OutPos";
	KERNEL_PBPOS_OUTPUT_NAMES[1] = "OutPosLast";
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

	// acquiring mesh

	glm::vec4 tCol = glm::vec4(1.0f, 0.5f, 0.7f, 1.0f);
	m_meshPlane = new MeshGLPlane(m_obj, m_simParams.width, m_simParams.length, m_simParams.edgesWidth, m_simParams.edgesLength, &tCol);
	m_meshPlane->Initialize();
	m_meshPlane->SetGloss(10.0f);
	m_meshPlane->SetSpecular(0.2f);
	m_meshPlane->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	m_obj->AddMesh(m_meshPlane);

	// get a copy of start data
	m_vd = m_meshPlane->GetVertexDataDualPtr();
	m_vdCopy = new VertexData*[2];
	m_vdCopy[0] = new VertexData;
	m_vdCopy[1] = new VertexData;
	CopyVertexData(m_vd[0], m_vdCopy[0]);
	CopyVertexData(m_vd[1], m_vdCopy[1]);

	// initialize simulation data

	m_simData.m_vertexCount = m_vd[0]->data->vertexCount;
	m_simData.m_edgesWidthAll = m_meshPlane->GetEdgesWidth() + 2;
	m_simData.m_edgesLengthAll = m_meshPlane->GetEdgesLength() + 2;

	m_simData.b_positionLast = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_neighbours = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_neighboursDiag = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_neighbours2 = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_neighbourMultipliers = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_neighbourDiagMultipliers = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_neighbour2Multipliers = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_elMassCoeffs = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_multipliers = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_sLengths = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_sLengthsDiag = new glm::vec4[m_simData.m_vertexCount];
	m_simData.b_sLengths2 = new glm::vec4[m_simData.m_vertexCount];

	glm::vec3 baseLength = glm::vec3(
		glm::length(m_vd[0]->data->positionBuffer[0] - m_vd[0]->data->positionBuffer[m_simData.m_edgesLengthAll]),
		0.0f,
		glm::length(m_vd[0]->data->positionBuffer[0] - m_vd[0]->data->positionBuffer[1])
		);

	m_cellSize = glm::max(baseLength.x, baseLength.z) * 1.5f;

	for (int i = 0; i < m_simData.m_vertexCount; ++i)
	{
		///
		m_simData.b_neighboursDiag[i] = glm::vec4(0.0f);
		m_simData.b_neighbours2[i] = glm::vec4(0.0f);
		m_simData.b_neighbourDiagMultipliers[i] = glm::vec4(0.0f);
		m_simData.b_neighbour2Multipliers[i] = glm::vec4(0.0f);
		///
		m_simData.b_positionLast[i] = m_vd[0]->data->positionBuffer[i];
		m_simData.b_elMassCoeffs[i].y = m_simParams.vertexMass / (sqrtf(glm::min((float)m_simData.m_vertexCount, MIN_MASS)));		// mass
		m_simData.b_elMassCoeffs[i].w = m_simParams.vertexAirDamp;
		m_simData.b_multipliers[i].x = 1.0f;

		m_simData.b_multipliers[i].y = glm::min(glm::min(baseLength.x, baseLength.z) / 2.0f, VERTEX_COLLIDER_MULTIPLIER);

		m_simData.b_elMassCoeffs[i].x = m_simParams.elasticity;						// elasticity
		m_simData.b_elMassCoeffs[i].z = m_simParams.elasticityDamp;	// el damp

		// calculating neighbouring vertices ids and spring lengths

		// upper
		int id = (i - 1) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours[i][0] = (float)id;
		m_simData.b_sLengths[i][0] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i % m_simData.m_edgesLengthAll)
		{
			m_simData.b_neighbourMultipliers[i][0] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourMultipliers[i][0] = 0.0f;
		}

		// left
		id = (i - m_simData.m_edgesLengthAll) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours[i][1] = id;
		m_simData.b_sLengths[i][1] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i >= m_simData.m_edgesLengthAll)
		{
			m_simData.b_neighbourMultipliers[i][1] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourMultipliers[i][1] = 0.0f;
		}

		// lower
		id = (i + 1) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours[i][2] = id;
		m_simData.b_sLengths[i][2] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i % m_simData.m_edgesLengthAll != (m_simData.m_edgesLengthAll - 1))
		{
			m_simData.b_neighbourMultipliers[i][2] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourMultipliers[i][2] = 0.0f;
		}

		// right
		id = (i + m_simData.m_edgesLengthAll) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours[i][3] = id;
		m_simData.b_sLengths[i][3] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i < (m_simData.m_vertexCount - m_simData.m_edgesLengthAll))
		{
			m_simData.b_neighbourMultipliers[i][3] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourMultipliers[i][3] = 0.0f;
		}

		// tl
		id = (i - m_simData.m_edgesLengthAll - 1) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighboursDiag[i][0] = id;
		m_simData.b_sLengthsDiag[i][0] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i >= m_simData.m_edgesLengthAll && i % m_simData.m_edgesLengthAll)
		{
			m_simData.b_neighbourDiagMultipliers[i][0] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourDiagMultipliers[i][0] = 0.0f;
		}

		// bl
		id = (i - m_simData.m_edgesLengthAll + 1) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighboursDiag[i][1] = id;
		m_simData.b_sLengthsDiag[i][1] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i >= m_simData.m_edgesLengthAll && (i % m_simData.m_edgesLengthAll != (m_simData.m_edgesLengthAll - 1)))
		{
			m_simData.b_neighbourDiagMultipliers[i][1] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourDiagMultipliers[i][1] = 0.0f;
		}

		// br
		id = (i + m_simData.m_edgesLengthAll + 1) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighboursDiag[i][2] = id;
		m_simData.b_sLengthsDiag[i][2] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i < (m_simData.m_vertexCount - m_simData.m_edgesLengthAll) && (i % m_simData.m_edgesLengthAll != (m_simData.m_edgesLengthAll - 1)))
		{
			m_simData.b_neighbourDiagMultipliers[i][2] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourDiagMultipliers[i][2] = 0.0f;
		}

		// tr
		id = (i + m_simData.m_edgesLengthAll - 1) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighboursDiag[i][3] = id;
		m_simData.b_sLengthsDiag[i][3] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i < (m_simData.m_vertexCount - m_simData.m_edgesLengthAll) && i % m_simData.m_edgesLengthAll)
		{
			m_simData.b_neighbourDiagMultipliers[i][3] = 1.0f;
		}
		else
		{
			m_simData.b_neighbourDiagMultipliers[i][3] = 0.0f;
		}

		// upper2
		id = (i - 2) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours2[i][0] = id;
		m_simData.b_sLengths2[i][0] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i % m_simData.m_edgesLengthAll > 1)
		{
			m_simData.b_neighbour2Multipliers[i][0] = 1.0f;
		}
		else
		{
			m_simData.b_neighbour2Multipliers[i][0] = 0.0f;
		}

		// left2
		id = (i - 2 * m_simData.m_edgesLengthAll) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours2[i][1] = id;
		m_simData.b_sLengths2[i][1] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i >= 2 * m_simData.m_edgesLengthAll)
		{
			m_simData.b_neighbour2Multipliers[i][1] = 1.0f;
		}
		else
		{
			m_simData.b_neighbour2Multipliers[i][1] = 0.0f;
		}

		// lower2
		id = (i + 2) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours2[i][2] = id;
		m_simData.b_sLengths2[i][2] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i % m_simData.m_edgesLengthAll < (m_simData.m_edgesLengthAll - 2))
		{
			m_simData.b_neighbour2Multipliers[i][2] = 1.0f;
		}
		else
		{
			m_simData.b_neighbour2Multipliers[i][2] = 0.0f;
		}

		// right2
		id = (i + 2 * m_simData.m_edgesLengthAll) % m_simData.m_vertexCount;
		if (id < 0 || id >= m_simData.m_vertexCount)
			id = ((i + 1) % m_simData.m_vertexCount);
		m_simData.b_neighbours2[i][3] = id;
		m_simData.b_sLengths2[i][3] = glm::length(m_vd[0]->data->positionBuffer[i] - m_vd[0]->data->positionBuffer[id]);
		if (i < (m_simData.m_vertexCount -  2 * m_simData.m_edgesLengthAll))
		{
			m_simData.b_neighbour2Multipliers[i][3] = 1.0f;
		}
		else
		{
			m_simData.b_neighbour2Multipliers[i][3] = 0.0f;
		}

		/*
		if (i == 0)
		{
			LOGI("%f %f %f %f", m_simData.b_neighbours[i][0], m_simData.b_neighbours[i][1], m_simData.b_neighbours[i][2], m_simData.b_neighbours[i][3]);
			LOGI("%f %f %f %f", m_simData.b_neighboursDiag[i][0], m_simData.b_neighboursDiag[i][1], m_simData.b_neighboursDiag[i][2], m_simData.b_neighboursDiag[i][3]);
			LOGI("%f %f %f %f", m_simData.b_neighbours2[i][0], m_simData.b_neighbours2[i][1], m_simData.b_neighbours2[i][2], m_simData.b_neighbours2[i][3]);
			LOGI("%f %f %f %f", m_simData.b_neighbourMultipliers[i][0], m_simData.b_neighbourMultipliers[i][1], m_simData.b_neighbourMultipliers[i][2], m_simData.b_neighbourMultipliers[i][3]);
			LOGI("%f %f %f %f", m_simData.b_neighbourDiagMultipliers[i][0], m_simData.b_neighbourDiagMultipliers[i][1], m_simData.b_neighbourDiagMultipliers[i][2], m_simData.b_neighbourDiagMultipliers[i][3]);
			LOGI("%f %f %f %f", m_simData.b_neighbour2Multipliers[i][0], m_simData.b_neighbour2Multipliers[i][1], m_simData.b_neighbour2Multipliers[i][2], m_simData.b_neighbour2Multipliers[i][3]);
		}
		*/
	}

	// hard-coded locks
	m_simData.b_multipliers[0][0] = 0.0f;
	m_simData.b_multipliers[(m_simData.m_vertexCount - m_simData.m_edgesLengthAll)][0] = 0.0f;

	m_readID = 0;
	m_writeID = 1;

	///////////////////////////////////////////////////////////////////////////////////////////////////

	// OpenGL Data initialization and binding

	if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_GPU || m_simParams.mode == ClothSimulationMode::POSITION_BASED_GPU)
	{
		m_vaoRenderID[0] = m_vd[0]->ids->vertexArrayID;
		m_vaoRenderID[1] = m_vd[1]->ids->vertexArrayID;
		glGenVertexArrays(2, m_vaoUpdateID);
		m_vboPosID[0] = m_vd[0]->ids->vertexBuffer;
		m_vboPosID[1] = m_vd[1]->ids->vertexBuffer;
		glGenBuffers(2, m_vboPosLastID);
		m_vboNrmID[0] = m_vd[0]->ids->normalBuffer;
		m_vboNrmID[1] = m_vd[1]->ids->normalBuffer;

		for (uint i = 0; i < 2; ++i)
		{
			glBindVertexArray(m_vaoUpdateID[i]);
			glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, m_vboPosID[i]);
			glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
				m_vd[0]->data->vertexCount * sizeof(m_vd[0]->data->positionBuffer[0]),
				&m_vd[0]->data->positionBuffer[0].x,
				GL_DYNAMIC_COPY);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, m_vboPosLastID[i]);
			glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
				m_simData.m_vertexCount * sizeof(m_simData.b_positionLast[0]),
				&m_simData.b_positionLast[0].x,
				GL_DYNAMIC_COPY);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, m_vboNrmID[i]);
			glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
				m_vd[0]->data->vertexCount * sizeof(m_vd[0]->data->normalBuffer[0]),
				m_vd[0]->data->normalBuffer,
				GL_DYNAMIC_COPY);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glGenBuffers(1, &m_simData.i_neighbours);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbours);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_neighbours[0]),
			m_simData.b_neighbours,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_neighboursDiag);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighboursDiag);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_neighboursDiag[0]),
			m_simData.b_neighboursDiag,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_neighbours2);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbours2);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_neighbours2[0]),
			m_simData.b_neighbours2,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_neighbourMultipliers);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbourMultipliers);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_neighbourMultipliers[0]),
			m_simData.b_neighbourMultipliers,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_neighbourDiagMultipliers);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbourDiagMultipliers);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_neighbourDiagMultipliers[0]),
			m_simData.b_neighbourDiagMultipliers,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_neighbour2Multipliers);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbour2Multipliers);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_neighbour2Multipliers[0]),
			m_simData.b_neighbour2Multipliers,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_sLengths);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_sLengths);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_sLengths[0]),
			m_simData.b_sLengths,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_sLengthsDiag);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_sLengthsDiag);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_sLengthsDiag[0]),
			m_simData.b_sLengthsDiag,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(10);
		glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_sLengths2);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_sLengths2);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_sLengths2[0]),
			m_simData.b_sLengths2,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(11);
		glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_elMassCoeffs);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_elMassCoeffs);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_elMassCoeffs[0]),
			m_simData.b_elMassCoeffs,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(12);
		glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &m_simData.i_multipliers);
		glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_multipliers);
		glBufferData(GL_ARRAY_BUFFER,
			m_simData.m_vertexCount * sizeof(m_simData.b_multipliers[0]),
			m_simData.b_multipliers,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(13);
		glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// Other kernel loading and feedbacks initialization goes here
		m_collisionsKernel = ResourceManager::GetInstance()->LoadKernel(&KERNEL_COL_NAME);

		glGenTransformFeedbacks(1, &m_ctfID);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_ctfID);
		glTransformFeedbackVaryings(m_collisionsKernel->id, KERNEL_COL_OUTPUT_NAME_COUNT, KERNEL_COL_OUTPUT_NAMES, GL_SEPARATE_ATTRIBS);
		glLinkProgram(m_collisionsKernel->id);

#ifndef PLATFORM_WINDOWS
		glGenBuffers(1, &m_vboColBAAID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_vboColBAAID);
		glGenBuffers(1, &m_vboColSID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_vboColSID);

		for (uint i = 0; i < 2; ++i)
		{
			m_texColPosID[i] = glGetUniformBlockIndex(m_collisionsKernel->id, KERNEL_COL_INPUT_NAMES[0]);
			glUniformBlockBinding(m_collisionsKernel->id, m_texColPosID[i], 1);
		}
		m_texColBAAID = glGetUniformBlockIndex(m_collisionsKernel->id, KERNEL_COL_INPUT_NAMES[1]);
		m_texColSID = glGetUniformBlockIndex(m_collisionsKernel->id, KERNEL_COL_INPUT_NAMES[2]);
		glUniformBlockBinding(m_collisionsKernel->id, m_texColBAAID, 0);
		glUniformBlockBinding(m_collisionsKernel->id, m_texColSID, 2);
#else
		glGenBuffers(1, &m_vboColBAAID);
		glGenBuffers(1, &m_vboColSID);
		for (uint i = 0; i < 2; ++i)
		{
			glGenTextures(1, &m_texColPosID[i]);
			glBindTexture(GL_TEXTURE_BUFFER, m_texColPosID[i]);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboPosID[i]);
		}
		glGenTextures(1, &m_texColBAAID);
		glBindTexture(GL_TEXTURE_BUFFER, m_texColBAAID);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboColBAAID);
		glGenTextures(1, &m_texColSID);
		glBindTexture(GL_TEXTURE_BUFFER, m_texColSID);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboColSID);
#endif

		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "WorldMatrix"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "ViewMatrix"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "ProjMatrix"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "TouchVector"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "GroundLevel"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "BoxAAColliderCount"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "SphereColliderCount"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "EdgesWidthAll"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "EdgesLengthAll"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "VertexCount"));
		m_collisionsKernel->uniformIDs->push_back(glGetUniformLocation(m_collisionsKernel->id, "InternalCollisionCheckWindowSize"));

		////////////

		m_normalsKernel = ResourceManager::GetInstance()->LoadKernel(&KERNEL_NRM_NAME);

		glGenTransformFeedbacks(1, &m_ntfID);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_ntfID);
		glTransformFeedbackVaryings(m_normalsKernel->id, KERNEL_NRM_OUTPUT_NAME_COUNT, KERNEL_NRM_OUTPUT_NAMES, GL_SEPARATE_ATTRIBS);
		glLinkProgram(m_normalsKernel->id);

#ifndef PLATFORM_WINDOWS
		for (uint i = 0; i < 2; ++i)
		{
			m_texNrmPosID[i] = glGetUniformBlockIndex(m_normalsKernel->id, KERNEL_NRM_INPUT_NAMES[0]);
			glUniformBlockBinding(m_normalsKernel->id, m_texNrmPosID[i], 0);
		}
#else
		for (uint i = 0; i < 2; ++i)
		{
			glGenTextures(1, &m_texNrmPosID[i]);
			glBindTexture(GL_TEXTURE_BUFFER, m_texNrmPosID[i]);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboPosID[i]);
		}
#endif

		// Sim-specific initialization. Sim-related transform feedback initialization and kernel loading goes here.
		if(m_simParams.mode == ClothSimulationMode::MASS_SPRING_GPU)
			err = InitializeSimMSGPU();
		else
			err = InitializeSimPBGPU();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
		glBindVertexArray(0);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	}
	

	if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPU || m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPU ||
		m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPUx4 || m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPUx4)
	{
		m_meshPlane->SwapDataPtrs();
	}

	if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPUx4 || m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPUx4)
	{
		// sync structures
		m_threadsRunning = true;
		m_barrierCounter = 0;
		m_mainThreadID = pthread_self();

		m_mutexBarrierCounter = new pthread_mutex_t;
		m_mutexHold = new pthread_mutex_t;
		m_mutexHoldStage2 = new pthread_mutex_t;
		m_mutexHoldStage3 = new pthread_mutex_t;
		m_mutexThreadsRunning = new pthread_mutex_t;

		pthread_mutexattr_t mutexAttrs;
		pthread_mutexattr_init(&mutexAttrs);
		pthread_mutexattr_settype(&mutexAttrs, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(m_mutexThreadsRunning, &mutexAttrs);
		pthread_mutex_init(m_mutexHold, &mutexAttrs);
		pthread_mutex_init(m_mutexHoldStage2, &mutexAttrs);
		pthread_mutex_init(m_mutexHoldStage3, &mutexAttrs);
		pthread_mutex_init(m_mutexBarrierCounter, &mutexAttrs);

		pthread_mutexattr_destroy(&mutexAttrs);

		pthread_mutex_lock(m_mutexHold);
		pthread_mutex_lock(m_mutexHoldStage2);
		pthread_mutex_lock(m_mutexHoldStage3);

		// thread initialization and suspend until recieving signal from main thread
		int vertexCountTo4 = m_simData.m_vertexCount;
		while (vertexCountTo4 < INT32_MAX)
		{
			if (vertexCountTo4 % THREAD_COUNT == 0)
				break;

			++vertexCountTo4;
		}

		pthread_attr_t threadsAttr;
		pthread_attr_init(&threadsAttr);
		pthread_attr_setdetachstate(&threadsAttr, PTHREAD_CREATE_JOINABLE);

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			m_threadData[i].inst = this;
			m_threadData[i].id = i;
			m_threadData[i].diBegin = (vertexCountTo4 / THREAD_COUNT) *  i;			// 0, 2, 4, 6
			if (m_threadData[i].diBegin > m_simData.m_vertexCount)
				m_threadData[i].diBegin = m_simData.m_vertexCount;
			m_threadData[i].diEnd = (vertexCountTo4 / THREAD_COUNT) * (i + 1);		// 2, 4, 6, 8
			if (m_threadData[i].diEnd > m_simData.m_vertexCount)
				m_threadData[i].diEnd = m_simData.m_vertexCount;

			LOGI("%d: %d - %d", i, m_threadData[i].diBegin, m_threadData[i].diEnd);

			pthread_create(&m_threadIDs[i], &threadsAttr, ClothSimulator::UpdatePartial, (void*)&m_threadData[i]);
		}

		pthread_attr_destroy(&threadsAttr);
		
	}

	return err;
}

unsigned int ClothSimulator::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	// Sim-specific shutdown
	if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_GPU || m_simParams.mode == ClothSimulationMode::POSITION_BASED_GPU)
	{
		if(m_simParams.mode == ClothSimulationMode::MASS_SPRING_GPU)
			err = ShutdownSimMSGPU();
		else
			err = ShutdownSimPBGPU();

#ifndef PLATFORM_WINDOWS
		glDeleteBuffers(1, &m_vboColBAAID);
		glDeleteBuffers(1, &m_vboColSID);
#else
		glDeleteTextures(1, &m_texColBAAID);
		glDeleteTextures(1, &m_texColSID);
		glDeleteTextures(2, m_texColPosID);
		glDeleteTextures(2, m_texNrmPosID);
#endif

		glDeleteVertexArrays(2, m_vaoUpdateID);
		glDeleteBuffers(2, m_vboPosLastID);

		glDeleteBuffers(1, &m_simData.i_neighbours);
		glDeleteBuffers(1, &m_simData.i_neighboursDiag);
		glDeleteBuffers(1, &m_simData.i_neighbours2);
		glDeleteBuffers(1, &m_simData.i_neighbourMultipliers);
		glDeleteBuffers(1, &m_simData.i_neighbourDiagMultipliers);
		glDeleteBuffers(1, &m_simData.i_neighbour2Multipliers);
		glDeleteBuffers(1, &m_simData.i_sLengths);
		glDeleteBuffers(1, &m_simData.i_sLengthsDiag);
		glDeleteBuffers(1, &m_simData.i_sLengths2);
		glDeleteBuffers(1, &m_simData.i_elMassCoeffs);
		glDeleteBuffers(1, &m_simData.i_multipliers);

		glDeleteTransformFeedbacks(1, &m_ntfID);
		glDeleteTransformFeedbacks(1, &m_ctfID);
	}
	else if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPUx4 || m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPUx4)
	{
		// here we will have thread shutdown
		m_threadsRunning = false;
		pthread_mutex_unlock(m_mutexHold);
		pthread_mutex_unlock(m_mutexHoldStage2);
		pthread_mutex_unlock(m_mutexHoldStage3);
		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			pthread_join(m_threadIDs[i], NULL);
		}
		pthread_mutex_destroy(m_mutexBarrierCounter);
		pthread_mutex_destroy(m_mutexHold);
		pthread_mutex_destroy(m_mutexHoldStage2);
		pthread_mutex_destroy(m_mutexHoldStage3);
		pthread_mutex_destroy(m_mutexThreadsRunning);
		delete m_mutexBarrierCounter;
		delete m_mutexHold;
		delete m_mutexHoldStage2;
		delete m_mutexHoldStage3;
		delete m_mutexThreadsRunning;
	}

	m_meshPlane->Shutdown();
	m_obj->RemoveMesh(m_meshPlane);
	delete m_meshPlane;
	
	delete m_vdCopy[0];
	delete m_vdCopy[1];
	delete[] m_vdCopy;


	return err;
}



unsigned int ClothSimulator::Update()
{
	unsigned int err = CS_ERR_NONE;

	if (m_ifRestart)
	{
		RestartSimulation();
		m_ifRestart = false;
	}

	/////////////////////////
	m_timeStartMS = Timer::GetInstance()->GetCurrentTimeMS();
	/////////////////////////

	VertexData* clothData = m_meshPlane->GetVertexDataPtr();
	BoxAAData* boxData = nullptr;
	SphereData* sphereData = nullptr;
	if (PhysicsManager::GetInstance()->GetBoxCollidersData()->size() != 0)
		boxData = &(PhysicsManager::GetInstance()->GetBoxCollidersData()->at(0));
	if (PhysicsManager::GetInstance()->GetSphereCollidersData()->size() != 0)
		sphereData = &(PhysicsManager::GetInstance()->GetSphereCollidersData()->at(0));
	int bcCount = PhysicsManager::GetInstance()->GetBoxCollidersData()->size();
	int scCount = PhysicsManager::GetInstance()->GetSphereCollidersData()->size();
	float groundLevel = System::GetInstance()->GetCurrentScene()->GetGroundLevel();

	glm::mat4 zero = glm::mat4();
	glm::mat4* wm = &zero;

	if (m_obj->GetTransform() != nullptr)
		wm = m_obj->GetTransform()->GetWorldMatrix();

	glm::mat4* vw = System::GetInstance()->GetCurrentScene()->GetCamera()->GetViewMatrix();
	glm::mat4* pr = System::GetInstance()->GetCurrentScene()->GetCamera()->GetProjMatrix();

	if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPU || m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPU)
	{
		err = UpdateSimCPU(clothData, boxData, sphereData, bcCount, scCount, wm, vw, pr, PhysicsManager::GetInstance()->GetGravity(), Timer::GetInstance()->GetFixedDeltaTime(), groundLevel);
	}
	else if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPUx4 || m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPUx4)
	{
		err = UpdateSimCPUx4(clothData, boxData, sphereData, bcCount, scCount, wm, vw, pr, PhysicsManager::GetInstance()->GetGravity(), Timer::GetInstance()->GetFixedDeltaTime(), groundLevel);
	}
	else if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_GPU || m_simParams.mode == ClothSimulationMode::POSITION_BASED_GPU)
	{
		err = UpdateSimGPU(clothData, boxData, sphereData, bcCount, scCount, wm, vw, pr, PhysicsManager::GetInstance()->GetGravity(), Timer::GetInstance()->GetFixedDeltaTime(), groundLevel);
	}

	/////////////////////////
	double cTime = Timer::GetInstance()->GetCurrentTimeMS();
	m_timeSimMS = cTime - m_timeStartMS;
	/////////////////////////

	return err;
}
unsigned int ClothSimulator::Draw()
{
	unsigned int err = CS_ERR_NONE;

	return err;
}

void ClothSimulator::SetMode(ClothSimulationMode mode)
{
	if (mode != m_simParams.mode)
	{
		m_simParams.mode = mode;
		AppendRestart();
	}
}

void ClothSimulator::SwitchMode()
{
	m_simParams.mode = (ClothSimulationMode)(((int)m_simParams.mode + 1) % 2);
	AppendRestart();
}

void ClothSimulator::Restart()
{
	AppendRestart();
}

void ClothSimulator::Restart(SimParams * params)
{
	m_tempSimParamsPtr = params;
	Restart();
}

ClothSimulationMode ClothSimulator::GetMode()
{
	return m_simParams.mode;
}

double ClothSimulator::GetSimTimeMS()
{
	return m_timeSimMS;
}

float ClothSimulator::GetD()
{
	return m_d;
}

void ClothSimulator::UpdateSimParams(SimParams * params)
{
	m_simParams = *params;
}

void ClothSimulator::UpdateTouchVector(const glm::vec2 * pos, const glm::vec2 * dir)
{
	m_simData.c_touchVector.x = pos->x;
	m_simData.c_touchVector.y = pos->y;
	m_simData.c_touchVector.z = dir->x;
	m_simData.c_touchVector.w = dir->y;
}

SimParams * ClothSimulator::GetSimParams()
{
	return &m_simParams;
}

inline unsigned int ClothSimulator::UpdateSimCPU
	(
		VertexData* vertexData,
		BoxAAData* boxAAData,
		SphereData* sphereData,
		int bcCount,
		int scCount,
		glm::mat4* worldMatrix,
		glm::mat4* viewMatrix,
		glm::mat4* projMatrix,
		glm::vec3* gravity,
		float fixedDelta,
		float groundLevel
	)
{
	unsigned int err = CS_ERR_NONE;

	// execute simulation procedure
	if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPU)
	{
		err = UpdateSimMSCPU(
			PhysicsManager::GetInstance()->GetGravity(),
			fixedDelta
			);
	}
	else if (m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPU)
	{
		err = UpdateSimPBCPU(
			PhysicsManager::GetInstance()->GetGravity(),
			fixedDelta
			);
	}

	SwapRWIds();

	// getting the D

	int dID = m_simData.m_vertexCount / 2;
	glm::vec3 data = glm::vec3(m_vd[m_readID]->data->positionBuffer[dID]);
	m_d = glm::length(data - m_dLast);
	m_dLast = data;
	glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
	
	// compute collisions and finger movement
	for (int i = 0; i < m_simData.m_vertexCount; ++i)
	{
		CPUComputeCollision(i, worldMatrix, viewMatrix, projMatrix, boxAAData, sphereData, bcCount, scCount, groundLevel);
	}
	SwapRWIds();

	// compute normals
	for (int i = 0; i < m_simData.m_vertexCount; ++i)
	{
		CPUComputeNormal(i);
	}

	// update GPU buffers
	glBindVertexArray(m_vd[m_writeID]->ids->vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vd[m_writeID]->ids->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_simData.m_vertexCount * sizeof(m_vd[m_writeID]->data->positionBuffer[0]),
		m_vd[m_writeID]->data->positionBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vd[m_writeID]->ids->normalBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_simData.m_vertexCount * sizeof(m_vd[m_writeID]->data->normalBuffer[0]),
		m_vd[m_writeID]->data->normalBuffer);

	//SwapRWIds();
	//m_meshPlane->SwapDataPtrs();

	return err;
}

inline unsigned int ClothSimulator::UpdateSimCPUx4(VertexData * vertexData, BoxAAData * boxAAData, SphereData * sphereData, int bcCount, int scCount, 
	glm::mat4 * worldMatrix, glm::mat4 * viewMatrix, glm::mat4 * projMatrix, glm::vec3* gravity, float fixedDelta, float groundLevel)
{
	// one step of cloth simulation
	pthread_mutex_unlock(m_mutexHold);
#ifndef PLATFORM_WINDOWS
	while (m_barrierCounter < THREAD_COUNT) __asm__("nop");
#else
	while (m_barrierCounter < THREAD_COUNT) _asm nop;
#endif
	pthread_mutex_lock(m_mutexHold);
	m_barrierCounter = 0;
	SwapRWIds();

	pthread_mutex_unlock(m_mutexHoldStage2);
#ifndef PLATFORM_WINDOWS
	while (m_barrierCounter < THREAD_COUNT) __asm__("nop");
#else
	while (m_barrierCounter < THREAD_COUNT) _asm nop;
#endif
	pthread_mutex_lock(m_mutexHoldStage2);
	m_barrierCounter = 0;
	SwapRWIds();

	pthread_mutex_unlock(m_mutexHoldStage3);
#ifndef PLATFORM_WINDOWS
	while (m_barrierCounter < THREAD_COUNT) __asm__("nop");
#else
	while (m_barrierCounter < THREAD_COUNT) _asm nop;
#endif
	pthread_mutex_lock(m_mutexHoldStage3);
	m_barrierCounter = 0;

	// update GPU buffers
	glBindVertexArray(m_vd[m_writeID]->ids->vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vd[m_writeID]->ids->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_simData.m_vertexCount * sizeof(m_vd[m_writeID]->data->positionBuffer[0]),
		m_vd[m_writeID]->data->positionBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vd[m_writeID]->ids->normalBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_simData.m_vertexCount * sizeof(m_vd[m_writeID]->data->normalBuffer[0]),
		m_vd[m_writeID]->data->normalBuffer);

	return 0;
}


void* ClothSimulator::UpdatePartial(void * args)
{
	ThreadData* tData = (ThreadData*)args;

	while (tData->inst->m_threadsRunning)
	{
		// wait unitl hold lock is released
		pthread_mutex_lock(tData->inst->m_mutexHold);
		pthread_mutex_unlock(tData->inst->m_mutexHold);
		if (tData->inst->m_barrierCounter >= THREAD_COUNT)
			continue;

		// get data
		VertexData* clothData = tData->inst->m_meshPlane->GetVertexDataPtr();
		BoxAAData* boxData = nullptr;
		SphereData* sphereData = nullptr;
		if (PhysicsManager::GetInstance()->GetBoxCollidersData()->size() != 0)
			boxData = &(PhysicsManager::GetInstance()->GetBoxCollidersData()->at(0));
		if (PhysicsManager::GetInstance()->GetSphereCollidersData()->size() != 0)
			sphereData = &(PhysicsManager::GetInstance()->GetSphereCollidersData()->at(0));
		int bcCount = PhysicsManager::GetInstance()->GetBoxCollidersData()->size();
		int scCount = PhysicsManager::GetInstance()->GetSphereCollidersData()->size();
		float groundLevel = System::GetInstance()->GetCurrentScene()->GetGroundLevel();

		glm::mat4 zero = glm::mat4();
		glm::mat4* wm = &zero;

		if (tData->inst->m_obj->GetTransform() != nullptr)
			wm = tData->inst->m_obj->GetTransform()->GetWorldMatrix();

		glm::mat4* vw = System::GetInstance()->GetCurrentScene()->GetCamera()->GetViewMatrix();
		glm::mat4* pr = System::GetInstance()->GetCurrentScene()->GetCamera()->GetProjMatrix();

		// first stage
		if (tData->inst->m_simParams.mode == ClothSimulationMode::MASS_SPRING_CPUx4)
		{
			for (int i = tData->diBegin; i < tData->diEnd; ++i)
			{
				tData->inst->CPUComputePositionMS(i, &tData->inst->m_simData.b_sLengths[i], &tData->inst->m_simData.b_sLengthsDiag[i], &tData->inst->m_simData.b_sLengths2[i],
					(float)Timer::GetInstance()->GetFixedDeltaTime(), PhysicsManager::GetInstance()->GetGravity());
			}
		}
		else if (tData->inst->m_simParams.mode == ClothSimulationMode::POSITION_BASED_CPUx4)
		{
			for (int i = tData->diBegin; i < tData->diEnd; ++i)
			{
				tData->inst->CPUComputePositionPB(i, &tData->inst->m_simData.b_sLengths[i], &tData->inst->m_simData.b_sLengthsDiag[i], &tData->inst->m_simData.b_sLengths2[i],
					(float)Timer::GetInstance()->GetFixedDeltaTime(), PhysicsManager::GetInstance()->GetGravity());
			}
		}

		pthread_mutex_lock(tData->inst->m_mutexBarrierCounter);
		++tData->inst->m_barrierCounter;
		pthread_mutex_unlock(tData->inst->m_mutexBarrierCounter);

		pthread_mutex_lock(tData->inst->m_mutexHoldStage2);
		pthread_mutex_unlock(tData->inst->m_mutexHoldStage2);

		//second stage
		for (int i = tData->diBegin; i < tData->diEnd; ++i)
		{
			tData->inst->CPUComputeCollision(i, wm, vw, pr, boxData, sphereData, bcCount, scCount, groundLevel);
		}

		pthread_mutex_lock(tData->inst->m_mutexBarrierCounter);
		++tData->inst->m_barrierCounter;
		pthread_mutex_unlock(tData->inst->m_mutexBarrierCounter);

		pthread_mutex_lock(tData->inst->m_mutexHoldStage3);
		pthread_mutex_unlock(tData->inst->m_mutexHoldStage3);

		// third stage

		for (int i = tData->diBegin; i < tData->diEnd; ++i)
		{
			tData->inst->CPUComputeNormal(i);
		}

		pthread_mutex_lock(tData->inst->m_mutexBarrierCounter);
		++tData->inst->m_barrierCounter;
		pthread_mutex_unlock(tData->inst->m_mutexBarrierCounter);
	}

	return NULL;
}

inline unsigned int ClothSimulator::UpdateSimGPU
	(
		VertexData* vertexData,
		BoxAAData* boxAAData,
		SphereData* sphereData,
		int bcCount,
		int scCount,
		glm::mat4* worldMatrix,
		glm::mat4* viewMatrix,
		glm::mat4* projMatrix,
		glm::vec3* gravity,
		float fixedDelta,
		float groundLevel
	)
{
	unsigned int err = CS_ERR_NONE;
	ShaderID* cShaderID = Renderer::GetInstance()->GetCurrentShaderID();


	glBindVertexArray(m_vaoUpdateID[m_readID]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosID[m_readID]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosLastID[m_readID]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vd[m_readID]->ids->normalBuffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbours);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighboursDiag);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbours2);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbourMultipliers);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbourDiagMultipliers);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbour2Multipliers);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_sLengths);
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_sLengthsDiag);
	glEnableVertexAttribArray(10);
	glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_sLengths2);
	glEnableVertexAttribArray(11);
	glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_elMassCoeffs);
	glEnableVertexAttribArray(12);
	glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_multipliers);
	glEnableVertexAttribArray(13);
	glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// execute simulation kernel
	if (m_simParams.mode == ClothSimulationMode::MASS_SPRING_GPU)
	{
		err = UpdateSimMSGPU(
			PhysicsManager::GetInstance()->GetGravity(),
			fixedDelta
			);
	}
	else if (m_simParams.mode == ClothSimulationMode::POSITION_BASED_GPU)
	{
		err = UpdateSimPBGPU(
			PhysicsManager::GetInstance()->GetGravity(),
			fixedDelta
			);
	}

	SwapRWIds();

	// here we get the D

	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, m_vboPosID[m_readID]);
	int dID = m_simData.m_vertexCount / 2;
	glm::vec4* ptr = (glm::vec4*)glMapBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, (dID) * sizeof(glm::vec4), sizeof(glm::vec4), GL_MAP_READ_BIT);
	//LOGW("%f %f %f %f\n", ptr->x, ptr->y, ptr->z, ptr->w);
	if (ptr != nullptr)
	{
		glm::vec3 dNew = glm::vec3(*ptr);
		m_d = glm::length(dNew - m_dLast);
		m_dLast = dNew;
		glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
	}

	// execute collision computation kernel

	glUseProgram(m_collisionsKernel->id);

	//glDisableVertexAttribArray(0);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texColPosID[m_readID], 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosID[m_readID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, 0);

	glUniformMatrix4fv(m_collisionsKernel->uniformIDs->at(0), 1, GL_FALSE, (float*)worldMatrix);
	glUniformMatrix4fv(m_collisionsKernel->uniformIDs->at(1), 1, GL_FALSE, (float*)viewMatrix);
	glUniformMatrix4fv(m_collisionsKernel->uniformIDs->at(2), 1, GL_FALSE, (float*)projMatrix);
	glUniform4fv(m_collisionsKernel->uniformIDs->at(3), 1, (float*)&m_simData.c_touchVector);
	glUniform1f(m_collisionsKernel->uniformIDs->at(4), groundLevel);
	glUniform1i(m_collisionsKernel->uniformIDs->at(5), bcCount);
	glUniform1i(m_collisionsKernel->uniformIDs->at(6), scCount);
	glUniform1i(m_collisionsKernel->uniformIDs->at(7), m_simData.m_edgesWidthAll);
	glUniform1i(m_collisionsKernel->uniformIDs->at(8), m_simData.m_edgesLengthAll);
	glUniform1i(m_collisionsKernel->uniformIDs->at(9), m_simData.m_vertexCount);
	glUniform1i(m_collisionsKernel->uniformIDs->at(10), COLLISION_CHECK_WINDOW_SIZE);

#ifndef PLATFORM_WINDOWS
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texColPosID[m_readID], m_vboPosID[m_readID]);

	glBindBuffer(GL_UNIFORM_BUFFER, m_vboColBAAID);
	glBufferData(GL_UNIFORM_BUFFER, bcCount * sizeof(BoxAAData), boxAAData, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texColBAAID, m_vboColBAAID);

	glBindBuffer(GL_UNIFORM_BUFFER, m_vboColSID);
	glBufferData(GL_UNIFORM_BUFFER, scCount * sizeof(SphereData), sphereData, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texColSID, m_vboColSID);
#else
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_texColPosID[m_readID]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboColBAAID);
	glBufferData(GL_ARRAY_BUFFER, bcCount * sizeof(BoxAAData), boxAAData, GL_DYNAMIC_COPY);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_BUFFER, m_texColBAAID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboColBAAID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboColSID);
	glBufferData(GL_ARRAY_BUFFER, scCount * sizeof(SphereData), sphereData, GL_DYNAMIC_COPY);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_BUFFER, m_texColSID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboColSID);

	GLint loc = glGetUniformLocation(m_collisionsKernel->id, "InPos");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_collisionsKernel->id, "InBaaCols");
	glUniform1i(loc, 1);
	loc = glGetUniformLocation(m_collisionsKernel->id, "InSCols");
	glUniform1i(loc, 2);
#endif

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosID[m_readID]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosLastID[m_readID]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_ctfID);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vboPosID[m_writeID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vboPosLastID[m_writeID]);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_simData.m_vertexCount);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	//glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, m_vboPosID[m_writeID]);
	//ptr = (glm::vec4*)glMapBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(glm::vec4), GL_MAP_READ_BIT);
	//LOGW("%f %f %f %f\n", ptr->x, ptr->y, ptr->z, ptr->w);
	//glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);

	glBindBufferBase(GL_UNIFORM_BUFFER, m_texColPosID[m_readID], 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosID[m_readID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);


	// execute normal computation kernel

	glUseProgram(m_normalsKernel->id);

	//glDisableVertexAttribArray(0);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texNrmPosID[m_readID], 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosID[m_readID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, 0);

#ifndef PLATFORM_WINDOWS

	glBindBufferBase(GL_UNIFORM_BUFFER, m_texNrmPosID[m_readID], m_vboPosID[m_readID]);

#else

	glBindTexture(GL_TEXTURE_BUFFER, m_texNrmPosID[m_readID]);

#endif

	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosID[m_readID]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighboursDiag);

	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_simData.i_neighbourDiagMultipliers);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_ntfID);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vboNrmID[m_writeID]);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_simData.m_vertexCount);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	glBindBufferBase(GL_UNIFORM_BUFFER, m_texNrmPosID[m_readID], 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPosID[m_readID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, 0);
	glBindVertexArray(0);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	/////////////////////

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(7);
	glDisableVertexAttribArray(8);
	glDisableVertexAttribArray(9);
	glDisableVertexAttribArray(10);
	glDisableVertexAttribArray(11);
	glDisableVertexAttribArray(12);
	glDisableVertexAttribArray(13);

	SwapRWIds();
	
	//m_meshPlane->SwapDataPtrs();

	glUseProgram(cShaderID->id);

#ifndef PLATFORM_WINDOWS

#else
	glBindTexture(GL_TEXTURE_BUFFER, 0);
#endif

	return err;
}

////////////////////// MASS SPRING 

inline unsigned int ClothSimulator::InitializeSimMSGPU()
{
	unsigned int err = CS_ERR_NONE;

	m_msPosKernelID = ResourceManager::GetInstance()->LoadKernel(&KERNEL_MSPOS_NAME);
	glGenTransformFeedbacks(1, &m_msPtfID);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_msPtfID);
	glTransformFeedbackVaryings(m_msPosKernelID->id, KERNEL_MSPOS_OUTPUT_NAME_COUNT, KERNEL_MSPOS_OUTPUT_NAMES, GL_SEPARATE_ATTRIBS);
	glLinkProgram(m_msPosKernelID->id);

#ifndef PLATFORM_WINDOWS
	for (uint i = 0; i < 2; ++i)
	{
		m_texPosID[i] = glGetUniformBlockIndex(m_msPosKernelID->id, KERNEL_MSPOS_INPUT_NAMES[0]);
		m_texPosLastID[i] = glGetUniformBlockIndex(m_msPosKernelID->id, KERNEL_MSPOS_INPUT_NAMES[1]);
		glUniformBlockBinding(m_msPosKernelID->id, m_texPosID[i], 1);
		glUniformBlockBinding(m_msPosKernelID->id, m_texPosID[i], 0);
	}
#else
	for (uint i = 0; i < 2; ++i)
	{
		glGenTextures(1, &m_texPosID[i]);
		glBindTexture(GL_TEXTURE_BUFFER, m_texPosID[i]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboPosID[i]);
		glGenTextures(1, &m_texPosLastID[i]);
		glBindTexture(GL_TEXTURE_BUFFER, m_texPosLastID[i]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboPosLastID[i]);
	}
#endif

	m_msPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_msPosKernelID->id, "VertexCount"));
	m_msPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_msPosKernelID->id, "EdgesWidthAll"));
	m_msPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_msPosKernelID->id, "EdgesLengthAll"));
	m_msPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_msPosKernelID->id, "DeltaTime"));
	m_msPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_msPosKernelID->id, "Gravity"));

	return err;
}

inline unsigned int ClothSimulator::ShutdownSimMSGPU()
{
	unsigned int err = CS_ERR_NONE;

	// shutdown kernels 
	glDeleteTransformFeedbacks(1, &m_msPtfID);

	// shutdown method-related data
#ifndef PLATFORM_WINDOWS

#else
	glDeleteTextures(2, m_texPosID);
	glDeleteTextures(2, m_texPosLastID);
#endif

	return err;
}

inline unsigned int ClothSimulator::UpdateSimMSGPU(glm::vec3* gravity, float fixedDelta)
{
	unsigned int err = CS_ERR_NONE;

	glUseProgram(m_msPosKernelID->id);

	glUniform1i(m_msPosKernelID->uniformIDs->at(0), m_simData.m_vertexCount);
	glUniform1i(m_msPosKernelID->uniformIDs->at(1), m_simData.m_edgesWidthAll);
	glUniform1i(m_msPosKernelID->uniformIDs->at(2), m_simData.m_edgesLengthAll);
	glUniform1f(m_msPosKernelID->uniformIDs->at(3), fixedDelta);
	glUniform1f(m_msPosKernelID->uniformIDs->at(4), -gravity->y);

#ifndef PLATFORM_WINDOWS
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosID[m_readID], m_vboPosID[m_readID]);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosLastID[m_readID], m_vboPosLastID[m_readID]);
#else
	glBindTexture(GL_TEXTURE_BUFFER, m_texPosID[m_readID]);
	glBindTexture(GL_TEXTURE_BUFFER, m_texPosLastID[m_readID]);
#endif

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_msPtfID);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vboPosID[m_writeID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vboPosLastID[m_writeID]);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_simData.m_vertexCount);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	//glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, m_vboPosID[m_writeID]);
	//glm::vec4* ptr = (glm::vec4*)glMapBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(glm::vec4), GL_MAP_READ_BIT);
	//LOGW("%f %f %f %f\n", ptr->x, ptr->y, ptr->z, ptr->w);
	//glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);

#ifndef PLATFORM_WINDOWS
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosID[m_readID], 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosLastID[m_readID], 0);
#else
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif

	return err;
}

//////////////////////////////

//////////////////// POSITION BASED

inline unsigned int ClothSimulator::InitializeSimPBGPU()
{
	unsigned int err = CS_ERR_NONE;

	m_pbPosKernelID = ResourceManager::GetInstance()->LoadKernel(&KERNEL_PBPOS_NAME);
	glGenTransformFeedbacks(1, &m_pbPtfID);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_pbPtfID);
	glTransformFeedbackVaryings(m_pbPosKernelID->id, KERNEL_PBPOS_OUTPUT_NAME_COUNT, KERNEL_PBPOS_OUTPUT_NAMES, GL_SEPARATE_ATTRIBS);
	glLinkProgram(m_pbPosKernelID->id);

#ifndef PLATFORM_WINDOWS
	for (uint i = 0; i < 2; ++i)
	{
		m_texPosID[i] = glGetUniformBlockIndex(m_pbPosKernelID->id, KERNEL_PBPOS_INPUT_NAMES[0]);
		m_texPosLastID[i] = glGetUniformBlockIndex(m_pbPosKernelID->id, KERNEL_PBPOS_INPUT_NAMES[1]);
		glUniformBlockBinding(m_pbPosKernelID->id, m_texPosID[i], 1);
		glUniformBlockBinding(m_pbPosKernelID->id, m_texPosID[i], 0);
	}
#else
	for (uint i = 0; i < 2; ++i)
	{
		glGenTextures(1, &m_texPosID[i]);
		glBindTexture(GL_TEXTURE_BUFFER, m_texPosID[i]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboPosID[i]);
		glGenTextures(1, &m_texPosLastID[i]);
		glBindTexture(GL_TEXTURE_BUFFER, m_texPosLastID[i]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vboPosLastID[i]);
	}
#endif

	m_pbPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_pbPosKernelID->id, "VertexCount"));
	m_pbPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_pbPosKernelID->id, "EdgesWidthAll"));
	m_pbPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_pbPosKernelID->id, "EdgesLengthAll"));
	m_pbPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_pbPosKernelID->id, "DeltaTime"));
	m_pbPosKernelID->uniformIDs->push_back(glGetUniformLocation(m_pbPosKernelID->id, "Gravity"));

	return err;
}

inline unsigned int ClothSimulator::ShutdownSimPBGPU()
{
	unsigned int err = CS_ERR_NONE;

	// shutdown kernels 
	glDeleteTransformFeedbacks(1, &m_pbPtfID);

	// shutdown method-related data
#ifndef PLATFORM_WINDOWS

#else
	glDeleteTextures(2, m_texPosID);
	glDeleteTextures(2, m_texPosLastID);
#endif
	return err;
}

inline unsigned int ClothSimulator::UpdateSimPBGPU(glm::vec3* gravity, float fixedDelta)
{
	unsigned int err = CS_ERR_NONE;

	glUseProgram(m_pbPosKernelID->id);

	glUniform1i(m_pbPosKernelID->uniformIDs->at(0), m_simData.m_vertexCount);
	glUniform1i(m_pbPosKernelID->uniformIDs->at(1), m_simData.m_edgesWidthAll);
	glUniform1i(m_pbPosKernelID->uniformIDs->at(2), m_simData.m_edgesLengthAll);
	glUniform1f(m_pbPosKernelID->uniformIDs->at(3), fixedDelta);
	glUniform1f(m_pbPosKernelID->uniformIDs->at(4), -gravity->y);

#ifndef PLATFORM_WINDOWS
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosID[m_readID], m_vboPosID[m_readID]);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosLastID[m_readID], m_vboPosLastID[m_readID]);
#else
	glBindTexture(GL_TEXTURE_BUFFER, m_texPosID[m_readID]);
	glBindTexture(GL_TEXTURE_BUFFER, m_texPosLastID[m_readID]);
#endif

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_pbPtfID);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vboPosID[m_writeID]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vboPosLastID[m_writeID]);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_simData.m_vertexCount);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);
	/*
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, m_vboPosID[m_writeID]);
	glm::vec4* ptr = (glm::vec4*)glMapBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 1, GL_MAP_READ_BIT);
	for (int i = 0; i < 1; ++i, ++ptr)
	LOGW("%f %f %f %f", ptr->x, ptr->y, ptr->z, ptr->w);
	glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
	*/

#ifndef PLATFORM_WINDOWS
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosID[m_readID], 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_texPosLastID[m_readID], 0);
#else
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif

	return err;
}

inline unsigned int ClothSimulator::UpdateSimMSCPU(glm::vec3* gravity, float fixedDelta)
{
	unsigned int err = CS_ERR_NONE;

	for (int i = 0; i < m_simData.m_vertexCount; ++i)
	{
		CPUComputePositionMS(i, &m_simData.b_sLengths[i], &m_simData.b_sLengthsDiag[i], &m_simData.b_sLengths2[i], fixedDelta, gravity);
	}

	return err;
}

inline unsigned int ClothSimulator::UpdateSimPBCPU(glm::vec3* gravity, float fixedDelta)
{
	unsigned int err = CS_ERR_NONE;

	for (int i = 0; i < m_simData.m_vertexCount; ++i)
	{
		CPUComputePositionPB(i, &m_simData.b_sLengths[i], &m_simData.b_sLengthsDiag[i], &m_simData.b_sLengths2[i], fixedDelta, gravity);
	}
	
	return err;
}

inline void ClothSimulator::CPUComputePositionMS(int i, glm::vec4* sls1, glm::vec4* sls2, glm::vec4* sls3, float fixedDelta, glm::vec3* gravity)
{
	glm::vec3 posCurrent, posLast, force, tempForce, velocity, posNew, nPos, nPosLast, nVelocity;
	float elasticity, elDamp, airDamp, mass, lockMultiplier;

	posCurrent = glm::vec3(m_vd[m_readID]->data->positionBuffer[i]);
	posLast = glm::vec3(m_vdCopy[m_readID]->data->positionBuffer[i]);
	velocity = (posCurrent - posLast) / fixedDelta;
	force = glm::vec3(0.0f);
	elasticity = m_simData.b_elMassCoeffs[i][0];
	mass = m_simData.b_elMassCoeffs[i][1];
	elDamp = m_simData.b_elMassCoeffs[i][2];
	airDamp = m_simData.b_elMassCoeffs[i][3];
	lockMultiplier = m_simData.b_multipliers[i][0];

	for (int j = 0; j < 4; ++j)
	{
		int nID = (int)(glm::roundEven(m_simData.b_neighbours[i][j]));
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
		{
			//LOGI("%d", nID);
			continue;
		}

		nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		nPosLast = glm::vec3(m_vdCopy[m_readID]->data->positionBuffer[nID]);

		CalculateSpringForce(&posCurrent, &posLast, &nPos, &nPosLast, (*sls1)[j],
			elasticity, elDamp, fixedDelta, &tempForce);

		force += tempForce * m_simData.b_neighbourMultipliers[i][j];
	}
	for (int j = 0; j < 4; ++j)
	{
		int nID = (int)(m_simData.b_neighboursDiag[i][j]);
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
			continue;
		nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		nPosLast = glm::vec3(m_vdCopy[m_readID]->data->positionBuffer[nID]);

		CalculateSpringForce(&posCurrent, &posLast, &nPos, &nPosLast, (*sls2)[j],
			elasticity, elDamp, fixedDelta, &tempForce);

		force += tempForce * m_simData.b_neighbourDiagMultipliers[i][j];
	}
	for (int j = 0; j < 4; ++j)
	{
		int nID = (int)(m_simData.b_neighbours2[i][j]);
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
			continue;
		nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		nPosLast = glm::vec3(m_vdCopy[m_readID]->data->positionBuffer[nID]);

		CalculateSpringForce(&posCurrent, &posLast, &nPos, &nPosLast, (*sls3)[j],
			elasticity, elDamp, fixedDelta, &tempForce);

		force += tempForce * m_simData.b_neighbour2Multipliers[i][j];
	}

	force += mass * (*gravity);
	force += (-airDamp * velocity);
	force *= lockMultiplier;
	force = force / mass;

	posNew = 2.0f * posCurrent - posLast + force * fixedDelta * fixedDelta;

	m_vdCopy[m_writeID]->data->positionBuffer[i] = glm::vec4(posCurrent, 1.0f);
	m_vd[m_writeID]->data->positionBuffer[i] = glm::vec4(posNew, 1.0f);
}

inline void ClothSimulator::CPUComputePositionPB(int i, glm::vec4* sls1, glm::vec4* sls2, glm::vec4* sls3, float fixedDelta, glm::vec3* gravity)
{
	glm::vec3 posCurrent, posLast, velocity, force, posPredicted, cPos, posNew;
	float elasticity, airDamp, mass, lockMultiplier;
	float elBias = 0.0004f;

	posCurrent = glm::vec3(m_vd[m_readID]->data->positionBuffer[i]);
	posLast = glm::vec3(m_vdCopy[m_readID]->data->positionBuffer[i]);
	velocity = (posCurrent - posLast) / fixedDelta;
	force = glm::vec3(0.0f, 0.0f, 0.0f);
	elasticity = m_simData.b_elMassCoeffs[i].x * elBias;
	mass = m_simData.b_elMassCoeffs[i].y;
	airDamp = m_simData.b_elMassCoeffs[i].w;
	lockMultiplier = m_simData.b_multipliers[i].x;

	force += mass * (*gravity);
	force += -airDamp * velocity;
	force *= lockMultiplier;
	force = force / mass;
	posPredicted = 2.0f * posCurrent - posLast + force * fixedDelta * fixedDelta;
	//posPredicted = posCurrent;

	int fOrder[4] = { 0, 2, 1, 3 };
	cPos = glm::vec3(0.0f);
	for (int j = 0; j < 2; ++j)
	{
		int id = fOrder[j];
		int nID = (int)(glm::roundEven(m_simData.b_neighbours[i][id]));
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
		{
			continue;
		}

		glm::vec3 nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		glm::vec3 cstr = glm::vec3(0.0f, 0.0f, 0.0f);
		float w = 1.0f;
		CalcDistConstraint(&posPredicted, &nPos, mass, (*sls1)[id], elasticity, &cstr, &w);
		cstr = cstr * w * m_simData.b_neighbourMultipliers[i][id];
		cPos -= cstr * lockMultiplier;
	}
	posPredicted += cPos;
	cPos = glm::vec3(0.0f);
	for (int j = 2; j < 4; ++j)
	{
		int id = fOrder[j];
		int nID = (int)(glm::roundEven(m_simData.b_neighbours[i][id]));
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
		{
			continue;
		}

		glm::vec3 nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		glm::vec3 cstr = glm::vec3(0.0f, 0.0f, 0.0f);
		float w = 1.0f;
		CalcDistConstraint(&posPredicted, &nPos, mass, (*sls1)[id], elasticity, &cstr, &w);
		cstr = cstr * w * m_simData.b_neighbourMultipliers[i][id];
		cPos -= cstr * lockMultiplier;
	}
	posPredicted += cPos;

	cPos = glm::vec3(0.0f);
	for (int j = 0; j < 2; ++j)
	{
		int id = fOrder[j];
		int nID = (int)(glm::roundEven(m_simData.b_neighboursDiag[i][id]));
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
		{
			continue;
		}

		glm::vec3 nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		glm::vec3 cstr = glm::vec3(0.0f, 0.0f, 0.0f);
		float w = 1.0f;
		CalcDistConstraint(&posPredicted, &nPos, mass, (*sls2)[id], elasticity, &cstr, &w);
		cstr = cstr * w * m_simData.b_neighbourDiagMultipliers[i][id];
		cPos -= cstr * lockMultiplier;
	}
	posPredicted += cPos;
	cPos = glm::vec3(0.0f);
	for (int j = 2; j < 4; ++j)
	{
		int id = fOrder[j];
		int nID = (int)(glm::roundEven(m_simData.b_neighboursDiag[i][id]));
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
		{
			continue;
		}

		glm::vec3 nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		glm::vec3 cstr = glm::vec3(0.0f, 0.0f, 0.0f);
		float w = 1.0f;
		CalcDistConstraint(&posPredicted, &nPos, mass, (*sls2)[id], elasticity, &cstr, &w);
		cstr = cstr * w * m_simData.b_neighbourDiagMultipliers[i][id];
		cPos -= cstr * lockMultiplier;
	}
	posPredicted += cPos;
	
	cPos = glm::vec3(0.0f);
	for (int j = 0; j < 2; ++j)
	{
		int id = fOrder[j];
		int nID = (int)(glm::roundEven(m_simData.b_neighbours2[i][id]));
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
		{
			continue;
		}

		glm::vec3 nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		glm::vec3 cstr = glm::vec3(0.0f, 0.0f, 0.0f);
		float w = 1.0f;
		CalcDistConstraint(&posPredicted, &nPos, mass, (*sls3)[id], elasticity, &cstr, &w);
		cstr = cstr * w * m_simData.b_neighbour2Multipliers[i][id];
		cPos -= cstr * lockMultiplier;
	}
	posPredicted += cPos;
	cPos = glm::vec3(0.0f);
	for (int j = 2; j < 4; ++j)
	{
		int id = fOrder[j];
		int nID = (int)(glm::roundEven(m_simData.b_neighbours2[i][id]));
		if (nID < 0 || nID >= m_simData.m_vertexCount || nID == i)
		{
			continue;
		}

		glm::vec3 nPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[nID]);
		glm::vec3 cstr = glm::vec3(0.0f, 0.0f, 0.0f);
		float w = 1.0f;
		CalcDistConstraint(&posPredicted, &nPos, mass, (*sls3)[id], elasticity, &cstr, &w);
		cstr = cstr * w * m_simData.b_neighbour2Multipliers[i][id];
		cPos -= cstr * lockMultiplier;
	}
	posPredicted += cPos;

	posNew = posPredicted;
	m_vd[m_writeID]->data->positionBuffer[i] = glm::vec4(posNew, 1.0f);
	m_vdCopy[m_writeID]->data->positionBuffer[i] = glm::vec4(posCurrent, 1.0f);
}

inline void ClothSimulator::CPUComputeCollision(int i, glm::mat4* worldMatrix, glm::mat4* viewMatrix, glm::mat4* projMatrix, BoxAAData boxAAData[], SphereData sphereData[],
	int bcCount, int scCount, float groundLevel)
{
	glm::vec3 colOffset = glm::vec3(0.0f);
	glm::vec4 modelPos = m_vd[m_readID]->data->positionBuffer[i];
	glm::vec3 worldPos = glm::vec3(*worldMatrix * modelPos);
	glm::vec3 screenPos = glm::vec3(0.0f);	// !
	glm::vec3 mPos = glm::vec3(modelPos);
	glm::vec4 lPos = m_vdCopy[m_readID]->data->positionBuffer[i];
	glm::vec3 totalOffset = glm::vec3(0.0f);
	float mR = m_simData.b_multipliers[i].y;

	// external collisons
	for (int j = 0; j < bcCount; ++j)
	{
		colOffset = glm::vec3(0.0f);
		BoxAAData col = boxAAData[j];
		glm::vec3 closest = glm::min(glm::max(worldPos, col.min), col.max);
		glm::vec3 diff = closest - worldPos;
		float dist = Vec3LengthSquared(&diff);

		if (dist < (mR * mR) && dist != 0.0f)
		{
			closest = worldPos - closest;
			colOffset = normalize(closest) * (mR - sqrt(dist));
			worldPos += colOffset;
			totalOffset += colOffset;
		}
	}

	for (int j = 0; j < scCount; ++j)
	{
		colOffset = glm::vec3(0.0f);
		SphereData col = sphereData[j];
		glm::vec3 diff = worldPos - col.center;
		float diffLength = Vec3LengthSquared(&diff);
		if
			(
				diffLength < (mR + col.radius) * (mR + col.radius) &&
				diffLength != 0.0f
				)
		{
			diff = normalize(diff);
			diff = diff * ((mR + col.radius) - sqrt(diffLength));

			colOffset = diff;
			worldPos += colOffset;
			totalOffset += colOffset;
		}
	}

	// internal collisions
	for (int j = 0; j < 4; ++j)
	{
		colOffset = glm::vec3(0.0f);
		glm::vec4 mPos = m_vd[m_readID]->data->positionBuffer[(int)(glm::roundEven(m_simData.b_neighbours[i][j]))];
		glm::vec3 nPos = glm::vec3(*worldMatrix * mPos);
		glm::vec3 diff = worldPos - nPos;
		float diffLength = Vec3LengthSquared(&diff);
		if
			(
				diffLength < (mR + mR) * (mR + mR) &&
				diffLength != 0.0f
				)
		{
			diff = normalize(diff);
			diff = diff * ((mR + mR) - sqrt(diffLength)) * 0.5f;

			colOffset = diff;
			worldPos += colOffset;
			totalOffset += colOffset;
		}
	}

	totalOffset *= m_simData.b_multipliers[i].x;
	glm::vec4 finalPos = glm::vec4(mPos + totalOffset, 1.0f);

	// finger movement
	glm::vec4 mPosScreen = *projMatrix * (*viewMatrix * (*worldMatrix * finalPos));
	glm::vec4 mPosScreenNorm = mPosScreen / mPosScreen.w;
	glm::vec4 fPosScreen = glm::vec4(m_simData.c_touchVector.x, m_simData.c_touchVector.y, 0.0f, mPosScreenNorm.w);
	glm::vec4 fDirScreen = glm::vec4(m_simData.c_touchVector.z, m_simData.c_touchVector.w, 0.0f, 0.0f);
	float A = 200.0f;
	float s = 300.0f;
	float coeff = A * glm::exp(-((fPosScreen.x - mPosScreenNorm.x) * (fPosScreen.x - mPosScreenNorm.x) +
		(fPosScreen.y - mPosScreenNorm.y) * (fPosScreen.y - mPosScreenNorm.y)) / 2.0f * s);
	fDirScreen *= mPosScreen.w;
	fDirScreen = glm::inverse(*worldMatrix) * (glm::inverse(*viewMatrix) * (glm::inverse(*projMatrix) * fDirScreen));
	fDirScreen *= coeff * glm::length(glm::vec2(m_simData.c_touchVector.z, m_simData.c_touchVector.w)) * m_simData.b_multipliers[i].x;
	finalPos.x += fDirScreen.x;
	finalPos.y += fDirScreen.y;
	finalPos.z += fDirScreen.z;

	// ground level
	glm::vec4 mPosWorld = *worldMatrix * finalPos;
	float glAddition = glm::max(groundLevel - mPosWorld.y, 0.0f);
	finalPos.y += glAddition;

	m_vd[m_writeID]->data->positionBuffer[i] = finalPos;
	m_vdCopy[m_writeID]->data->positionBuffer[i] = lPos;
}

inline void ClothSimulator::CPUComputeNormal(int i)
{
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec3 mPos = glm::vec3(m_vd[m_readID]->data->positionBuffer[i]);

	float ids[8];
	float mpliers[8];
	ids[0] = m_simData.b_neighbours[i][0];
	ids[1] = m_simData.b_neighboursDiag[i][0];
	ids[2] = m_simData.b_neighbours[i][1];
	ids[3] = m_simData.b_neighboursDiag[i][1];
	ids[4] = m_simData.b_neighbours[i][2];
	ids[5] = m_simData.b_neighboursDiag[i][2];
	ids[6] = m_simData.b_neighbours[i][3];
	ids[7] = m_simData.b_neighboursDiag[i][3];
	mpliers[0] = m_simData.b_neighbourMultipliers[i][0];
	mpliers[1] = m_simData.b_neighbourDiagMultipliers[i][0];
	mpliers[2] = m_simData.b_neighbourMultipliers[i][1];
	mpliers[3] = m_simData.b_neighbourDiagMultipliers[i][1];
	mpliers[4] = m_simData.b_neighbourMultipliers[i][2];
	mpliers[5] = m_simData.b_neighbourDiagMultipliers[i][2];
	mpliers[6] = m_simData.b_neighbourMultipliers[i][3];
	mpliers[7] = m_simData.b_neighbourDiagMultipliers[i][3];

	for (int j = 0; j < 8; ++j)
	{
		int nID1 = (int)(glm::roundEven(ids[j]));
		int nID2 = (int)(glm::roundEven(ids[(j + 1) % 8]));
		glm::vec3 diff1 = mPos - glm::vec3(m_vd[m_readID]->data->positionBuffer[nID1]);
		glm::vec3 diff2 = mPos - glm::vec3(m_vd[m_readID]->data->positionBuffer[nID2]);

		glm::vec3 pNormal = (glm::cross(diff1, diff2) * mpliers[j] *
			mpliers[(j + 1) % 8]);
		if (pNormal.x != pNormal.x || pNormal.y != pNormal.y || pNormal.z != pNormal.z)
			pNormal = glm::vec3(0.0f, 1.0f, 0.0f);
		normal += pNormal;
	}
	normal = glm::normalize(normal);
	normal.z = -normal.z;
	m_vd[m_writeID]->data->normalBuffer[i] = glm::vec4(normal, 1.0f);
}

/////////////////////////////////

inline void ClothSimulator::CalculateSpringForce
(
	const glm::vec3 * mPos, 
	const glm::vec3 * mPosLast, 
	const glm::vec3 * nPos, 
	const glm::vec3 * nPosLast, 
	float sLength, 
	float elCoeff, 
	float dampCoeff,
	float fixedDelta,
	glm::vec3* ret
)
{
	glm::vec3 mVelocity = (*mPos - *mPosLast) / fixedDelta;
	glm::vec3 nVelocity = (*nPos - *nPosLast) / fixedDelta;

	glm::vec3 f = *mPos - *nPos;
	glm::vec3 n = glm::normalize(f);
	float fLength = glm::length(f);
	float spring = fLength - sLength;
	glm::vec3 spr = -elCoeff * spring * n;

	glm::vec3 dV = mVelocity - nVelocity;
	float damp = dampCoeff * (glm::dot(dV, f) / fLength);

	float sL = length(spr);
	glm::vec3 dmp = n * glm::min(sL, damp);

	glm::vec3 fin = (spr + dmp);
	ret->x = fin.x;
	ret->y = fin.y;
	ret->z = fin.z;
}

inline void ClothSimulator::CalcDistConstraint(
	glm::vec3* mPos,
	glm::vec3* nPos,
	float mass,
	float sLength,
	float elCoeff,
	glm::vec3* outConstraint,
	float* outW
	) 
{
	glm::vec3 diff = *mPos - *nPos;
	float cLength = length(diff);
	glm::vec3 dP = (2.0f * mass) * (cLength - sLength) * (diff / cLength) * elCoeff;
	*outConstraint = dP;
	*outW = 1.0f / mass;
}

inline void ClothSimulator::CopyVertexData(VertexData * source, VertexData * dest)
{
	if (dest->data == nullptr)
		dest->data = new VertexDataRaw;
	if (dest->ids == nullptr)
		dest->ids = new VertexDataID;

	dest->data->vertexCount = source->data->vertexCount;
	dest->data->indexCount = source->data->indexCount;

	if (dest->data->barycentricBuffer == nullptr)
		dest->data->barycentricBuffer = new glm::vec4[dest->data->indexCount];
	if (dest->data->colorBuffer == nullptr)
		dest->data->colorBuffer = new glm::vec4[dest->data->vertexCount];
	if (dest->data->indexBuffer == nullptr)
		dest->data->indexBuffer = new unsigned int[dest->data->indexCount];
	if (dest->data->normalBuffer == nullptr)
		dest->data->normalBuffer = new glm::vec4[dest->data->vertexCount];
	if (dest->data->positionBuffer == nullptr)
		dest->data->positionBuffer = new glm::vec4[dest->data->vertexCount];
	if (dest->data->uvBuffer == nullptr)
		dest->data->uvBuffer = new glm::vec2[dest->data->vertexCount];

	dest->ids->barycentricBuffer = source->ids->barycentricBuffer;
	dest->ids->colorBuffer = source->ids->colorBuffer;
	dest->ids->indexBuffer = source->ids->indexBuffer;
	dest->ids->normalBuffer = source->ids->normalBuffer;
	dest->ids->uvBuffer = source->ids->uvBuffer;
	dest->ids->vertexArrayID = source->ids->vertexArrayID;
	dest->ids->vertexBuffer = source->ids->vertexBuffer;	

	for (unsigned int i = 0; i < dest->data->vertexCount; ++i)
	{
		dest->data->positionBuffer[i] = source->data->positionBuffer[i];
		dest->data->uvBuffer[i] = source->data->uvBuffer[i];
		dest->data->normalBuffer[i] = source->data->normalBuffer[i];
		dest->data->colorBuffer[i] = source->data->colorBuffer[i];
	}

	for (unsigned int i = 0; i < dest->data->vertexCount; ++i)
	{
		dest->data->indexBuffer[i] = source->data->indexBuffer[i];
		dest->data->barycentricBuffer[i] = source->data->barycentricBuffer[i];
	}
}

inline void ClothSimulator::SwapRWIds()
{
	unsigned int tmp = m_readID;
	m_readID = m_writeID;
	m_writeID = tmp;
}

inline void ClothSimulator::AppendRestart()
{
	m_ifRestart = true;
}

inline void ClothSimulator::RestartSimulation()
{
	if(m_readID == 1)
		m_meshPlane->SwapDataPtrs();

	m_readID = 0;
	m_writeID = 1;

	Shutdown();

	if (m_tempSimParamsPtr != nullptr)
	{
		UpdateSimParams(m_tempSimParamsPtr);
		m_tempSimParamsPtr = nullptr;
	}	

	Initialize();
}
