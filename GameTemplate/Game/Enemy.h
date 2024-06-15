#pragma once

#include "EnemyAttackPoint.h"

class Game;
class Player;
class EnemyHpUi;
class Bullet;

class Enemy : public IGameObject
{
public:
	//行動ステート
	enum EnEnemyState {
		enEnemyState_Idle,				//待機
		enEnemyState_Chase,				//追跡
		enEnemyState_Attack,			//攻撃
		enEnemyState_Stand,				//構え
		enEnemyState_ReceiveDamage,		//被ダメージ
		enEnemyState_Down				//ダウン
	};
	//攻撃頻度のステート
	enum EnEnemyAttackSpeed {
		en_FrequencyHigh,
		en_FrequencyFew,
	};
	//攻撃段階のステート
	enum EnEnemyAttackStep {
		en_stanceStep,
		en_shotStep,
		en_noneStep,
	};
	//アニメーション類/////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_Chase,			//追跡
		enAnimationClip_Attack,			//攻撃
		enAnimationClip_ShotStandby,    //攻撃待機
		enAnimationClip_PostureWalk,	//構え歩き
		enAnimationClip_Damage,			//被ダメージモーション
		enAnimationClip_Num,			//アニメーション数
	};
	void PlayAnimation();
	//ステート遷移関数///////////////////////////////////////////
	void ManageState();							//遷移処理
	void ProcessCommonStateTransition();		//共通遷移処理
	void ProcessIdleStateTransition();			//待機遷移
	void ProcessChaseStateTransition();			//追跡遷移
	void ProcessAttackStateTransition();		//攻撃遷移
	void ProcessStandStateTransition();			//構え遷移
	void ProcessReceiveDamageStateTransition();	//被ダメージ遷移
	/////////////////////////////////////////////////////////////

	Enemy() {};
	~Enemy();
	bool Start();						//アップデート
	void Update();                                         //アップデート
	//モデルの回転をする関数
	void Rotation();                                       //回転
	void Collision();										//本体の当たり判定
	void Chase();										   //追跡ステートの時に行う処理
	void Attack();										   //攻撃ステートの時に行う処理
	void Stand();											//構えステートの時に行う処理

	//プレイヤーが視界内に居るか確かめる関数
	bool SearchPlayer();

	//プレイヤーが一定範囲内に居るか確かめる関数
	const bool SearchChaseDistance() const;

	//プレイヤーが攻撃範囲内に居るか確かめる関数
	const bool SearchAttackDistance() const;

	//アニメーションイベント
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	//モデルの描画処理
	void Render(RenderContext& rc);

	//所持しているアタックポイントを解放する
	void ReleaseAttackPoint();

	//自分の座標から指定された座標までに壁があるか調べる関数
	//あった場合ture、なかった場合falseが返される
	bool WallCheck(const Vector3 position);

	//周囲のエネミーにchase状態を伝播させる
	void AroundEnemyTransitionToChase();
	//AroundEnemyTransitionToChase関数で呼び出す
	//伝播条件を満たしたエネミーのステートをchaseにする関数
	void TransitionToChaseTest(Enemy* enemy);

	///////////////////////////////////////////////////////////
	//初期設定系統
	//メンバ関数宣言
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	Bullet* m_bullet = nullptr;
	SoundSource* m_bgm = nullptr;
	

	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	EnEnemyAttackSpeed m_enemyAttackSpeed = en_FrequencyFew;			//エネミーの攻撃頻度の分類
	EnEnemyAttackStep m_enemyAttackStep = en_noneStep;
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	Vector3 m_movespeed;									//移動速度
	Vector3 m_position;										//座標
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	ModelRender m_modelRender;	//モデルレンダー
	CharacterController m_charaCon;        //キャラコン
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	int m_hp = 4;                           //HP
	int m_Vectornum = 0;					//配列のナンバー
	float m_idleTimer= 0.0f;								//待機時間タイマー。
	float m_chaseTimer = 0.0f;								//追跡時間タイマー。
	float m_attackTimer = 0.0f;
	float attackTime = 3.0f;
	bool m_shotBool = false;

private:
	EnEnemyState m_oldEnemyState;          //被弾時に被ダメージステートから元のステートに戻るために、記憶しておく変数
	EnemyAttackPoint::AttackPoint* m_AttackPoint = nullptr;		//アタックポイント構造体のポインタ
	SphereCollider m_sphereCollider;		//プレイヤーと自分の間に壁があるか判定するのに使うコライダー

	bool m_gunmove = false;
};

