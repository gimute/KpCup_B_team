#include "stdafx.h"
#include "system/system.h"
#include "ModelTest.h"
#include "ModelRenderTest.h"
#include "SpriteTest.h"
#include "Title.h"
#include "Game.h"
#include "Player.h"

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

	
	//追加コード
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
	Title* title = NewGO<Title>(0, "title");
	//Game* game = NewGO<Game>(0,"game");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//追加したコード
		auto& renderContext = g_graphicsEngine->GetRenderContext();
		// 

		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		//追加コード
		//ライトの更新処理
		m_sceneLight.Update();

		//レンダリングエンジンの描画処理
		m_renderingEngine.Execute(renderContext);
		//

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}


	//DeleteGO(modelTest);
	//DeleteGO(modelRenderTest);
	//DeleteGO(spriteTest);
	DeleteGO(title);
	//DeleteGO(game);

	delete g_k2EngineLow;

	return 0;
}

