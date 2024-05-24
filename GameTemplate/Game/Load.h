#pragma once
class Load : public IGameObject
{
public: 
	Load();
	~Load();
	bool Start();
	//�`��֐�
	void Render(RenderContext& rc);
	//�X�V����
	void Update();
	//�t�F�[�h�C��
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	//�t�F�[�h�A�E�g
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	//�t�F�[�h���Ȃ�true�B
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	
	//�X�v���C�g�����_�[
	SpriteRender m_spriteRender_load;

	//�A���t�@�`�����l���̒����p�̕ϐ�
	float m_alpha = 0.1f;
	float m_currentAlpha = 0.0f; //���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
	bool m_alphaCalcBool;

	enum EnState {
		enState_FadeIn,		//�t�F�[�h�C����
		enState_FadeOut,	//�t�F�[�h�A�E�g��
		enState_Idle,		//�A�C�h����
	};
	//���
	EnState m_state = enState_Idle;


};

