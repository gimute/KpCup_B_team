#pragma once

#define WAYPOINT_MAX 10
#define CHANGE_TARGET_MAX 10

class EventCamera :public IGameObject
{
	enum EnEventScene {
		en_Scene1_Door,
		en_SceneNum,
		en_Scene_None
	};

	struct OneScene{
		/// <summary>
		/// カメラを動かす際のウェイポイント
		/// </summary>
		Vector3 m_cameraWayPoint[WAYPOINT_MAX];
		/// <summary>
		/// カメラのターゲットチェンジ
		/// </summary>
		Vector3 m_cameraChangeTarget[CHANGE_TARGET_MAX];
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
	/////////////////////////////////////////メンバ変数
	/////////////////////////////////////////変数
	/// <summary>
	/// シーン
	/// </summary>
	OneScene m_scene[EnEventScene::en_SceneNum];
	/// <summary>
	/// 現在シーン
	/// </summary>
	EnEventScene m_sceneNow = EnEventScene::en_Scene_None;
	/////////////////////////////////////////初期設定系統
	int GetSceneUseWayPoint(const EnEventScene setScene)
	{
		return m_scene[setScene].m_useWayPoint;
	}
	/// <summary>
	/// シーンセット
	/// </summary>
	/// <param name="setScene"></param>
	void SetSceneNow(const EnEventScene setScene)
	{
		m_sceneNow = setScene;
		return;
	}
	/// <summary>
	/// シーンを入れて初期位置をセット
	/// </summary>
	/// <param name="setScene"></param>
	/// <param name="pos"></param>
	void SetScenePosInstance(const EnEventScene setScene
		,const Vector3 (&setVector)[5])
	{
		m_scene[setScene].m_useWayPoint = sizeof(Vector3) / sizeof((&setVector)[5]);

		int useNum = GetSceneUseWayPoint(setScene);
		
		for (int i = 0; i < 10; i++)
		{
			m_scene[setScene].m_cameraWayPoint[i] = setVector[i];
		}
	}
	/// <summary>
	/// シーンを入れて初期ターゲットをセット
	/// </summary>
	/// <param name="setScene"></param>
	/// <param name="pos"></param>
	void SetSceneTargetInstance(const EnEventScene setScene
		,const Vector3& setVector)
	{

	}
};

