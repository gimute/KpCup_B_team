#pragma once

#define ATTACK_POINT_NUM 15		//アタックポイントの数

//エネミーの攻撃ポイント
//このポイントを確保できているエネミーのみ攻撃可能
class EnemyAttackPoint
{
public:

	//アタックポイントの構造体
	struct AttackPoint
	{
		Vector3 m_position;				//アタックポイントの座標
		int m_number;					//アタックポイントの要素番号
		bool m_use = false;				//使用中か
		Enemy* m_useEnemy = nullptr;	//アタックポイントを使用しているエネミー
	};

	EnemyAttackPoint();
	~EnemyAttackPoint();

	
	//アタックポイントの座標を更新する関数
	//引数で送られた座標を基準に、円状にポイントを設置する
	void Update(Vector3 position);

	//近くにある使用可能なアタックポイントのアドレスを取得する関数
	AttackPoint* GetNearAttackPoint(Vector3 position);

	//アタックポイントを確保する関数
	//確保するアタックポイントのナンバーと
	//登録用に、ポイントを確保するエネミーのアドレスを受け取る
	void GetAttackPoint(int number, Enemy* enemy);

	//アタックポイントを解放する関数
	//解放するアックポイントのナンバーと
	//確認用に、ポイントを確保しているエネミーのアドレスを受け取る
	void ReleaseAttackPoint(int number, Enemy* enemy);
	

	//アタックポイントが利用可能かどうか
	//現状、アタックポイントを使っているエネミーが一定数居る場合利用を制限するようにしている
	bool IsUsableAttackPoint();

private:
	AttackPoint m_AttackPointList[ATTACK_POINT_NUM];
	int m_useAttackPointNum = 0;
};

