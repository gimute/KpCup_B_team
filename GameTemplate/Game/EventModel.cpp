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

	//�C�x���g�J�����̃C���X�^���X���擾
	m_eventCam = FindGO<EventCamera>("camera");

	return true;
}