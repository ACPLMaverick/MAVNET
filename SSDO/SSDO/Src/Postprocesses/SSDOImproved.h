#pragma once
#include "Postprocess.h"
#include "SATGenerator.h"

class Texture;
class RWTexture;

namespace Postprocesses
{
	class SSDOImproved :
		public Postprocess
	{
	public:

		static const int32_t SAMPLE_COUNT = 14;

	protected:


		XMFLOAT4A _offsets[SAMPLE_COUNT];
		float _maxDistance;
		float _fadeStart;
		float _epsilon;
		float _powFactor;

		SATGenerator _satGen;

		ID3D11Buffer* _dataBuffer;
		RWTexture* _satColor;
		RWTexture* _satNormalDepth;
		RWTexture* _bufColor;
		RWTexture* _bufNormalDepth;

		Texture* _testInput;
		RWTexture* _testBuf;
		RWTexture* _testOutput;

		inline void FillParams(XMFLOAT4A* paramBuffer) const;

	public:
		SSDOImproved();
		~SSDOImproved();

		// Inherited via Postprocess
		virtual void Update() override;
		virtual void SetPass(GBuffer& gBuffer, const Camera& camera, int32_t passIndex = 0) const override;
		virtual void AfterPass(GBuffer& gBuffer, const Camera& camera, int32_t passIndex = 0) const override;
		virtual inline int GetPassCount() const { return 1; }
	};

}

