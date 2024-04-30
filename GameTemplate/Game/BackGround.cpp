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
	m_modelRender.Init("Assets/modelData/Map/Map3.tkm");
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);

	//���f�����X�V����B
	m_modelRender.Update();
	//�ÓI�����I�u�W�F�N�g���쐬
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);


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
