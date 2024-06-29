#include "stdafx.h"
#include "InformationUi.h"

InformationUi::InformationUi()
{

}

InformationUi::~InformationUi()
{

}

bool InformationUi::Start()
{
	InformationSpriteParam* info1 = new InformationSpriteParam;
	info1->InitInformationSpriteParam("test/test", 0.0f, 0.0f, 5.0f);
	InitInformationList("info1", info1);

	return true;
}

void InformationUi::Update()
{

}

void InformationUi::Render(RenderContext& rc)
{

}

