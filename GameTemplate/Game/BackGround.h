#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


//�����o�֐�

	ModelRender				m_modelRender;			//���f�������_�[�B
	Vector3					m_position;				//���W�B
	Vector3					m_scale = Vector3::One;	//�傫���B
	PhysicsStaticObject		physicsStaticObject;	//�ÓI�I�u�W�F�N�g��ێ�������B
};

