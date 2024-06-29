#pragma once
class Game;
class Title;
class Load : public IGameObject
{
public: 

	struct loadParam {
		float m_currentAlpha;
		float m_fontAlpha;
	};

	Load();
	~Load();
	bool Start();
	//�`��֐�
	void Render(RenderContext& rc);
	//�X�V����
	void Update();
	//�A���t�@�`�����l���̒���
	void AlphaCalc();
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
	SpriteRender m_spriteLoad;

	//�A���t�@�`�����l���̒����p�̕ϐ�
	float m_alpha = 0.1f;
	//float m_currentAlpha = 0.0f; //���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
	//bool m_loadEffect;
	bool m_alphaCalcBool;
	bool m_loadDraw = false;
	enum EnState {
		enState_FadeIn,		//�t�F�[�h�C����
		enState_FadeOut,	//�t�F�[�h�A�E�g��
		enState_Idle,		//�A�C�h����
	};
	//���
	EnState m_state = enState_Idle;

	Title* m_title;

	loadParam m_loadParam;

	/// <summary>
	/// �댯�M���p�e�N�X�`�����̂P
	/// </summary>
	Texture m_LoadMeianTexture1;


};

