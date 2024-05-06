#include "stdafx.h"
#include "Enemy.h"
#include "EnemyAttackPoint.h"

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

	//基準の座標からポイントまでの距離
	float lenge = 200.0f;

	//ポイントを設置する方向
	Vector3 direction = g_vec3Front;

	//ポイントを設置する方向を回転させるクオータニオン
	Quaternion directionRot = g_quatIdentity;

	//360度をエネミーアタックポイント配列の要素数で割って等間隔で置くための角度を設定する
	directionRot.SetRotationDegY(360.0f / ENEMY_ATTACK_POINT_NUM);

	//ポイントの配列を回して座標を設定する
	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		//ポイントの座標を設定する
		attackPoint.m_position = position + direction * lenge;

		//設置する方向を回転
		directionRot.Apply(direction);
	}
}

EnemyAttackPoint::AttackPoint* EnemyAttackPoint::GetNearAttackPoint(Vector3 position)
{
	//アタックポイントが使用可能でなければnullptrを返す
	if (IsUsableAttackPoint())
	{
		return nullptr;
	}

	//距離比較用の変数
	float distance = 10000.0f;	//	最初は極端に大きい値にしておく

	//一番近いアタックポイントのアドレスを入れておくポインタ
	AttackPoint* tmp = nullptr;

	//使用中のアタックポイントの数をカウントする変数
	int useCount = 0;


	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		if (attackPoint.m_use == true)
		{
			continue;
		}

		float distance2 = (attackPoint.m_position - position).Length();

		//diffの値よりアタックポイントとの距離の方が小さければ
		if (distance > distance2)
		{
			//diffの値を近い方のアタックポイントとの距離に変更し
			distance = distance2;
			//そのアタックポイントのアドレスを保存する
			tmp = &attackPoint;
		}
	}

	//一番近いアタックポイントのアドレスを返す
	return tmp;
}

bool EnemyAttackPoint::IsUsableAttackPoint()
{
	if (m_useAttackPointNum <= 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void EnemyAttackPoint::GetAttackPoint(int number, Enemy* enemy)
{
	//ポイントが使用中なら何もせず返す
	if (m_AttackPointList[number].m_use == true)
	{
		return;
	}

	m_AttackPointList[number].m_use = true;
	m_AttackPointList[number].m_useEnemy = enemy;

	m_useAttackPointNum++;
}

void EnemyAttackPoint::ReleaseAttackPoint(int number, Enemy* enemy)
{
	//ポイントを確保しているエネミーと引数で渡されたエネミーが同じでなければ何もせず返す
	if (m_AttackPointList[number].m_useEnemy != enemy)
	{
		return;
	}
	
	m_AttackPointList[number].m_use = false;
	m_AttackPointList[number].m_useEnemy = nullptr;

	m_useAttackPointNum--;
}

//Vector3 EnemyAttackPoint::GetNearAttackPointPosition()
//{
//	//距離比較用の変数
//	float distance = 10000.0f;	//	最初は極端に大きい値にしておく
//
//	//一番近いアタックポイントのアドレスを入れておくポインタ
//	EnemyAttackPoint* tmp = nullptr;
//
//	//使用中のアタックポイントの数をカウントする変数
//	int useCount = 0;
//
//	//一番近い未使用のアタックポイントを探す
//	for (int i = 0; i < ENEMY_ATTACK_POINT_NUM; i++)
//	{
//		//アタックポイントが使用中なら
//		if (m_enemyAttackPointList[i].m_use == true)
//		{
//			//使用中アタックポイントのカウントを増やす
//			useCount++;
//			//使用中のポイントが一定数を超えていることがわかったら
//			if (useCount >= 5)
//			{
//				//nullptrを返す
//				return nullptr;
//			}
//			//超えてなければ
//			else
//			{
//				continue;
//			}
//
//		}
//
//		//diffの値よりアタックポイントとの距離の方が小さければ
//		if (diff > (m_enemyAttackPointList[i].m_position - pos).Length())
//		{
//			//diffの値を近い方のアタックポイントとの距離に変更し
//			diff = (m_enemyAttackPointList[i].m_position - pos).Length();
//			//そのアタックポイントのアドレスを保存する
//			tmp = &m_enemyAttackPointList[i];
//		}
//	}
//
//	//一番近いアタックポイントのアドレスを返す
//	return tmp;
//}

