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
	////↓レベルレンダーで移動経路などを取得する処理↓////
	////↑レベルレンダーで移動経路などを取得する処理↑////
	
	////↓モデルレンダーにそのシーンで使うアニメーションを登録する処理↓////
	
	//↓登録例
	static SceneModel testModel1(1, "Assets/modelData/player/proto_player/proto_player2.tkm");
	testModel1.AnimationRegistration(0, "Assets/modelData/player/proto_player/idle.tka", true);
	testModel1.RegistrationConfirmed();
	m_sceneModelMapList.push_back(testModel1);

	////↑モデルレンダーにそのシーンで使うアニメーションを登録する処理↑////

	//イベントカメラのインスタンスを取得
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