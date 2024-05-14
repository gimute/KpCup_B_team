#include "stdafx.h"
#include "Enemy.h"
#include "EnemyAttackPoint.h"


namespace
{
	const float TO_ATTACK_POINT_DISTANSE = 200.0f;	//基準となる座標からアタックポイントまでの距離
}

EnemyAttackPoint::EnemyAttackPoint()
{
	//アタックポイントのナンバーを初期化する
	for (int i = 0; i < ATTACK_POINT_NUM; i++)
	{
		m_AttackPointList[i].m_number = i;
	}
}

EnemyAttackPoint::~EnemyAttackPoint()
{

}

void EnemyAttackPoint::Update(Vector3 position)
{
	//アタックポイントを指定された座標の周囲に円状に配置する

	//ポイントを設置する方向
	Vector3 direction = g_vec3Front;

	//ポイントを設置する方向を回転させるクオータニオン
	Quaternion directionRot = g_quatIdentity;

	//360度をエネミーアタックポイント配列の要素数で割って等間隔で置くための角度を設定する
	directionRot.SetRotationDegY(360.0f / ATTACK_POINT_NUM);

	//ポイントの配列を回して座標を設定する
	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		//ポイントの座標を設定する
		attackPoint.m_position = position + direction * TO_ATTACK_POINT_DISTANSE;

		//設置する方向を回転
		directionRot.Apply(direction);
	}
}

EnemyAttackPoint::AttackPoint* EnemyAttackPoint::GetNearAttackPoint(Vector3 position)
{
	//アタックポイントが使用可能でなければnullptrを返す
	if (IsUsableAttackPoint() == false)
	{
		return nullptr;
	}

	//距離比較用の変数
	float distance = 10000.0f;	//	最初は極端に大きい値にしておく

	//一番近いアタックポイントのアドレスを入れておくポインタ
	AttackPoint* tmp = nullptr;

	//アタックポイントの配列を回す
	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		//アタックポイントが使用中なら処理を飛ばす
		if (attackPoint.m_use == true)
		{
			continue;
		}

		//アタックポイントとの距離を求める
		float compareDistance = (attackPoint.m_position - position).Length();

		//保持しているアタックポイントとの距離より、今回出したアタックポイントとの距離の方が小さければ
		if (distance > compareDistance)
		{
			//保持しているアタックポイントとの距離を、今回出したアタックポイントとの距離に変更し
			distance = compareDistance;
			//そのアタックポイントのアドレスを保存する
			tmp = &attackPoint;
		}
	}

	//一番近いアタックポイントのアドレスを返す
	return tmp;
}

bool EnemyAttackPoint::IsUsableAttackPoint()
{
	//アックポイント使用数が一定数より少なければ
	if (m_useAttackPointNum < ATTACK_POINT_USE_LIMIT)
	{
		//使用可能
		return true;
	}
	else
	{
		//使用不可
		return false;
	}
}

void EnemyAttackPoint::UseAttackPoint(int number, Enemy* enemy)
{
	//ポイントが使用中なら何もせず返す。
	if (m_AttackPointList[number].m_use == true)
	{
		return;
	}

	//アタックポイントを使用中にする。
	m_AttackPointList[number].m_use = true;
	//アタックポイントを保持しているエネミーを登録する
	m_AttackPointList[number].m_useEnemy = enemy;
	//使用中のアタックポイントカウントを増やす
	m_useAttackPointNum++;
}

void EnemyAttackPoint::ReleaseAttackPoint(int number, Enemy* enemy)
{
	//ポイントを確保しているエネミーと引数で渡されたエネミーが同じでなければ何もせず返す
	if (m_AttackPointList[number].m_useEnemy != enemy)
	{
		return;
	}
	
	//アタックポイントを未使用にする。
	m_AttackPointList[number].m_use = false;
	//アタックポイントを保持しているエネミーの登録を解除する
	m_AttackPointList[number].m_useEnemy = nullptr;
	//使用中のアタックポイントカウントを減らす
	m_useAttackPointNum--;
}

