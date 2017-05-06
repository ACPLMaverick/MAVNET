#pragma once
#include "Postprocess.h"

namespace Postprocesses
{
	class SimpleSSAO :
		public Postprocess
	{
	public:
		SimpleSSAO();
		~SimpleSSAO();

		// Inherited via Postprocess
		virtual void Update() override;
	};
}