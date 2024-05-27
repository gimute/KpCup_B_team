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
		en_SceneNum,
		en_Scene_None
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
		float m_easingTime = 0.0f;
		/// <summary>
		/// bool型
		/// </summary>
		bool isEasing = false;
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
	/// アップデート関数
	/// </summary>
	void Update();
	/// <summary>
	/// カメラ位置更新
	/// </summary>
	void CamPositionUpdate();
	/// <summary>
	/// カメラターゲット更新
	/// </summary>
	void CamTargetUpdate();
	/// <summary>
	/// カメラ位置配列更新
	/// </summary>
	void CamPositionListChange();
	/// <summary>
	/// カメラターゲット位置配列更新
	/// </summary>
	void CamTargetListChange();
	/// <summary>
	/// 時間処理
	/// </summary>
	void Time();
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
	/// 現在処理中のカメラ位置のイテレーター
	/// </summary>
	std::map<int, SceneVector>::iterator m_camPosListIterator;
	/// <summary>
	/// 現在処理中のカメラターゲット位置のイテレーター
	/// </summary>
	std::map<int, SceneVector>::iterator m_camTarListIterator;
	/////////////////////////////////////////初期設定系統
	void StartScene(const EnEventScene setScene)
	{
		m_sceneNow = setScene;

		m_posChangeTime = m_scene[setScene].m_cameraWayPoint[0].m_changeTime;

		m_tarChangeTime = m_scene[setScene].m_cameraChangeTarget[0].m_changeTime;

		m_camPosListIterator = m_scene[setScene].m_cameraWayPoint.begin();

		m_camTarListIterator = m_scene[setScene].m_cameraChangeTarget.begin();

		m_eventFlag = true;

		return;
	}
	/// <summary>
	/// レベルから位置と4桁の番号を取得して位置をそのまま、
	/// 4桁の番号を0A先頭番号、0Bbool、00C秒数として
	/// 指定したイベントシーンの要素数のカメラ位置格納用map
	/// に格納する処理。
	/// </summary>
	/// <param name="setPos"></param>
	/// <param name="setNum"></param>
	/// <param name="setScene"></param>
	void SetSceneCamPos(const Vector3& setPos, const int setNum
		, const EnEventScene setScene);
	/// <summary>
	/// レベルから位置と4桁の番号を取得して位置をそのまま、
	/// 4桁の番号を0A先頭番号、0Bbool、00C秒数として
	/// 指定したイベントシーンの要素数のカメラターゲット位置格納用map
	/// に格納する処理。
	/// </summary>
	/// <param name="setPos"></param>
	/// <param name="setNum"></param>
	/// <param name="setScene"></param>
	void SetSceneTarget(const Vector3& setPos, const int setNum
		, const EnEventScene setScene);
	/// <summary>
	/// イベント再生中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsEvent()
	{
		return m_eventFlag;
	}
	bool IsIteratorEasing(std::map<int, SceneVector>::iterator setIterator)
	{
		return setIterator->second.isEasing;
	}
};

