#pragma once
class Car : public IGameObject
{
public: 
	Car();
	~Car();
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);


	//�����o�ϐ��B
	ModelRender				m_modelRender;			//���f�������_�[�B
	Vector3					m_position;				//���W�B
	Vector3					m_scale = Vector3::One;	//�傫���B
	PhysicsStaticObject		physicsStaticObject;	//�ÓI�I�u�W�F�N�g��ێ�������B

};

