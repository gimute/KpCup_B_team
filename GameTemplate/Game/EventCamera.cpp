#include "stdafx.h"
#include "EventCamera.h"

namespace {
	//�V�[��1�̐ݒ�
	//const Vector3 SCENE_1_VECTOR[] = { {0.0f,400.0f,400.0f}, };
	const Vector3 VEC = { 0.0,0.0,0.0 };
	const Vector3	SCENE_1_VECTOR[5] = { { 0.0f, 50.0f, 200.0f },
										{ 170.0f, 50.0f, 30.0f },
										{ 100.0f, 50.0f, 50.0f },
										{ 220.0f, 5.0f, 0.0f },
										{ 0.0f, 50.0f, 200.0f } };
}

EventCamera::EventCamera()
{

}

EventCamera::~EventCamera()
{

}

bool EventCamera::Start()
{
	//�V�[��1�̐ݒ�
	SetScenePosInstance(en_Scene1_Door, SCENE_1_VECTOR);

	return true;
}

void EventCamera::Update()
{

}

