#include "stdafx.h"
#include "EventModel.h"
#include "Game.h"
#include "EventCamera.h"

EventModel::EventModel()
{

}

EventModel::~EventModel()
{

}

bool EventModel::Start()
{
	////�����x�������_�[�ňړ��o�H�Ȃǂ��擾���鏈����////
	////�����x�������_�[�ňړ��o�H�Ȃǂ��擾���鏈����////
	
	////�����f�������_�[�ɂ��̃V�[���Ŏg���A�j���[�V������o�^���鏈����////
	
	//���o�^��
	static SceneModel testModel1(2, "Assets/modelData/player/EventTest/proto_player2.tkm");
	testModel1.AnimationRegistration(0, "Assets/modelData/player/EventTest/idle.tka", true);
	testModel1.AnimationRegistration(1, "Assets/modelData/player/EventTest/run.tka", true);
	testModel1.RegistrationConfirmed();
	m_sceneModelMapList.insert({ 0,&testModel1 });

	testIter = m_sceneModelMapList.begin();

	////�����f�������_�[�ɂ��̃V�[���Ŏg���A�j���[�V������o�^���鏈����////

	//�C�x���g�J�����̃C���X�^���X���擾
	m_eventCam = FindGO<EventCamera>("camera");

	return true;
}

void EventModel::Update()
{
	testIter->second->m_modelRender.PlayAnimation(0,0.1f);

	testIter->second->m_modelRender.SetPosition(0.0,0.0,0.0);

	testIter->second->m_modelRender.Update();

	if (!m_eventFlag)
	{
		return;
	}

}
void EventModel::Render(RenderContext& rc)
{
	testIter->second->m_modelRender.Draw(rc);
}