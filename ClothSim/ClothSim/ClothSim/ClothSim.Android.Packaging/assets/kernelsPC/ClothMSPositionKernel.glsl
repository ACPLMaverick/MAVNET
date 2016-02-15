#version 330 core
#extension GL_EXT_gpu_shader4 : enable

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

uniform samplerBuffer InPos;
uniform samplerBuffer InPosLast;

uniform int VertexCount;
uniform int EdgesWidthAll;
uniform int EdgesLengthAll;
uniform float DeltaTime;
uniform float Gravity;

out vec4 OutPos;
out vec4 OutPosLast;

vec3 CalcSpringForce(vec3 mPos, vec3 mPosLast, vec3 nPos, vec3 nPosLast, float sLength, float elCoeff, float dampCoeff)
{
	vec3 ret = vec3(0.0f, 0.0f, 0.0f);
	vec3 mVel = (mPos - mPosLast) / DeltaTime;
	vec3 nVel = (nPos - nPosLast) / DeltaTime;

	vec3 f = mPos - nPos;
	vec3 n = normalize(f);
	float fLength = length(f);
	float spring = fLength - sLength;
	vec3 springiness = - elCoeff * spring * n;

	vec3 dV = mVel - nVel;
	float damp = dampCoeff * (dot(dV, f) / fLength);
	float sL = length(springiness);
	vec3 damping = n * min(sL, damp);

	ret = (springiness + damping);
	return ret;
}

void main()
{
	int mID = gl_VertexID;
	vec3 mPos = vec3(Pos);
	vec3 mPosLast = vec3(PosLast);
	vec3 mVel = (mPos - mPosLast) / DeltaTime;
	vec3 mForce = vec3(0.0f, 0.0f, 0.0f);

	// calculate elasticity force for each neighbouring vertices
	for(int i = 0; i < 4; ++i)
	{
		int nID = int(roundEven(Neighbours[i]));
		vec3 nPos = vec3(texelFetchBuffer(InPos, nID));
		vec3 nPosLast = vec3(texelFetchBuffer(InPosLast, nID));
		
		vec3 force = CalcSpringForce(mPos, mPosLast, nPos, nPosLast, SLengths[i], ElMassCoeffs.x, ElMassCoeffs.z);
		mForce += force * NeighbourMultipliers[i];
	}

	for(int i = 0; i < 4; ++i)
	{
		int nID = int(roundEven(NeighboursDiag[i]));
		vec3 nPos = vec3(texelFetchBuffer(InPos, nID));
		vec3 nPosLast = vec3(texelFetchBuffer(InPosLast, nID));
		
		vec3 force = CalcSpringForce(mPos, mPosLast, nPos, nPosLast, SLengthsDiag[i], ElMassCoeffs.x, ElMassCoeffs.z);
		mForce += force * NeighbourDiagMultipliers[i];
	}
	
	for(int i = 0; i < 4; ++i)
	{
		int nID = int(roundEven(Neighbours2[i]));
		vec3 nPos = vec3(texelFetchBuffer(InPos, nID));
		vec3 nPosLast = vec3(texelFetchBuffer(InPosLast, nID));
		
		vec3 force = CalcSpringForce(mPos, mPosLast, nPos, nPosLast, SLengths2[i], ElMassCoeffs.x, ElMassCoeffs.z);
		mForce += force * Neighbour2Multipliers[i];
	}

	// calculate gravity force
	float grav = Gravity;
	mForce = mForce + (ElMassCoeffs.y * vec3(0.0f, -grav, 0.0f));

	// calculate air damp force
	mForce = mForce + (-ElMassCoeffs.w * mVel);

	// check hooks
	mForce = mForce * Multipliers.x;
	
	// calculate acceleration and use Verelet integration to calculate position
	vec3 newPos;
	vec3 acc = mForce / ElMassCoeffs.y;

	newPos = 2.0f * vec3(Pos) - vec3(PosLast) + acc * DeltaTime * DeltaTime;

	// update positions
	OutPos = vec4(newPos, 1.0f);
	//OutPos = vec4(InPosBuffer[int(roundEven(NeighboursDiag[0]))][0], InPosBuffer[int(roundEven(NeighboursDiag[0]))][1], 
	//			InPosBuffer[int(roundEven(NeighboursDiag[0]))][2], InPosBuffer[int(roundEven(NeighboursDiag[0]))][3]);
	//OutPos = vec4(mForce, 1.0f);
	OutPosLast = Pos;
	gl_Position = Pos;
}