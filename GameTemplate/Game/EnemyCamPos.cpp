#include "stdafx.h"
#include "EnemyCamPos.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"

EnemyCamPos::EnemyCamPos()
{
	for (int i = 0; i < LIST_MAX; i++){
		m_camForwardList[i].m_num = i;
	}
}

EnemyCamPos::~EnemyCamPos()
{

}

float EnemyCamPos::EnemyCamPosConfirmation(Enemy* enemy)
{
	m_game = FindGO<Game>("game");
	m_camera = FindGO<GameCamera>("gamecamera");
	m_player = FindGO<Player>("player");

	//Enemyの座標を取得
	Vector3 enemypos = enemy->m_position;
	int Listnum = enemy->m_Vectornum;

		//取得した座標がカメラ正面にあるかどうかを調べる
	if (AngleCheck(enemypos) /*&& EnemyStateIsAttack(Listnum)*/)
	{
		//取得した座標がカメラ正面にあったら次は
		//m_camForwardListに未登録の要素があるかどうかを調べる
		if (ListCheck())
		{
			//未登録の場所があればその要素に座標のEnemyを登録する
			Registration(Listnum);
		}
		else
		{
			//未登録の場所がなかった場合は登録中のEnemyの中で一番
			//カメラから遠いEnemyを調べてそのEnemyが現在探索中のEnemyよりも
			//カメラから近い場所にあるならそのm_camForwardListの要素のEnemyを
			//探索中のEnemyと入れ替える
			CompareNear(Listnum, enemypos);
		}
	}
	else
	{
		//取得した座標がカメラ正面になかった場合は
		//取得した座標のEnemyの攻撃頻度を"低"にする
		CamOut(Listnum,enemypos);
	}

	if (enemy->m_enemyAttack == Enemy::en_FrequencyHigh)
	{
		return 3.0f;
	}
	else
	{
		return 10.0f;
	}
}

bool EnemyCamPos::ListCheck()
{
	//構造体配列を回す
	for (EnemyCamForward& ListPoint : m_camForwardList)
	{
		//配列に空きがあれば
		if (ListPoint.m_enemyRegist == false)
		{
			//その開いている配列の番号を変数に登録して
			m_emptyPoint = ListPoint.m_num;
			//trueを返す
			return true;
		}
	}

	//開いている場所が無ければfalseを返す
	return false;
}

bool EnemyCamPos::ListCheck(int ListNum)
{
	//構造体配列を回す
	for (EnemyCamForward& ListPoint : m_camForwardList)
	{
		//渡されたEnemyの配列番号と同じ番号が
		//m_camForwardListにあれば
		if (ListPoint.m_registNum == ListNum)
		{
			//その配列の番号を変数に登録して
			m_emptyPoint = ListPoint.m_num;
			//trueを返す
			return true;
		}
	}

	//開いている場所が無ければfalseを返す
	return false;
}

bool EnemyCamPos::EnemyStateIsAttack(int ListNum)
{
	if (m_game->m_EnemyList[ListNum]->m_enemystate
		== Enemy::enEnemyState_Attack)
	{
		return true;
	}
	return false;
}

bool EnemyCamPos::AngleCheck(const Vector3& position)
{
	Vector3 m_camForward = g_camera3D->GetForward();
	Vector3 m_camPosition = g_camera3D->GetPosition();

	Vector3 diff = position - m_camPosition;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_camForward));
	if (Math::PI * 0.15f <= fabsf(angle))
	{
		return false;
	}

	return true;
}

void EnemyCamPos::CompareNear(int ListNum,const Vector3& enemypos)
{
	//カメラの現在座標を取得
	Vector3 CamPos = g_camera3D->GetPosition();
	//水平なベクトルにしたいためy値を0に
	CamPos.y = 0.0f;

	//カメラの正面ベクトルを取得
	Vector3 CamForward = g_camera3D->GetForward();
	//水平なベクトル(以下略)
	CamForward.y = 0.0f;
	//*400してベクトルを伸ばす
	CamForward *= 400;

	//位置+正面*400でカメラの位置から正面*400の座標を出す
	Vector3 CamForwardAdvanceVec = CamPos + CamForward;

	//現在探索中のEnemyの座標を取得
	Vector3 changeEnemyPos = enemypos;

	//取得したEnemyの座標とカメラ正面*400の現在座標を計算して
	//プレイヤーからEnemyの座標へ向かうベクトルを出す
	Vector3 diffChange = changeEnemyPos - CamForwardAdvanceVec;

	//m_camForwardListの中でどれが一番カメラ正面*400の現在座標から遠い
	//位置のEnemyを格納しているかm_camForwardListの要素番号
	//を取得する変数を作る
	int MaxListNum = -1;

	//比べる用に変数を作成
	Vector3 diffC;

	//m_camForwardListの配列の中で一番遠いEnemyを登録している要素番号を調べたいので
	//for分の中でm_camForwardListのiとi + 1で座標を比べる
	//3回繰り返すと + 1回分あふれるのでm_camForwardListの全体要素数から - 1回繰り返す
	for (int i = 0; i < LIST_MAX - 1; i++)
	{

		//m_camForwardListのiの要素から登録されているEnemyの座標を取得する
		Vector3 compareEnemyA = m_game->GetEnemyListPos(m_camForwardList[i].m_registNum);

		//取得したEnemyの座標とカメラ正面*400の現在座標を計算して
		//カメラ正面*400の現在座標からEnemyの座標へ向かうベクトルを出す
		Vector3 diffA = compareEnemyA - CamForwardAdvanceVec;

		//m_camForwardListのi + 1の要素から登録されているEnemyの座標を取得する
		Vector3 compareEnemyB = m_game->GetEnemyListPos(m_camForwardList[i + 1].m_registNum);

		//取得したEnemyの座標とカメラ正面*400の現在座標を計算して
		//カメラ正面*400の現在座標からEnemyの座標へ向かうベクトルを出す
		Vector3 diffB = compareEnemyB - CamForwardAdvanceVec;

		//出された2つのベクトルを比べて長いベクトルの方をdiffCとも比べて
		//それよりも大きければその要素番号を格納する
		if (diffA.Length() < diffB.Length())
		{
			if (diffC.Length() < diffB.Length())
			{
				MaxListNum = m_camForwardList[i + 1].m_num;
				diffC = diffB;
			}
		}
		else
		{
			if (diffC.Length() < diffA.Length())
			{
				MaxListNum = m_camForwardList[i].m_num;
				diffC = diffA;
			}
		}
	}

	if (diffC.Length() >= diffChange.Length())
	{
		if (!ListCheck(ListNum))
		{
			m_camForwardList[MaxListNum].m_registNum = ListNum;

			m_game->m_EnemyList[m_camForwardList[MaxListNum].m_registNum]
				->SetEnemyAttackState(Enemy::en_FrequencyHigh);
			return;
		}
	}
	else
	{
		m_game->m_EnemyList[ListNum]
			->SetEnemyAttackState(Enemy::en_FrequencyFew);
	}

	return;
}

void EnemyCamPos::CamOut(int ListNum,const Vector3 enemypos)
{
	//もしm_camForwardList内に登録されているEnemyの配列番号と
	//現在の探索場所が同じなら登録を解除する
	if (ListCheck(ListNum))
	{
		m_camForwardList[m_emptyPoint].m_registNum = -1;
		m_camForwardList[m_emptyPoint].m_enemyRegist = false;
	}

	if (!AngleCheck(enemypos))
	{
		//攻撃頻度を"低"にする処理
		m_game->m_EnemyList[ListNum]->SetEnemyAttackState(Enemy::en_FrequencyFew);
	}
	return;
}

void EnemyCamPos::Registration(int ListNum)
{
	if (ListCheck(ListNum))
	{
		return;
	}

	m_camForwardList[m_emptyPoint].m_registNum = ListNum;
	m_camForwardList[m_emptyPoint].m_enemyRegist = true;
	m_game->m_EnemyList[ListNum]->SetEnemyAttackState(Enemy::en_FrequencyHigh);
}
