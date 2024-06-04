#include "stdafx.h"
#include "EventCamera.h"
#include "Game.h"

namespace {
	int extractionNumListA[] = { 0,1,2,5,6 };
	int extractionNumListB[] = { 1,1,3,1,3 };
}

EventCamera::EventCamera()
{
	
}

EventCamera::~EventCamera()
{

}

bool EventCamera::Start()
{
	//カメラ位置が記録されたレベルを読み込む
	m_camLevelRender.Init("Assets/levelData/eventCamTest.tkl", [&](LevelObjectData_Render& objData)
	{
		if (objData.ForwardMatchName(L"A_Scene1Pos") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene1_Door,ListUpdateMode::en_ModePosition);
			return true;
		}
		else if(objData.ForwardMatchName(L"A_Scene1Tar") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene1_Door, ListUpdateMode::en_ModeTarget);
		}
		else if(objData.ForwardMatchName(L"B_Scene2Pos") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene2_MapUp1, ListUpdateMode::en_ModePosition);
		}
		else if (objData.ForwardMatchName(L"B_Scene2Tar") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene2_MapUp1, ListUpdateMode::en_ModeTarget);
		}
		else if (objData.ForwardMatchName(L"C_Scene3Pos") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene3_MapUp2, ListUpdateMode::en_ModePosition);
		}
		else if (objData.ForwardMatchName(L"C_Scene3Tar") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene3_MapUp2, ListUpdateMode::en_ModeTarget);
		}
		return true;
	});

	//ゲームクラスのインスタンスを取得
	m_game = FindGO<Game>("game");

	return true;
}

void EventCamera::SetSceneCamAndTarPos(const Vector3& setPos, const int setNum
	, const EnEventScene setScene,ListUpdateMode updateMode)
{
	//構造体宣言
	SceneVector setVector;

	std::string charStrA = std::to_string(setNum);

	charStrA.erase(charStrA.begin());

	int Num, BoolNum,IfNum;
	float EasingTime,ChangeTime;

	for (int i = 0; i < 5; i++)
	{
		//00000000を0A 0B 000C 000Dとして0Aが先頭番号
		//0BがBool、000Cがイージング割合、0Dが切り替え条件の付与、
		//000Eが切り替えまでの秒数とする。

		//※注意！
		//切り替え条件がオン(切り替え条件の付与変数が１)になっている場合は
		//切り替えまでの秒数(000E)が切り替え条件対象ナンバーとなります。

		std::string charStrB = charStrA.substr(extractionNumListA[i],
			extractionNumListB[i]);

		switch (i)
		{
		case 0:
			//先頭番号
			Num = std::stoi(charStrB);
			break;		
		case 1:
			//bool１か０かで判定
			BoolNum = std::stoi(charStrB);
			break;		
		case 2:
			//カメライージング割合
			EasingTime = std::stoi(charStrB);
			EasingTime *= 0.1;
			break;		
		case 3:
			//切り替え条件の付与
			IfNum = std::stoi(charStrB);
			break;
		case 4:
			//カメラ切り替え秒数
			ChangeTime = std::stoi(charStrB);
			if (IfNum == 0)
			{
				ChangeTime *= 0.1;
			}
			break;
		}
	}

	//↑の変数を構造体に格納
	if (BoolNum == 0)
	{
		setVector.isEasing = false;
	}
	else
	{
		setVector.isEasing = true;
	}

	if (IfNum == 0)
	{
		setVector.isSwitchingCon = false;
	}
	else
	{
		setVector.isSwitchingCon = true;
	}

	setVector.m_changeTime = ChangeTime;

	setVector.m_easingRatio = EasingTime;

	setVector.m_vector = setPos;


	//enum型で格納対象を変更
	switch (updateMode)
	{
	case EventCamera::en_ModePosition:
		//カメラ位置map型配列に格納
		m_scene[setScene].m_cameraWayPoint.insert({ Num,setVector });

		m_scene[setScene].m_useWayPoint++;
		break;
	case EventCamera::en_ModeTarget:
		//カメラターゲット位置map型配列に格納
		m_scene[setScene].m_cameraChangeTarget.insert({ Num,setVector });

		m_scene[setScene].m_useChangeTarget++;
		break;
	}
}

void EventCamera::Update()
{
	//イベントフラグが立っていなければ
	if (!m_eventFlag)
	{
		//処理はしない
		return;
	}

	//カメラ位置変更時間が0以下＆
	//イテレーターがまだendじゃなかったら
	if (m_posChangeTime <= 0.0f && 
		!m_cameraPosListEnd)
	{
		//カメラ位置のイテレーターの要素を進める
		CamPositionListChange(m_camPosListIterator
			,ListUpdateMode::en_ModePosition);
	}

	//カメラターゲット位置変更時間が0以下＆
	//イテレーターがまだendじゃなかったら
	if (m_tarChangeTime <= 0.0f && 
		!m_cameraTarListEnd)
	{
		//カメラターゲット位置のイテレーターの要素を進める
		CamPositionListChange(m_camTarListIterator
			, ListUpdateMode::en_ModeTarget);
	}

	//イテレーターがまだendじゃなかったら
	if (!m_cameraPosListEnd)
	{
		//カメラ位置更新
		m_sendCameraPosition = CamPositionUpdate(m_camPosListIterator
			,m_easingRatioCamPos,ListUpdateMode::en_ModePosition);
	}

	//イテレーターがまだendじゃなかったら
	if (!m_cameraTarListEnd)
	{
		//ターゲット位置更新
		m_sendTargetPosition = CamPositionUpdate(m_camTarListIterator
			, m_easingRatioTarPos,ListUpdateMode::en_ModeTarget);
	}

	//カメラに座標を送る
	g_camera3D->SetPosition(m_sendCameraPosition);
	g_camera3D->SetTarget(m_sendTargetPosition);

	//カメラ位置、カメラターゲット位置、どちらもイテレーターがendだったら
	if (m_cameraTarListEnd && m_cameraPosListEnd)
	{
		//イベントを終了する
		m_game->EventUiDelete(false);
		m_eventFlag = false;
	}
}

Vector3 EventCamera::CamPositionUpdate(std::map<int, SceneVector>::iterator setIterator
, const float easingTime,ListUpdateMode updateMode)
{
	//最終的に戻り値として返すベクトル
	Vector3 LastVector;

	//現在処理中のイテレーターのイージングがオンだったら
	if (IsCamPosIteratorEasing(setIterator,updateMode))
	{
		//そしてイージングが終了していなかったら
		if (!IsIteratorEasingEnd(setIterator))
		{
			//イージングした座礁を代入する
			LastVector = Easing(setIterator, easingTime
			,updateMode);
		}
		//イージングが終了していたら
		else
		{
			//現在のイテレーターから要素を１つ進めたイテレーターの
			//座標を代入する
			LastVector = GetListPos(setIterator, 1,updateMode);
		}
	}
	//オフだったら
	else
	{
		//イテレーターに格納されている座標を代入
		LastVector = GetListPos(setIterator);
	}	
	
	//イージングの割合計算、カメラ切り替えまでの時間を処理する
	Time(setIterator,updateMode);

	return LastVector;
}

void EventCamera::CamPositionListChange(std::map<int, SceneVector>::iterator &setIterator
, ListUpdateMode updateMode)
{
	//イテレーターの要素を１つ進める
	setIterator++;

	//enum型アップデートモードで分岐
	switch (updateMode)
	{

	//アップデートモードがpositionだったら
	case EventCamera::en_ModePosition:

		//現在のイテレーターがendだったら
		if (setIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
		{
			//endフラグをtrueにする
			m_cameraPosListEnd = true;

			return;
		}


		//現在のイテレーターのカメラ切り替え条件がオンになっているなら
		if (setIterator->second.isSwitchingCon == true)
		{
			//カメラターゲット切り替え時間を1.0で初期化
			m_posChangeTime = 1.0;
			//カメラターゲット切り替え条件ナンバーを初期化
			m_camSwitchingNum = setIterator->second.m_changeTime;
		}
		//オフだったら
		else
		{
			//カメラ位置切り替え時間を現在のイテレーターで初期化
			m_posChangeTime = setIterator->second.m_changeTime;
		}


		//現在のイテレーターのイージングがオンだったら
		if (IsCamPosIteratorEasing(setIterator,ListUpdateMode::en_ModePosition))
		{
			//イージングの割合を現在のイテレーターで初期化
			m_easingPosRatio = m_camPosListIterator->second.m_easingRatio;
		}

		break;

		//アップデートモードがtargetだったら
	case EventCamera::en_ModeTarget:

		//現在のイテレーターがendだったら
		if (setIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
		{			
			//endフラグをtrueにする
			m_cameraTarListEnd = true;

			return;
		}

		//現在のイテレーターのカメラ切り替え条件がオンになっているなら
		if (setIterator->second.isSwitchingCon == true)
		{
			//カメラターゲット切り替え時間を1.0で初期化
			m_tarChangeTime = 1.0;
			//カメラターゲット切り替え条件ナンバーを初期化
			m_tarSwitchingNum = setIterator->second.m_changeTime;
		}
		//オフだったら
		else
		{
			//カメラターゲット切り替え時間を現在のイテレーターで初期化
			m_tarChangeTime = setIterator->second.m_changeTime;
		}

		//現在のイテレーターのイージングがオンだったら
		if (IsCamPosIteratorEasing(setIterator,ListUpdateMode::en_ModeTarget))
		{
			//イージングの割合を現在のイテレーターで初期化
			m_easingTarRatio = m_camTarListIterator->second.m_easingRatio;
		}

		break;

	}

	return;
}

Vector3 EventCamera::Easing(std::map<int, SceneVector>::iterator setIterator
,float easingRatio,ListUpdateMode updateMode)
{

	//イージング割合が最大になったら
	if (easingRatio >= 1.0f)
	{
		easingRatio = 1.0f;
		//イージング割合を初期化
		EasingClear(updateMode);
		//イージング終了フラグをtrueに
		setIterator->second.isEasingEnd = true;
	}

	//ココから下はイージングの処理
	Vector3 Last;

	Vector3 a = GetListPos(setIterator);

	Vector3 b = GetListPos(setIterator,1);

	Last.Lerp(easingRatio,a, b);

	return Last;
}

void EventCamera::Time(std::map<int, SceneVector>::iterator setIterator
	, ListUpdateMode updateMode)
{
	//切り替え条件がナンバーの時の処理
	if (setIterator->second.isSwitchingCon == true)
	{
		switch (updateMode)
		{
		case EventCamera::en_ModePosition:
			//現在処理中のイテレーターのイージングが終了しておらず
			//現在処理中のイテレーターのイージングがオンであれば
			if (!IsIteratorEasingEnd(m_camPosListIterator)
				&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModePosition))
			{
				//イージングの割合を増やす
				m_easingRatioCamPos += g_gameTime->GetFrameDeltaTime() * m_easingPosRatio;
			}
			//上記の条件と合わなければ
			else
			{
				//もしカメラターゲットのナンバーが
				//指定した条件ナンバーだったら
				if (m_camTarListIterator->first == m_camSwitchingNum)
				{
					//カメラ位置切り替えタイマーを0.0fにする
					m_posChangeTime = 0.0f;
				}
			}
			break;
		case EventCamera::en_ModeTarget:
			//現在処理中のイテレーターのイージングが終了しておらず
			//現在処理中のイテレーターのイージングがオンであれば
			if (!IsIteratorEasingEnd(m_camTarListIterator)
				&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModeTarget))
			{
				//イージングの割合を増やす
				m_easingRatioTarPos += g_gameTime->GetFrameDeltaTime() * m_easingTarRatio;
			}
			//上記の条件と合わなければ
			else
			{
				//もしカメラターゲットのナンバーが
				//指定した条件ナンバーだったら
				if (m_camPosListIterator->first == m_tarSwitchingNum)
				{
					//カメラ位置切り替えタイマーを0.0fにする
					m_tarChangeTime = 0.0f;
				}
			}
			break;
		}
		return;
	}

	//切り替え条件が時間の時の処理
	//	//enum型アップデートモードで分岐
	switch (updateMode)
	{

	//アップデートモードがpositionだったら
	case EventCamera::en_ModePosition:

		//現在処理中のイテレーターのイージングが終了しておらず
		//現在処理中のイテレーターのイージングがオンであれば
		if (!IsIteratorEasingEnd(m_camPosListIterator)
			&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModePosition))
		{
			//イージングの割合を増やす
			m_easingRatioCamPos += g_gameTime->GetFrameDeltaTime() * m_easingPosRatio;
		}
		//上記の条件と合わなければ
		else
		{
			//カメラ位置切り替え時間を減らす
			m_posChangeTime -= g_gameTime->GetFrameDeltaTime();
		}
		break;

	//アップデートモードがpositionだったら
	case EventCamera::en_ModeTarget:

		//現在処理中のイテレーターのイージングが終了しておらず
		//現在処理中のイテレーターのイージングがオンであれば
		if (!IsIteratorEasingEnd(m_camTarListIterator)
			&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModeTarget))
		{
			//イージングの割合を増やす
			m_easingRatioTarPos += g_gameTime->GetFrameDeltaTime() * m_easingTarRatio;
		}
		else
		{
			//カメラターゲット位置切り替え時間を減らす
			m_tarChangeTime -= g_gameTime->GetFrameDeltaTime();
		}
		break;
	}
}