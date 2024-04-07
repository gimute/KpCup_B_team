#include "stdafx.h"
#include "ModelRenderTest.h"

ModelRenderTest::ModelRenderTest()
{
	animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	animationClips[enAnimationClip_Jump].SetLoopFlag(false);

	m_pModelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_bgModelRender.Init("Assets/modelData/bg.tkm");
}

ModelRenderTest::~ModelRenderTest()
{

}


void ModelRenderTest::Update()
{
	m_modelPos.x += g_pad[0]->GetLStickXF() * 3.0f;
	m_modelPos.y += g_pad[0]->GetLStickYF() * 3.0f;

	m_modelRot.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_modelRot.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	if (g_pad[0]->IsPress(enButtonUp)) {
		m_modelScale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_modelScale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_modelScale.z += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_modelScale.z -= 0.02f;
	}

	m_pModelRender.SetPosition(m_modelPos);
	m_pModelRender.SetRotation(m_modelRot);
	m_pModelRender.SetScale(m_modelScale);

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_animeState += 1;
		m_animeState %= enAnimationClip_Num;
	}

	m_pModelRender.PlayAnimation(m_animeState);

	m_pModelRender.Update();
}

void ModelRenderTest::Render(RenderContext& rc)
{
	m_pModelRender.Draw(rc);
	m_bgModelRender.Draw(rc);
}

