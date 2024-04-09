#include "stdafx.h"
#include "Car.h"

Car::Car()
{

}

Car::~Car()
{

}

bool Car::Start()
{
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/car/proto_car/proto_car.tkm");
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

void Car::Update()
{
	//���f�����X�V����B
	m_modelRender.Update();
}

void Car::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}