#version 300 es

layout(location = 0) in vec4 Pos;					//current position
layout(location = 1) in vec4 PosLast;				//previous position
layout(location = 3) in vec4 Neighbours;			//neighbours id
layout(location = 6) in vec4 NeighbourMultipliers;	// neighbour multipliers (i.e. do I have to take it into consideration)
layout(location = 13) in vec4 Multipliers;			// x - lock muliplier, y - collision multiplier

uniform InPos
{
	vec4[16384] InPosBuffer;
};
uniform InBaaCols
{
	mat2x4[8192] baaBuffer;
};
uniform InSCols
{
	vec4[16384] sBuffer;
};

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjMatrix;
uniform vec4 TouchVector;	// x,y - position; z,w - direction
uniform float GroundLevel;
uniform int BoxAAColliderCount;
uniform int SphereColliderCount;
uniform int EdgesWidthAll;
uniform int EdgesLengthAll;
uniform int VertexCount;
uniform int InternalCollisionCheckWindowSize;

out vec4 OutPos;
out vec4 OutPosLast;

void Vec3LengthSquared(in vec3 vec, out float ret)
{
	ret = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

void CalculateCollisionSphere(vec3 mCenter, float mRadius, vec3 sphereCenter, float sphereRadius, float multiplier, inout vec3 ret)
{
	vec3 diff = mCenter - sphereCenter;
	float diffLength;
	Vec3LengthSquared(diff, diffLength);
	ret = vec3(0.0f);

	if
	(
		diffLength < (mRadius + sphereRadius) * (mRadius + sphereRadius) &&
		diffLength != 0.0f
	)
	{
		diff = normalize(diff);
		diff = diff * ((mRadius + sphereRadius) - sqrt(diffLength)) * multiplier;

		ret = diff;
	}
}

void CalculateCollisionBoxAA(vec3 mCenter, float mRadius, vec3 bMin, vec3 bMax, float multiplier, inout vec3 ret)
{
	vec3 closest = min(max(mCenter, bMin), bMax);
	float dist;
	Vec3LengthSquared(closest - mCenter, dist);
	ret = vec3(0.0f);

	if(dist < (mRadius * mRadius) && dist != 0.0f)
	{
		closest = mCenter - closest;
		ret = normalize(closest) * (mRadius - sqrt(dist)) * multiplier;
	}
}

void main()
{
	vec3 colOffset = vec3(0.0f, 0.0f, 0.0f);
	vec3 mPos = vec3(WorldMatrix * Pos);
	vec3 totalOffset = vec3(0.0f);
	float mR = Multipliers.y;

	// solve external collisions
	for(int i = 0; i < BoxAAColliderCount; ++i)
	{
		mat2x4 box = baaBuffer[i];
		vec3 bMin = vec3(box[0][0], box[0][1], box[0][2]);
		vec3 bMax = vec3(box[1][0], box[1][1], box[1][2]);

		CalculateCollisionBoxAA(mPos, mR, bMin, bMax, 1.0f, colOffset);
		mPos += colOffset;
		totalOffset += colOffset;
	}

	for(int i = 0; i < SphereColliderCount; ++i)
	{
		vec4 sphere = sBuffer[i];
		vec3 sPos = vec3(sphere);
		float sR = sphere.w;

		CalculateCollisionSphere(mPos, mR, sPos, sR, 1.0f, colOffset);
		mPos += colOffset;
		totalOffset += colOffset;
	}

	// solve internal collisions
	//const int cc = 4;
	//const int nCount = (2 * cc + 1)*(2 * cc + 1) - 1;
	//int nIds[nCount];
	//int w = 0;
	//int v_cur = gl_VertexID;
	//for (int i = -cc; i <= cc; ++i)
	//{
	//	for (int j = -cc; j <= cc; ++j)
	//	{
	//		if (i == 0 && j == 0)
	//			continue;

	//		nIds[w] = abs((i * EdgesLengthAll + j + v_cur) % VertexCount);
	//		++w;
	//	}
	//}

	for (int i = 0; i < 4; ++i)
	{
		vec3 wnPos = vec3(WorldMatrix * InPosBuffer[int(Neighbours[i])]);
		CalculateCollisionSphere(mPos, mR, wnPos, mR, 0.5f, colOffset);
		mPos += colOffset;
		totalOffset += colOffset;
	}

	vec4 finalPos = vec4(Pos.x + totalOffset.x * Multipliers.x, Pos.y + totalOffset.y * Multipliers.x, Pos.z + totalOffset.z * Multipliers.x, Pos.w);
	
	// apply touch vector

	vec4 mPosScreen = ProjMatrix * (ViewMatrix * (WorldMatrix * finalPos));
	vec4 mPosScreenNorm = mPosScreen / mPosScreen.w;
	vec4 fPosScreen = vec4(TouchVector.x, TouchVector.y, 0.0f, mPosScreenNorm.w);
	vec4 fDirScreen = vec4(TouchVector.z, TouchVector.w, 0.0f, 0.0f);
	float A = 200.0f;
	float s = 300.0f;
	float coeff = A * exp(-((fPosScreen.x - mPosScreenNorm.x) * (fPosScreen.x - mPosScreenNorm.x) +
						(fPosScreen.y - mPosScreenNorm.y) * (fPosScreen.y - mPosScreenNorm.y)) / 2.0f * s);
	fDirScreen *= mPosScreen.w;
	fDirScreen = inverse(WorldMatrix) * (inverse(ViewMatrix) * (inverse(ProjMatrix) * fDirScreen));
	fDirScreen *= coeff * length(vec2(TouchVector.z, TouchVector.w)) * Multipliers.x;
	finalPos.x += fDirScreen.x;
	finalPos.y += fDirScreen.y;
	finalPos.z += fDirScreen.z;

	// ground level
	vec4 mPosWorld = WorldMatrix * finalPos;
	float glAddition = max(GroundLevel - mPosWorld.y, 0.0f);
	finalPos.y += glAddition;

	// update positions
	OutPos = finalPos;
	OutPosLast = PosLast;
	//OutPos = vec4(sBuffer[0][0], sBuffer[0][1], sBuffer[0][2], sBuffer[0][3]);
	gl_Position = Pos;
}