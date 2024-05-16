#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::Title()
{
	//‰æ‘œ‚ğ“Ç‚İ‚ŞB
	m_spriteRender.Init("Assets/sprite/Game_Title4.DDS", 1920.0f, 1080.0f);
}

Title::~Title()
{

}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0,"game");
		DeleteGO(this);
	}
}

void  Title::Render(RenderContext& rc)
{
	//‰æ‘œ‚ğ•`‰æ‚·‚éB
	m_spriteRender.Draw(rc);
}
