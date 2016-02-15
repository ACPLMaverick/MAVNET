#include "GUIButton.h"


GUIButton::GUIButton(const std::string* id) : GUIElement(id)
{
}

GUIButton::GUIButton(const GUIButton* c) : GUIElement(c)
{
}

GUIButton::~GUIButton()
{
}



unsigned int GUIButton::Initialize()
{
	return CS_ERR_NONE;
}

unsigned int GUIButton::Shutdown()
{
	return CS_ERR_NONE;
}



unsigned int GUIButton::Update()
{
	return CS_ERR_NONE;
}

unsigned int GUIButton::Draw()
{
	return CS_ERR_NONE;
}



void GUIButton::OnClick()
{
#ifdef _DEBUG
	printf("Error: GUIButton::OnClick() is not implemented yet.\n");
#endif
}

void GUIButton::OnHold()
{
#ifdef _DEBUG
	printf("Error: GUIButton::OnHold() is not implemented yet.\n");
#endif
}