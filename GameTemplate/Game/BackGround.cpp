#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/background/proto_map/proto_map.tkm");
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);

	//���f�����X�V����B
	m_modelRender.Update();
	//�ÓI�����I�u�W�F�N�g���쐬
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());


	return true;
}

void BackGround::Update()
{
	//���f�����X�V����B
	m_modelRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}