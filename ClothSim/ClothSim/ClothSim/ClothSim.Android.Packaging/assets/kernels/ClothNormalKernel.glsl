#version 300 es

layout(location = 0) in vec4 Pos;					//current position
layout(location = 2) in vec4 Normal;	
layout(location = 3) in vec4 Neighbours;			//neighbours id
layout(location = 4) in vec4 NeighboursDiag;			//neighboursDiag id
layout(location = 6) in vec4 NeighbourMultipliers;	// neighbour multipliers (i.e. do I have to take it into consideration)
layout(location = 7) in vec4 NeighbourDiagMultipliers;

uniform InPos
{
	vec4[16384] InPosBuffer;
};

out vec4 OutNormal;

void main()
{
	int mID = gl_VertexID;
	
	// calculate normals according to neighbours' positions

	vec3 normal = vec3(0.0f, 0.0f, 0.0f);
	vec3 mPos = vec3(Pos);

	float ids[8];
	float mpliers[8];
	ids[0] = Neighbours[0];
	ids[1] = NeighboursDiag[0];
	ids[2] = Neighbours[1];
	ids[3] = NeighboursDiag[1];
	ids[4] = Neighbours[2];
	ids[5] = NeighboursDiag[2];
	ids[6] = Neighbours[3];
	ids[7] = NeighboursDiag[3];
	mpliers[0] = NeighbourMultipliers[0];
	mpliers[1] = NeighbourDiagMultipliers[0];
	mpliers[2] = NeighbourMultipliers[1];
	mpliers[3] = NeighbourDiagMultipliers[1];
	mpliers[4] = NeighbourMultipliers[2];
	mpliers[5] = NeighbourDiagMultipliers[2];
	mpliers[6] = NeighbourMultipliers[3];
	mpliers[7] = NeighbourDiagMultipliers[3];

	for(int i = 0; i < 8; ++i)
	{
		int nID1 = int(roundEven(ids[i]));
		int nID2 = int(roundEven(ids[(i + 1) % 8]));

		vec3 diff1 = mPos - vec3(InPosBuffer[nID1]);
		vec3 diff2 = mPos - vec3(InPosBuffer[nID2]);

		normal = normal + (cross(diff1, diff2) * mpliers[i] * mpliers[(i + 1) % 8]);
	}
	normal = normalize(normal);
	normal.z = -normal.z;

	OutNormal = vec4(normal, 1.0f);
	gl_Position = Pos;
}