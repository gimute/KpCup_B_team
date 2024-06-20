#pragma once

#include <memory>

class EventCamera;

/// <summary>
/// イベント時にモデルを動かす処理クラス
/// </summary>
class EventModel : public IGameObject
{
public:
	/// <summary>
	/// 動く位置座標の構造体
	/// </summary>
	struct SceneSettingInfo {
		/// <summary>
		/// 座標
		/// </summary>
		Vector3 m_position = Vector3::Zero;
		/// <summary>
		/// イージング割合
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
		/// <summary>
		/// 切り替え条件の付与
		/// </summary>
		bool isSwitchingCon = false;
		/// <summary>
		/// 動かすモデルのナンバー
		/// </summary>
		int moveModelNumber = -1;
	};
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
		/// このモデルで使用するアニメーション
		/// </summary>
		std::unique_ptr<AnimationClip[]> m_animationclips;
		/// <summary>
		/// このモデルで使用するアニメーションの数
		/// </summary>
		int m_useAnimationclipsNumber = 0;
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
			m_animationclips = std::move(m_animationclips);

			//int型の引数で配列を動的に確保、スマートポインタにメモリの所有権を委ねる
			m_animationclips.reset(new AnimationClip[listNum]);

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
		void RegistrationAnimation(int listNum,const char* filePath,bool loopFlag)
		{
			//引数のint型の数値の配列の要素番号の場所に引数のファイルパスを登録
			m_animationclips[listNum].Load(filePath);

			//引数のbool型でループフラグを設定
			m_animationclips[listNum].SetLoopFlag(loopFlag);

			m_useAnimationclipsNumber++;
		}
		/// <summary>
		/// モデルレンダーに登録を確定
		/// </summary>
		void RegistrationConfirmed()
		{
			//モデルレンダーに登録を確定する
			m_modelRender.Init(m_sceneFilePath, m_animationclips.get(), m_useAnimationclipsNumber);
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
	/// <summary>
	/// シーンモデルリストにモデルとアニメーションを登録する処理
	/// </summary>
	/// <param name="nowMode"></param>
	void RegistrationSceneModel();
	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////ポインタ変数
	/// <summary>
	/// レベルレンダー
	/// </summary>
	LevelRender m_camLevelRender;
	/// <summary>
	/// イベントカメラのインスタンス取得用変数
	/// </summary>
	EventCamera* m_eventCamPtr = nullptr;
	/////////////////////////////////////////メンバ変数
	/// <summary>
	/// モデルの登録配列
	/// </summary>
	std::map<int,SceneModel*> m_sceneModelList;
	std::map<int, SceneModel*>::iterator testIter;
	/// <summary>
	/// イベントを再生するかしないかのフラグ
	/// </summary>
	bool m_eventFlag = false;
	/////////////////////////////////////////メンバ関数
};

