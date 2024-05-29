#pragma once

#define WAYPOINT_MAX 10
#define CHANGE_TARGET_MAX 10

class EventCamera :public IGameObject
{
public:
	/// <summary>
	/// イベントシーン
	/// </summary>
	enum EnEventScene {
		en_Scene1_Door,
		en_Scene2_MapUp,
		en_SceneNum,
		en_Scene_None
	};
	/// <summary>
	/// リスト更新モード
	/// </summary>
	enum ListUpdateMode {
		en_ModePosition,
		en_ModeTarget,
	};
	/// <summary>
	/// シーン内で使う座標記録用構造体
	/// </summary>
	struct SceneVector
	{
		/// <summary>
		/// vector3型
		/// </summary>
		Vector3 m_vector = Vector3::Zero;
		/// <summary>
		/// float型1
		/// </summary>
		float m_changeTime = 0.0f;
		/// <summary>
		/// float型2
		/// </summary>
		float m_easingRatio = 0.0f;
		/// <summary>
		/// この地点からつぎの地点にイージングしたいのであればtrue
		/// </summary>
		bool isEasing = false;
		/// <summary>
		/// この地点のイージングが終了しているかどうか
		/// </summary>
		bool isEasingEnd = false;
	};
	/// <summary>
	/// １シーンで使う変数などが入った構造体
	/// </summary>
	struct OneScene{
		/// <summary>
		/// カメラを動かす際のウェイポイント
		/// </summary>
		std::map<int, SceneVector> m_cameraWayPoint;
		/// <summary>
		/// カメラのターゲットチェンジ
		/// </summary>
		std::map<int, SceneVector> m_cameraChangeTarget;
		/// <summary>
		/// ウェイポイントをどれだけ使うか
		/// </summary>
		int m_useChangeTarget = 0;
		/// <summary>
		/// ターゲットチェンジをどれだけ使うか
		/// </summary>
		int m_useWayPoint = 0;
	};

public:
	/////////////////////////////////////////関数
	EventCamera();
	~EventCamera();
	/// <summary>
	/// スタート関数
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// 00000000を0A 0B 000C 000Dとして0Aが先頭番号
	/// 0BがBool、000Cがイージング割合、000Dが切り替えまでの秒数として
	/// 指定したイベントシーンの要素数のカメラ位置格納用mapに格納する処理。
	/// </summary>
	/// <param name="setPos"></param>
	/// <param name="setNum"></param>
	/// <param name="setScene"></param>
	/// <param name="updateMode"></param>
	void SetSceneCamAndTarPos(const Vector3& setPos, const int setNum
		, const EnEventScene setScene, ListUpdateMode updateMode);
	/// <summary>
	/// アップデート関数
	/// </summary>
	void Update();
	/// <summary>
	/// カメラ位置更新
	/// </summary>
	Vector3 CamPositionUpdate(std::map<int, SceneVector>::iterator setIterator
	, const float easingRatio,ListUpdateMode updateMode);
	/// <summary>
	/// カメラ位置配列更新
	/// </summary>
	void CamPositionListChange(std::map<int, SceneVector>::iterator &setIterator
	,ListUpdateMode updateMode);
	/// <summary>
	/// イージング処理
	/// </summary>
	Vector3 Easing(std::map<int, SceneVector>::iterator setIterator
	,const float easingRatio);
	/// <summary>
	/// 時間処理
	/// </summary>
	void Time(std::map<int, SceneVector>::iterator setIterator
	, ListUpdateMode updateMode);
	/////////////////////////////////////////メンバ変数
	/// <summary>
	/// レベルレンダー
	/// </summary>
	LevelRender m_camLevelRender;
	/////////////////////////////////////////変数
	/// <summary>
	/// g_3Dcameraにカメラの座標を送るための変数
	/// </summary>
	Vector3 m_sendCameraPosition = Vector3::Zero;
	/// <summary>
	/// g_3Dcameraにカメラのターゲットの位置を送るための変数
	/// </summary>
	Vector3 m_sendTargetPosition = Vector3::Zero;
	/// <summary>
	/// シーン
	/// </summary>
	OneScene m_scene[EnEventScene::en_SceneNum];
	/// <summary>
	/// 現在シーン
	/// </summary>
	EnEventScene m_sceneNow = EnEventScene::en_Scene_None;
	/// <summary>
	/// イベントを再生するかしないかのフラグ
	/// </summary>
	bool m_eventFlag = false;
	/// <summary>
	/// カメラ座標変更までの時間
	/// </summary>
	float m_posChangeTime = 0.0f;
	/// <summary>
	/// カメラターゲット座標変更までの時間
	/// </summary>
	float m_tarChangeTime = 0.0f;
	/// <summary>
	/// カメラ位置イージング時間
	/// </summary>
	float m_easingTimeCamPos = 0.0f;
	/// <summary>
	/// カメラターゲット位置イージング時間
	/// </summary>
	float m_easingTimeTarPos = 0.0f;
	/// <summary>
	/// カメラ位置のリストが終了しているかしていないか
	/// </summary>
	bool m_cameraPosListEnd = false;
	/// <summary>
	/// カメラターゲット位置のリストが終了しているかしていないか
    /// </summary>
	bool m_cameraTarListEnd = false;
	/// <summary>
	/// 現在処理中のカメラ位置のイテレーター
	/// </summary>
	std::map<int, SceneVector>::iterator m_camPosListIterator;
	/// <summary>
	/// 現在処理中のカメラターゲット位置のイテレーター
	/// </summary>
	std::map<int, SceneVector>::iterator m_camTarListIterator;
	/// <summary>
	/// カメラ位置イージング割合
	/// </summary>
	float m_easingPosRatio = 0.0f;
	/// <summary>
	/// カメラターゲット位置イージング割合
	/// </summary>
	float m_easingTarRatio = 0.0f;
	/// <summary>
	/// カメラ位置のイージングが終了しているかいないか
	/// </summary>
	bool m_camPoseasingEnd = false;
	/////////////////////////////////////////初期設定系統
	/// <summary>
	/// シーンスタート
	/// </summary>
	/// <param name="setScene"></param>
	void StartScene(const EnEventScene setScene)
	{
		m_sceneNow = setScene;

		m_posChangeTime = m_scene[setScene].m_cameraWayPoint[0].m_changeTime;

		m_tarChangeTime = m_scene[setScene].m_cameraChangeTarget[0].m_changeTime;

		m_camPosListIterator = m_scene[setScene].m_cameraWayPoint.begin();

		m_camTarListIterator = m_scene[setScene].m_cameraChangeTarget.begin();

		m_easingPosRatio = m_scene[setScene].m_cameraWayPoint[0].m_easingRatio;

		m_easingTarRatio = m_scene[setScene].m_cameraChangeTarget[0].m_easingRatio;

		m_eventFlag = true;

		return;
	}
	/// <summary>
	/// 現在のイテレーターに登録されているmap型配列に格納されている座標を取得する
	/// </summary>
	/// <param name="setIterator"></param>
	/// <returns></returns>
	Vector3 GetListPos(std::map<int, SceneVector>::iterator setIterator)
	{
		return setIterator->second.m_vector;
	}
	/// <summary>
	/// 現在のイテレーターに登録されているmap型配列から１つ進んだ要素に格納されている座標を取得する
	/// </summary>
	/// <param name="setIterator"></param>
	/// <param name="add"></param>
	/// <returns></returns>
	Vector3 GetListPos(std::map<int, SceneVector>::iterator setIterator, int add)
	{
		setIterator++;

		return setIterator->second.m_vector;
	}
	/// <summary>
	/// 現在のイテレーターに登録されているmap型配列から１つ進んだ要素に格納されている座標を取得する
	/// １つ進んだ要素がendあれば元の座標の取得する
	/// </summary>
	/// <param name="setIterator"></param>
	/// <param name="add"></param>
	/// <param name="updateMode"></param>
	/// <returns></returns>
	Vector3 GetListPos(std::map<int, SceneVector>::iterator setIterator, int add
		, ListUpdateMode updateMode)
	{
		std::map<int, SceneVector>::iterator subIterator = setIterator;

		setIterator++;

		switch (updateMode)
		{
		case EventCamera::en_ModePosition:
			if (setIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
			{
				return subIterator->second.m_vector;
			}
			break;
		case EventCamera::en_ModeTarget:
			if (setIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
			{
				return subIterator->second.m_vector;
			}
			break;
		}

		return setIterator->second.m_vector;
	}
	/// <summary>
	/// イベント再生中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsEvent()
	{
		return m_eventFlag;
	}
	/// <summary>
	/// カメラ位置のイテレーターのイージングがオンであるか
	/// </summary>
	bool IsCamPosIteratorEasing(std::map<int, SceneVector>::iterator setIterator
		,ListUpdateMode updateMode)
	{
		switch (updateMode)
		{
		case EventCamera::en_ModePosition:
			if (m_camPosListIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
			{
				return false;
			}
			break;
		case EventCamera::en_ModeTarget:
			if (m_camTarListIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
			{
				return false;
			}
			break;
		}

		return setIterator->second.isEasing;
	}

	bool IsIteratorEasingEnd(std::map<int, SceneVector>::iterator setIterator)
	{
		return setIterator->second.isEasingEnd;
	}
};

