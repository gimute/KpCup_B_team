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
	static SceneModel testModel1(1, "Assets/modelData/player/proto_player/proto_player2.tkm");
	testModel1.AnimationRegistration(0, "Assets/modelData/player/proto_player/idle.tka", true);
	testModel1.RegistrationConfirmed();
	m_sceneModelMapList.push_back(testModel1);

	////�����f�������_�[�ɂ��̃V�[���Ŏg���A�j���[�V������o�^���鏈����////

	//�C�x���g�J�����̃C���X�^���X���擾
	m_eventCam = FindGO<EventCamera>("camera");

	return true;
}

void EventModel::Update()
{
	if (!m_eventFlag)
	{
		return;
	}
}