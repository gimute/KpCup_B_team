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
	SceneModel* testModel1 = new SceneModel(2, "Assets/modelData/player/EventTest/proto_player2.tkm");
	testModel1->RegistrationAnimation(0, "Assets/modelData/player/EventTest/idle.tka", true);
	testModel1->RegistrationAnimation(1, "Assets/modelData/player/EventTest/run.tka", true);
	testModel1->RegistrationConfirmed();
	m_sceneModelList.insert({ 0,testModel1 });

	testIter = m_sceneModelList.begin();

	////↑モデルレンダーにそのシーンで使うアニメーションを登録する処理↑////

	//イベントカメラのインスタンスを取得
	m_eventCamPtr = FindGO<EventCamera>("camera");

	return true;
}

void EventModel::Update()
{
	testIter->second->m_modelRender.PlayAnimation(1);

	testIter->second->m_modelRender.SetPosition(0.0,0.0,0.0);

	testIter->second->m_modelRender.Update();

	if (!m_eventFlag)
	{
		return;
	}

}

void EventModel::RegistrationSceneModel()
{
	
}

void EventModel::Render(RenderContext& rc)
{
	testIter->second->m_modelRender.Draw(rc);
}