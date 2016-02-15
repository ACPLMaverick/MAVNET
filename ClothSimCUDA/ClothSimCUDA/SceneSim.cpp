#include "SceneSim.h"


SceneSim::SceneSim(string n) : Scene(n)
{
}

SceneSim::SceneSim(const SceneSim* c) : Scene(c)
{
}


SceneSim::~SceneSim()
{
}



unsigned int SceneSim::Initialize()
{
	return CS_ERR_NONE;
}
