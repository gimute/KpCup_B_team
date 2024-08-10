#include "stdafx.h"
#include "Door.h"
#include "Player.h"
#include "Game.h"

Door::Door()
{

}

Door::~Door()
{

}

namespace {
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };
	const Vector3 colScale = { 135.0f,125.0f,135.0f };
	const float AddDoor = 1.5f;
}

bool Door::Start()
{
	//�h�A�g
	m_RenderDoorMain.Init("Assets/modelData/door/doorframe.tkm");
	//�ʒu�ȂǏ����ݒ�
	SetDoorMain(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//�h�A�g�̓����蔻����쐬
	m_DoorMainPhy.CreateFromModel(m_RenderDoorMain.GetModel(), m_RenderDoorMain.GetModel().GetWorldMatrix());
	//�h�AL
	m_RenderDoorL.Init("Assets/modelData/door/doorLR.tkm");
	//�ʒu�ȂǏ����ݒ�
	SetDoorL(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//�h�A���̓����蔻����쐬
	m_DoorLPhy.CreateFromModel(m_RenderDoorL.GetModel(), m_RenderDoorL.GetModel().GetWorldMatrix());
	//�h�AR
	m_RenderDoorR.Init("Assets/modelData/door/doorLR.tkm");
	//�ʒu�ȂǏ����ݒ�
	SetDoorR(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//�h�A�E�̓����蔻����쐬
	m_DoorRPhy.CreateFromModel(m_RenderDoorR.GetModel(), m_RenderDoorR.GetModel().GetWorldMatrix());

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateBox(m_DoorMainPos, m_DoorMainRot, colScale);
	m_collisionObject->SetName("door_col");
	m_collisionObject->SetPosition(m_DoorMainPos + corre1);
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);
	//�v���C���[�̃C���X�^���X���󂯎��
	m_player = FindGO<Player>("player");
	//�Q�[���̃C���X�^���X���󂯎��
	m_game = FindGO<Game>("game");

	//�����������₷�����邽�߂Ƀ��������̉�ʂ�ǂݍ���
	m_spriteRender_moya.Init("Assets/modelData/maintimer/moya.DDS", 700.0f, 100.0f);
	m_spriteRender_moya.SetPosition(Vector3{ 0.0f,-350.0f,0.0f });
	m_spriteRender_moya.SetMulColor({ 0.0f,0.0f,0.0f,0.4f });
	m_spriteRender_moya.Update();

	//�A���_�[�o�[�摜�̐ݒ�
	SpriteInitData initData;
	//DDS�t�@�C���i�摜�f�[�^�j�̃t�@�C���p�X���w�肷��
	//�X�^�[�g�{�^���̉摜�f�[�^���w�肷��
	initData.m_ddsFilePath[0] = "Assets/modelData/maintimer/underBar.DDS";
	//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
	initData.m_fxFilePath = "Assets/shader/spriteUnder.fx";
	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);
	//�X�v���C�g�̕��ƍ������w�肷��
	initData.m_width = static_cast<UINT>(25);
	initData.m_height = static_cast<UINT>(1.5);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;

	m_spRen_under.Init(initData);
	m_spRen_under.SetPosition(Vector3{ 150.0f,-368.0f,0.0f });
	//m_spRen_under.SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
	m_spRen_under.Update();

	//�t�H���g�����_�[�̐ݒ�
	m_fontRender.SetText(L"�G�����ׂē|���Ȃ��ƊJ���Ȃ��悤��");
	m_fontRender.SetScale(0.35f);
	m_fontRender.SetPosition(Vector3(-350.0f, -400.0f, 0.0f));
	m_fontRender.SetColor({ 1.0f,1.0f,1.0f,1.0f });

	return true;
}

void Door::Update()
{
	//�A���t�@�`�����l���̒���
	AlphaCalc();
	m_spRen_under.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_spRen_under.Update();
	//�R���W�����̓����蔻�菈��
	Collision();
	//�J��
	OpenClose();
	// �����蔻��̍��W���X�V
	m_DoorMainPhy.GetRigitBody()->SetPositionAndRotation(m_DoorMainPos, m_DoorMainRot);
	m_DoorLPhy.GetRigitBody()->SetPositionAndRotation(m_DoorLPos, m_DoorLRot);
	m_DoorRPhy.GetRigitBody()->SetPositionAndRotation(m_DoorRPos, m_DoorRRot);
	//�`�揈��
	m_RenderDoorMain.Update();
	m_RenderDoorL.Update();
	m_RenderDoorR.Update();
}

void Door::AlphaCalc()
{
	if (m_alpha >= 1.0f)
	{
		m_alphaCalcBool = false;
	}
	if (m_alphaCalcBool)
	{
		m_alpha += 0.04f;
		return;
	}
	m_alpha -= 0.04f;
	if (m_alpha <= 0.1f)
	{
		m_alphaCalcBool = true;
	}
	
}

void Door::Collision()
{
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_col");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃR���W�������Փ˂�����B
		if (collision->IsHit(m_collisionObject))
		{
			if (m_game->GetEnemyAllKillFlag())
			{
				m_DoorOpen = true;

				return;
			}
			else
			{
				m_spriteDraw = true;
				m_fontDraw = true;
				m_spUnderDraw = true;
				return;
			}
		}
	}
	m_spriteDraw = false;
	m_fontDraw = false;
	m_spUnderDraw = false;
}

void Door::OpenClose()
{
	if (m_DoorOpen == false)
	{
		return;
	}

	//�Œᑬ�x�ɏ���������Ă���������
	Vector3 MovePowL = m_DoorLMove[1] - m_DoorLPos;
	MovePowL /= 70;
	float ratioL = m_DoorLMove[1].Length() / m_DoorLPos.Length();
	MovePowL = MovePowL * ratioL;
	Vector3 MovePowR = m_DoorRMove[1] - m_DoorRPos;
	MovePowR /= 70;
	float ratioR = m_DoorRMove[1].Length() / m_DoorRPos.Length();
	MovePowR = MovePowR * ratioR;
	if (MovePowL.Length() < 0.3f) {
		float test = MovePowL.Length();
	}

	if ((m_DoorLPos - m_DoorLMove[1]).Length() >= 0.01f)
	{
		m_DoorLPos += MovePowL;
		m_DoorRPos += MovePowR;
	}
	m_RenderDoorL.SetPosition(m_DoorLPos);
	m_RenderDoorR.SetPosition(m_DoorRPos);
}

void Door::Render(RenderContext& rc)
{
	m_RenderDoorMain.Draw(rc);
	m_RenderDoorL.Draw(rc);
	m_RenderDoorR.Draw(rc);

	if (m_fontDraw == true)
	{
		m_fontRender.Draw(rc);
	}
	if (m_spUnderDraw == true)
	{
		m_spRen_under.Draw(rc);
	}
	if (m_spriteDraw == true)
	{
		m_spriteRender_moya.Draw(rc);
	}
}