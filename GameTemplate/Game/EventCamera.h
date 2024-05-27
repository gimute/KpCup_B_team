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

	struct SceneVector
	{
		/// <summary>
		/// vector3型
		/// </summary>
		Vector3 m_vector = Vector3::Zero;
		/// <summary>
		/// float型
		/// </summary>
		float m_changeTime = 0.0f;
		/// <summary>
		/// bool型
		/// </summary>
		bool isEasing = false;
	};

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
	/////////////////////////////////////////初期設定系統
	int GetSceneUseWayPoint(const EnEventScene setScene)
	{
		return m_scene[setScene].m_useWayPoint;
	}
	/// <summary>
	/// シーンセット
	/// </summary>
	/// <param name="setScene"></param>
	void StartScene(const EnEventScene setScene)
	{
		m_sceneNow = setScene;
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
};

