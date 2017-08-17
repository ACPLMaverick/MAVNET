#pragma once
#include "Postprocess.h"

namespace Postprocesses
{
	class SSDOBase :
		public Postprocess
	{
	protected:

		ID3D11Buffer* _dataBuffer;

	public:
		SSDOBase();
		~SSDOBase();

		// Inherited via Postprocess
		virtual void Update() override;
		virtual void SetPass(const Camera& camera, int32_t passIndex = 0) const;
		virtual inline int GetPassCount() const { return 1; }
	};

}

