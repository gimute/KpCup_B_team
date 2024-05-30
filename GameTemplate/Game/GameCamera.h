#pragma once
//#include "camera/SpringCamera.h"
class Player;
class EventCamera;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();


	Player* m_player;	//プレイヤー。
	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。
	EventCamera* m_eventCam = nullptr;

};

