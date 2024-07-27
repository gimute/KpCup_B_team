#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"
#include "physics/BoxCollider.h"
//�Q�l�F�A�c�搶�̃Q�[������

struct CellBinary{
	Vector3		vecTexPos[3];	//�Z���̒��_�̃|�W�V����
	int			lincCellNo[3];	//�Z���ɗאڂ��Ă���Z��
	Vector3		cellCenterPos;	//�Z���̐^�񒆂̃|�W�V����
};

//�Z���N���X
struct Cell
{
	enum State {
		State_NotResearch,
		State_Opened,
		State_Closed,
	};
	Vector3		vertexPos[3];	//�Z���̒��_�̃|�W�V����
	Cell*		linkCells[3];	//�Z���ɗאڂ��Ă���Z��
	Vector3		cellCenterPos;	//�Z���̐^�񒆂̃|�W�V����
	int			linkMax = 0;	//�אڂ��Ă���Z���̌�
	float		costFromStart;
	float		costToGoal;
	Cell*		parentCell;
	State		state = State_NotResearch;
};

class Game;
class BackGround;

class Navimesh : public IGameObject
{
public: 
	Navimesh();
	~Navimesh();
	bool Start() override { return true; };
	void Update() override;
	void Init(BackGround* floor);
	void Create(const Model& model);
	const std::vector<Cell*>& GetCell() const
	{
		
	}
private:
	/// <summary>
	/// �Z���̗אڏ����\�z����B
	/// </summary>
	/// <param name="startCellNo">�J�n�Z���̔ԍ��B</param>
	/// <param name="endCellNo">�I���Z���̔ԍ��B</param>
	void BuildLinkCellInfo(int startCellNo, int endCellNo);
	/// <summary>
	/// ���f���f�[�^����i�r���b�V���\�z�p�̒��_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���\�z����B
	/// </summary>
	/// <param name="model"></param>
	void BuildVertexBufferAndIndexBufferFrom(const Model& model);
	/// <summary>
	/// �Z�����\�z����B
	/// </summary>
	void BuildCells();
private:
	//typedef�͊����̌^���ɐV�������O��t����B
	typedef std::vector<Vector3>					VertexBuffer;	//���_�o�b�t�@�B
	typedef std::vector<unsigned int>				IndexBuffer;	//�C���f�b�N�X�o�b�t�@�B
	//unique_ptr�̓R�s�[�s�\�ȃN���X�B
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//���_�o�b�t�@�̔z��B
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B

	BoxCollider			m_collider;						//�Z���p�̃J�v�Z���R���C�_�[�B
	const float			COLLIDER_EXPANSION = 2.45f;		//�R���C�_�[�̊g��{���B���X��2.45f�B
	std::vector<Cell*>	m_cells;						//�Z���̔z��B

	//�f�o�b�O�֌W�B
	ModelRender* m_debugModel = nullptr;			//�f�o�b�O���f���B
	std::vector<ModelRender*>	m_debugModelList;				//�f�o�b�O���f���̔z��B
	bool							m_isDebug = false;				//�f�o�b�O���[�h�̃I���I�t�B
	bool							m_isChangeActiveModel = false;	//�f�o�b�O���[�h�̃I���I�t�B
	bool							m_isCreateModel = false;		//�f�o�b�O���f������������ǂ����B
};

