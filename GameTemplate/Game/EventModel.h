#pragma once

#include <memory>

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
		std::unique_ptr<AnimationClip[]> m_animationclips;
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
		/// <param name="listNum"></param>
		/// <param name="tkmfilePath"></param>
		SceneModel(int listNum,const char* tkmfilePath)
		{
			//int型の引数で配列を動的に確保、スマートポインタにメモリの所有権を委ねる
			m_animationclips = std::make_unique<AnimationClip[]>(listNum);

			//引数のファイルパスを登録
			m_sceneFilePath = tkmfilePath;
		}
		/// <summary>
		/// デストラクタ
		/// </summary>
		~SceneModel()
		{
			//スマートポインタに任せてるから今は何も書いてない
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
		/// <summary>
		/// モデルレンダーに登録を確定
		/// </summary>
		void RegistrationConfirmed()
		{
			//登録を確定する
			m_modelRender.Init(m_sceneFilePath, m_animationclips.get(), m_useAnimationclips);
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
	/// <summary>
	/// アップデート関数
	/// </summary>
	void Update();
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
	/// <summary>
	/// モデルの配列
	/// </summary>
	std::list<SceneModel> m_sceneModelMapList;
	/// <summary>
	/// イベントを再生するかしないかのフラグ
	/// </summary>
	bool m_eventFlag = false;
	/////////////////////////////////////////初期設定系統
};

