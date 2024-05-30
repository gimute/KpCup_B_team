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


	Player* m_player;	//�v���C���[�B
	Vector3 m_toCameraPos;	//�����_���王�_�Ɍ������x�N�g���B
	EventCamera* m_eventCam = nullptr;

};

