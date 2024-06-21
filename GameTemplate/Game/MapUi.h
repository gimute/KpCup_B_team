#pragma once

#include "Game.h"
#include "Enemy.h"
#include "Player.h"

class Game;
class Enemy;
class Player;

class MapUi : public IGameObject
{
public:
	/////////////////////////////////////////�\����
	/// <summary>
	/// �G�l�~�[�̃|�C���^Ui
	/// </summary>
	struct MapUi_EnemyPoint
	{
		/// <summary>
		/// �G�l�~�[�|�C���^�X�v���C�g
		/// </summary>
		SpriteRender m_mapUiEnemyPoint;
		/// <summary>
		/// �G�l�~�[�N���X�̃C���X�^���X�i�[�ϐ�
		/// </summary>
		Enemy* m_enemy = nullptr;
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		MapUi_EnemyPoint()
		{
			m_mapUiEnemyPoint.Init("Assets/modelData/ui_map/MapEnemyPoint.DDS", 30.0f, 30.0f);
		}
		/// <summary>
		/// �G�l�~�[�̃C���X�^���X��o�^
		/// </summary>
		void SetEnemyInstance(Enemy* enemyinstance)
		{
			m_enemy = enemyinstance;
		}
	};
	/////////////////////////////////////////�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MapUi();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MapUi();
	/// <summary>
	/// �X�^�[�g�֐�
	/// </summary>
	bool Start();
	/// <summary>
	/// �A�b�v�f�[�g�֐�
	/// </summary>
	void Update();
	/// <summary>
	/// �}�b�v�v���C���[�|�C���^�̍X�V
	/// </summary>
	void MapPlayerPointUpdate();
	/// <summary>
	/// �}�b�v�G�l�~�[�|�C���^���쐬����
	/// </summary>
	void CreateMapEnemyPoint();
	/// <summary>
	/// �`�揈���֐�
	/// </summary>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�|�C���^�ϐ�
	/// <summary>
	/// Game�N���X�̃C���X�^���X�i�[�ϐ�
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// Player�N���X�̃C���X�^���X�i�[�ϐ�
	/// </summary>
	Player* m_player = nullptr;
	/////////////////////////////////////////�ϐ�
	/////////////////////////////////////////�X�v���C�g
	/// <summary>
	/// �}�b�v�t���[���X�v���C�g
	/// </summary>
	SpriteRender m_mapUiFrame;
	/// <summary>
	/// �}�b�v�x�[�X�X�v���C�g
	/// </summary>
	SpriteRender m_mapUiBase;
	/// <summary>
	/// �}�b�v�v���C���[�|�C���^�X�v���C�g
	/// </summary>
	SpriteRender m_mapUiPlayerPoint;
	/////////////////////////////////////////�z��
	/// <summary>
	/// �G�l�~�[�|�C���^�X�v���C�g�̍\���̂̃A�h���X���i�[����map
	/// </summary>
	std::map<int, MapUi_EnemyPoint*> m_mapUiEnemyPointList;
	/////////////////////////////////////////�����o�֐�
};

