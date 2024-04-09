#pragma once
class MiniTimer : public IGameObject
{
public:
	/////////////////////////////////////////�֐�
	MiniTimer() {}
	~MiniTimer() {}
	//�X�^�[�g�֐�
	bool Start();
	//�A�b�v�f�[�g
	void Update();
	//�t�H���g�h���[
	void FontDrow();
	//�^�C����������
	void ReductionTimer();
	//�����_�[
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	FontRender m_fontrender;				//�X�v���C�g�����_�[
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
	//�ʒu
	Vector3 m_position = { 0.0f,0.0f,0.0f };
	//�傫��
	float m_scale = 1.0f;
	//�F
	Vector4 m_color = { 0.0f,0.0f,0.0f,1.0f };
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
		return;
	}
	//�^�C�}�[�}�C�i�X
	void mainasuTimer(int reductiontime) {
		m_timer -= reductiontime;
		return;
	}
	//�ʒu�ݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		return;
	}
	//�傫���ݒ�
	void SetScale(float scale)
	{
		m_scale = scale;
		return;
	}
	//�F�ݒ�
	void SetColor(const Vector4& color)
	{
		m_color = color;
		return;
	}
};

