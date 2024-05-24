#include "stdafx.h"
#include "Load.h"
#include "Game.h"
#include "GameClear.h"

Load::Load()
{

}

Load::~Load()
{

}

bool Load::Start()
{
	//ƒ[ƒh‰æ–Ê‚Ì‰æ‘œ
	m_spriteRender_load.Init("Assets/sprite/Game_Loading.DDS", 1920.0f, 1080.0f);
	m_spriteRender_load.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_spriteRender_load.Update();
	return true;
}

void Load::Update()
{
	switch (m_state) {
	case enState_FadeIn: //”–‚Ü‚é
		m_currentAlpha -= 0.50f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut: //”Z‚­‚È‚é
		m_currentAlpha += 0.5f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_Idle:
		break;
	}
}

void Load::Render(RenderContext& rc)
{
	if (m_currentAlpha > 0.0f) {
		m_spriteRender_load.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_spriteRender_load.Draw(rc);
	}
}