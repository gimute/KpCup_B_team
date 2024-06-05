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

	//イベントカメラのインスタンスを取得
	m_eventCam = FindGO<EventCamera>("camera");

	return true;
}