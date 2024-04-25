#include "stdafx.h"
#include "system/system.h"
#include "ModelTest.h"
#include "ModelRenderTest.h"
#include "SpriteTest.h"
#include "Game.h"
#include "Player.h"

// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

	
	//�ǉ��R�[�h
	RenderingEngine m_renderingEngine;
	g_renderingEngine = &m_renderingEngine;

	SceneLight m_sceneLight;
	m_sceneLight.Init();
	g_sceneLight = &m_sceneLight;

	CollisionObjectManager m_collisionObjectManager;
	g_collisionObjectManager = &m_collisionObjectManager;
	//


	//ModelTest* modelTest = NewGO<ModelTest>(0);
	//ModelRenderTest* modelRenderTest = NewGO<ModelRenderTest>(0);
	//SpriteTest* spriteTest = NewGO<SpriteTest>(0);
	Game* game = NewGO<Game>(0);

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�ǉ������R�[�h
		auto& renderContext = g_graphicsEngine->GetRenderContext();
		// 

		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_k2EngineLow->ExecuteUpdate();

		//�ǉ��R�[�h
		//���C�g�̍X�V����
		m_sceneLight.Update();
		//

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_k2EngineLow->ExecuteRender();

		//�ǉ��R�[�h
		m_renderingEngine.Execute(renderContext);
		//

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}


	//DeleteGO(modelTest);
	//DeleteGO(modelRenderTest);
	//DeleteGO(spriteTest);
	DeleteGO(game);

	delete g_k2EngineLow;

	return 0;
}

