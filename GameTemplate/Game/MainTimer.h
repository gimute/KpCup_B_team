#pragma once
class MainTimer : public IGameObject
{
public:
	/////////////////////////////////////////�֐�
	MainTimer() {}
	~MainTimer() {}
	//�X�^�[�g�֐�
	bool Start();
	//�A�b�v�f�[�g
	void Update();
	//�t�H���g�h���[
	void FontDrow();
	//�^�C����������
	void ReductionTimer();
	//�^�C���ʒu��������
	void TimerPositionUpdate();
	//�����_�[
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	//�t�H���g�����_�[
	FontRender m_fontrender;
	//�X�v���C�g�����_�[
	SpriteRender m_spriterender;
	/////////////////////////////////////////�ϐ�
private:
	//���݃^�C�}�[�l
	int m_timer = 0.0f;
	//1�b�҂��Ɏg���^�C�}�[
	float m_1timer = 0;
	const float m_1time = 1.0;
	//�ő�^�C�}�[�l
	const float m_fulltimer = 0.0f;
	//�^�C�}�[�����炷�����炳�Ȃ���
	//true�Ō��炷
	bool m_reductiontime_bool = false;
	//���Ԉʒu
	Vector3 m_timerposition = { -900.0f,-320.0f,0.0f };
	//���ԑ傫��
	float m_timerscale = 3.5f;
	//���ԐF
	Vector4 m_timercolor = { 0.0f,0.0f,0.0f,1.0f };
	//UI�ʒu
	Vector3 m_uiposition = { -630.0f,-330.0f,0.0f };
	/////////////////////////////////////////�����ݒ�n��
public:
	//�^�C�}�[�Z�b�g
	void SetTimer(int settime) {
		m_timer = settime;
		m_reductiontime_bool = true;
		return;
	}
	//�^�C�}�[�v���X
	void purasuTimer(int addtime) {
		m_timer += addtime;
		m_1timer = m_1time;
		return;
	}
	//�^�C�}�[�}�C�i�X
	void mainasuTimer(int reductiontime) {
		m_timer -= reductiontime;
		m_1timer = m_1time;
		return;
	}
	//���Ԉʒu�ݒ�
	void SetTimerPosition(const Vector3& position)
	{
		m_timerposition = position;
		return;
	}
	//���ԑ傫���ݒ�
	void SetTimerScale(float scale)
	{
		m_timerscale = scale;
		return;
	}
	//���ԐF�ݒ�
	void SetTimerColor(const Vector4& color)
	{
		m_timercolor = color;
		return;
	}
	//����UI�ʒu�ݒ�
	void SetUIPosition(const Vector3& position)
	{
		m_uiposition = position;
		return;
	}
	void SetUIScale(const Vector3& scale)
	{

	}
};

