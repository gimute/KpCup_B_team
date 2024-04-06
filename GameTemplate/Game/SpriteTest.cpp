#include "stdafx.h"
#include "SpriteTest.h"

SpriteTest::SpriteTest()
{
	m_spriteRender.Init("Assets/image/test.DDS", 1280.0f, 720.0f);
	m_bgRender.Init("Assets/modelData/bg.tkm");

	m_spriteScale.Set(0.5f, 0.5f, 1.0f);
	//m_spritePos.Set(-600.0f, 300.0f, 0.0f);
}

SpriteTest::~SpriteTest()
{

}

void SpriteTest::Update()
{
	m_spritePos.x += g_pad[0]->GetLStickXF() * 1.0f;
	m_spritePos.y += g_pad[0]->GetLStickYF() * 1.0f;

	m_spriteRot.AddRotationZ(g_pad[0]->GetRStickXF() * 0.05f);

	if (g_pad[0]->IsPress(enButtonUp)) {
		m_spriteScale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_spriteScale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_spriteScale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_spriteScale.x -= 0.02f;
	}

	m_spriteRender.SetPosition(m_spritePos);
	m_spriteRender.SetRotation(m_spriteRot);
	m_spriteRender.SetScale(m_spriteScale);

	/*if (g_pad[0]->IsPress(enButtonA)) {
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
	}*/

	m_spriteRender.Update();
}

void SpriteTest::Render(RenderContext& rc)
{
	m_bgRender.Draw(rc);
	m_spriteRender.Draw(rc);
}