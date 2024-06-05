#pragma once

class EventCamera;

class EventModel : public IGameObject
{
public:
	/// <summary>
	/// 1シーンで使用するモデル、アニメーションの構造体
	/// </summary>
	struct SceneModel
	{
		/// <summary>
		/// このシーンで使用するモデル
		/// </summary>
		ModelRender m_modelRender;
		/// <summary>
		/// このシーンで使用するアニメーション
		/// </summary>
		AnimationClip* m_animationclips;
		/// <summary>
		/// このシーンで使用するアニメーションの数
		/// </summary>
		int m_useAnimationclips = 0;
		/// <summary>
		/// このシーンで使用するモデルのファイルパス
		/// </summary>
		const char* m_sceneFilePath;
		/// <summary>
		/// コンストラクタ
		/// </summary>
		SceneModel(int listNum,const char* tkmfilePath)
		{
			//int型の引数で配列を動的に確保
			m_animationclips = new AnimationClip[listNum];

			//引数のファイルパスを登録
			m_sceneFilePath = tkmfilePath;
		}
		/// <summary>
		/// 第一引数のint型の数のアニメーションクリップの要素番号の場所に
		/// 第二引数のファイルパスを登録し、第三引数のbool型を、
		/// アニメーションのループフラグとしてアニメーションクリップに登録する。
		/// </summary>
		/// <param name="listNum"></param>
		/// <param name="filePath"></param>
		/// <param name="loopFlag"></param>
		void AnimationRegistration(int listNum,const char* filePath,bool loopFlag)
		{
			//引数のint型の数値の配列の要素番号の場所に引数のファイルパスを登録
			m_animationclips[listNum].Load(filePath);

			//引数のbool型でループフラグを設定
			m_animationclips[listNum].SetLoopFlag(loopFlag);
		}
	};

public:
	/////////////////////////////////////////関数
	EventModel();
	~EventModel();
	/// <summary>
	/// スタート関数
	/// </summary>
	/// <returns></returns>
	bool Start();
	/////////////////////////////////////////メンバ変数
	/// <summary>
	/// レベルレンダー
	/// </summary>
	LevelRender m_camLevelRender;
	/// <summary>
	/// イベントカメラのインスタンス取得用変数
	/// </summary>
	EventCamera* m_eventCam = nullptr;
	/////////////////////////////////////////変数

	/////////////////////////////////////////初期設定系統
};

