#pragma once
#define MAX_DIRECTIONAL_LIGHT  4
#define MAX_POINT_LIGHT  32
#define MAX_SPOT_LIGHT  32

namespace nsK2EngineLow {

	//�f�B���N�V�������C�g�̍\����
	struct DirectionLight
	{
		Vector3 direction;
		int castShadow = true;
		Vector3 color;
		float pad;
	};

	//�|�C���g���C�g�̍\����
	struct SPointLight
	{
	private:
		Vector3 position = g_vec3Zero;		//���W
		int isUse = false;					//�g�p���t���O
		Vector3 color = g_vec3One;			//���C�g�̃J���[
		float pad1;
		Vector3 attn = { 1.0f,1.0f,0.0f };	//�����p�����[�^�Bx�ɉe���͈́Ay�ɂ͉e�����ɗݏ悷��p�����[�^
		float pad2;
	public:
		//���W��ݒ�
		void SetPosition(const Vector3& position)
		{
			this->position = position;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		//���W���擾
		const Vector3& GetPosition() const
		{
			return position;
		}

		//�J���[��ݒ�
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			SetColor({ r, g, b });
		}
		//�J���[���擾
		const Vector3& GetColor() const
		{
			return color;
		}

		//�͈͂�ݒ�
		void SetRange(float range)
		{
			attn.x = range;
		}
		//�e���͈͂��擾
		float GetRange() const
		{
			return attn.x;
		}

		//�e�����̗ݏ搔��ݒ�B
		void SetAffectPowParam(float powParam)
		{
			attn.y = powParam;
		}

		//�|�C���g���C�g���g�p���ɂ���
		//�G���W���Ŏg�p���邽�߂̊֐��Ȃ̂ŃQ�[��������Ăяo���Ȃ��悤����
		void Use()
		{
			isUse = true;
		}
		//�|�C���g���C�g�𖢎g�p�ɂ���
		//�G���W���Ŏg�p���邽�߂̊֐��Ȃ̂ŃQ�[��������Ăяo���Ȃ��悤����
		void UnUse()
		{
			isUse = false;
		}
		//�|�C���g���C�g���g�p�������ׂ�
		const int GetUse() const
		{
			return isUse;
		}

	};

	//�X�|�b�g���C�g�̍\����
	struct SSpotLight
	{
	private:
		Vector3 position = g_vec3Zero;	//���W
		int isUse = false;				//�g�p���t���O
		Vector3 color = g_vec3One;		//���C�g�̃J���[
		float range;					//�e���͈�
		Vector3 direction = g_vec3Down;	//�ˏo����
		float angle;					//�ˏo�p�x
		Vector3 pow = {1.0f,1.0f,0.0f};	//�e�����ɗݏ悷��p�����[�^
										//x�������ɂ��e�����ɏ�Z����p�����[�^
										//y���p�x�ɂ��e�����ɏ�Z����p�����[�^
		float pad;

	public:
		//���W��ݒ�
		void SetPosition(const Vector3& position)
		{
			this->position = position;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		//���W���擾
		const Vector3& GetPosition() const
		{
			return position;
		}

		//���C�g�̃J���[��ݒ�
		void SetColor(const Vector3 color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			SetColor({ r, g, b });
		}
		//���C�g�̃J���[���擾
		const Vector3& GetColor() const
		{
			return color;
		}

		//�ˏo������ݒ�
		void SetDirection(const Vector3& direction)
		{
			this->direction = direction;
			this->direction.Normalize();
		}
		void SetDirection(float x, float y, float z)
		{
			direction.Set(x, y, z);
			direction.Normalize();
		}
		//�ˏo�������擾
		const Vector3& GetDirection() const
		{
			return direction;
		}

		//�X�|�b�g���C�g���g�p���ɂ���
		//�G���W���Ŏg�p���邽�߂̊֐��Ȃ̂ŃQ�[��������Ăяo���Ȃ��悤����
		void Use()
		{
			isUse = true;
		}
		//�X�|�b�g���C�g�𖢎g�p�ɂ���
		//�G���W���Ŏg�p���邽�߂̊֐��Ȃ̂ŃQ�[��������Ăяo���Ȃ��悤����
		void UnUse()
		{
			isUse = false;
		}
		//�X�|�b�g���C�g���g�p�������ׂ�
		const int GetUse() const
		{
			return isUse;
		}


		//�e���͈͂�ݒ�
		void SetRange(float range)
		{
			this->range = range;
		}
		//�e���͈͂��擾
		const float GetRange() const
		{
			return range;
		}

		//�ˏo�p�x��ݒ�
		void SetAngle(float angle)
		{
			this->angle = angle;
		}
		//�ˏo�p�x���擾
		const float GetAngle() const
		{
			return angle;
		}

		//���̋����ɂ��e�����ɗݏ悷��l��ݒ�
		void SetRangeAffectPowParam(float powParam)
		{
			pow.x = powParam;
		}

		//���̊p�x�ɂ��e�����ɗݏ悷��l��ݒ�
		void SetAngleAffectPowParam(float powParam)
		{
			pow.y = powParam;
		}
	};

	//���C�g�\����
	struct Light
	{
		DirectionLight directionalLight[MAX_DIRECTIONAL_LIGHT];
		SPointLight pointLights[MAX_POINT_LIGHT];                    // �|�C���g���C�g�̔z��B
		SSpotLight spotLights[MAX_SPOT_LIGHT];
		Vector3 eyePos;			//���_�̈ʒu
		int numPointLight;		//�g�p���̃|�C���g���C�g�̐�
		Vector3 ambientLight;	//����
		int numSpotLight;		//�g�p���̃X�|�b�g���C�g�̐�
	};

	//�V�[�����C�g�N���X
	class SceneLight : public Noncopyable
	{
	public:
		SceneLight();

		//������
		void Init();


		//�V�[�����C�g���擾
		Light& GetSceneLight()
		{
			return m_light;
		}

		//�f�B���N�V�������C�g�̃p�����[�^��ݒ�
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			direction.Normalize();
			m_light.directionalLight[lightNo].direction = direction;
			m_light.directionalLight[lightNo].color = color;
		}

		//�e���L���X�g����H
		bool IsCastShadow(int ligNo)
		{
			return m_light.directionalLight[ligNo].castShadow;
		}

		//������ݒ�
		void SetAmbinet(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

		//�X�V�A���󉽂����ĂȂ�
		void Update();

		//�|�C���g���C�g�z�񂩂疢�g�p�̃��C�g�̃|�C���^���
		SPointLight* NewPointLight();

		//�g�p���̃|�C���g���C�g���폜
		void DeletePointLight(SPointLight* m_pointlight);


		//�X�|�b�g���C�g�̔z�񂩂疢�g�p�̃��C�g�̃|�C���^���
		SSpotLight* NewSpotLight();

		//�g�p���̃X�|�b�g���C�g���폜
		void DeleteSpotLight(SSpotLight* m_spotlight);

	private:



	private:
		Light m_light;	//�V�[�����C�g

	};
}

