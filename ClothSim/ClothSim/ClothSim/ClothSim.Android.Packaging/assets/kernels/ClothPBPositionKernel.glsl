#version 300 es

layout(location = 0) in vec4 Pos;					//current position
layout(location = 1) in vec4 PosLast;				//previous position
layout(location = 2) in vec4 Normal;	
layout(location = 3) in vec4 Neighbours;			//neighbours id
layout(location = 4) in vec4 NeighboursDiag;
layout(location = 5) in vec4 Neighbours2;
layout(location = 6) in vec4 NeighbourMultipliers;	// neighbour multipliers (i.e. do I have to take it into consideration)
layout(location = 7) in vec4 NeighbourDiagMultipliers;
layout(location = 8) in vec4 Neighbour2Multipliers;
layout(location = 9) in vec4 SLengths;
layout(location = 10) in vec4 SLengthsDiag;
layout(location = 11) in vec4 SLengths2;
layout(location = 12) in vec4 ElMassCoeffs;			// x - elasticity, y - mass, z - el damp coeff, w - air damp coeff
layout(location = 13) in vec4 Multipliers;			// x - lock muliplier, y - collision multiplier

uniform InPos
{
	vec4[16384] InPosBuffer;
};
uniform InPosLast
{
	vec4[16384] InPosLastBuffer;
};

uniform int VertexCount;
uniform int EdgesWidthAll;
uniform int EdgesLengthAll;
uniform float DeltaTime;
uniform float Gravity;

out vec4 OutPos;
out vec4 OutPosLast;

void CalcDistConstraint(vec3 mPos, vec3 nPos, float mass, float sLength, float elCoeff, float dampCoeff, out vec4 constraint)
{
	elCoeff = clamp(elCoeff, 0.0f, 1.0f);
	vec3 diff = mPos - nPos;
	float cLength = max(length(diff), 0.0000001f);
	vec3 dP = (2.0f * mass) * (cLength - sLength) * (diff / cLength) * elCoeff;
	constraint.xyz = dP;
	constraint.w = 1.0f / mass;
}

void main()
{
	vec3 mPos = vec3(Pos);
	vec3 mPosLast = vec3(PosLast);
	vec3 mVel = (mPos - mPosLast) / DeltaTime;

	//////////////////////////////////////////////////////
	// forces calculation
	vec3 mForce = vec3(0.0f);
	vec3 posPredicted = vec3(0.0f);
		// calculate gravity force
	float grav = Gravity;
	mForce = mForce + (ElMassCoeffs.y * vec3(0.0f, -grav, 0.0f));

		// calculate air damp force
	mForce = mForce + (-ElMassCoeffs.w * mVel);

		// check hooks
	mForce = mForce * Multipliers.x;
	
	// calculate acceleration and use Verelet integration to calculate PREDICTED position
	vec3 acc = mForce / ElMassCoeffs.y;
	posPredicted = 2.0f * vec3(Pos) - vec3(PosLast) + acc * DeltaTime * DeltaTime;

	
	// compute constraints
	ivec4 fOrder = ivec4(0, 2, 1, 3);
	float elBias = 0.0004f;
	int id = 0; 
	int nID = 0;
	vec3 nPos = vec3(0.0f); 
	vec3 nPosLast = vec3(0.0f);
	vec4 constraint = vec4(0.0f);

	// the manual loop unroll was necessary to prevent GPU crashing (EGL_CONTEXT_LOST) on high number of vertices
	// might be an issue with Adreno GLSL compiler
	// one might think two-iteration loops would always be automatically unrolled

	vec3 cPos = vec3(0.0f);
		id = fOrder[0];
		nID = int(roundEven(Neighbours[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourMultipliers[id] * Multipliers.x;

		id = fOrder[1];
		nID = int(roundEven(Neighbours[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourMultipliers[id] * Multipliers.x;
	posPredicted += cPos;


	cPos = vec3(0.0f);
		id = fOrder[2];
		nID = int(roundEven(Neighbours[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourMultipliers[id] * Multipliers.x;

		id = fOrder[3];
		nID = int(roundEven(Neighbours[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourMultipliers[id] * Multipliers.x;
	posPredicted += cPos;




	cPos = vec3(0.0f);
		id = fOrder[0];
		nID = int(roundEven(NeighboursDiag[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengthsDiag[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourDiagMultipliers[id] * Multipliers.x;

		id = fOrder[1];
		nID = int(roundEven(NeighboursDiag[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengthsDiag[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourDiagMultipliers[id] * Multipliers.x;
	posPredicted += cPos;


	cPos = vec3(0.0f);
		id = fOrder[2];
		nID = int(roundEven(NeighboursDiag[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengthsDiag[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourDiagMultipliers[id] * Multipliers.x;

		id = fOrder[3];
		nID = int(roundEven(NeighboursDiag[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengthsDiag[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * NeighbourDiagMultipliers[id] * Multipliers.x;
	posPredicted += cPos;




	cPos = vec3(0.0f);
		id = fOrder[0];
		nID = int(roundEven(Neighbours2[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths2[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * Neighbour2Multipliers[id] * Multipliers.x;

		id = fOrder[1];
		nID = int(roundEven(Neighbours2[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths2[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * Neighbour2Multipliers[id] * Multipliers.x;
	posPredicted += cPos;


	cPos = vec3(0.0f);
		id = fOrder[2];
		nID = int(roundEven(Neighbours2[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths2[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * Neighbour2Multipliers[id] * Multipliers.x;

		id = fOrder[3];
		nID = int(roundEven(Neighbours2[id]));
		nPos = vec3(InPosBuffer[nID]);
		nPosLast = vec3(InPosLastBuffer[nID]);
		// distance constriant. XYZ is position, W is inverse of mass
		CalcDistConstraint(posPredicted, nPos, ElMassCoeffs.y, SLengths2[id], ElMassCoeffs.x * elBias, ElMassCoeffs.z * elBias, constraint);
		cPos -= constraint.xyz * constraint.w * Neighbour2Multipliers[id] * Multipliers.x;
	posPredicted += cPos;

	// apply constraints
	vec3 finalPos = posPredicted;

	// update positions
	OutPos = vec4(finalPos, 1.0f);
	OutPosLast = Pos;
	gl_Position = Pos;
}