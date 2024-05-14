#pragma once

namespace nsK2EngineLow {
	class SpriteRender : public IRenderer
	{
	public:
		SpriteRender() {};
		~SpriteRender() {};

		// �������B
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);

		//���[�U�[���p�ӂ���SpriteInitData��g����������
		void Init(SpriteInitData initData)
		{
			m_sprite.Init(initData);
		}

		//���W��ݒ�Bz��0.0f�ŁB
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		//���W��擾
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		//�X�P�[����ݒ�
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		//�X�P�[����擾
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		//��]��ݒ�BZ���̉�]�ȊO�͋��������������Ȃ�̂Œ���
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		//��]��擾
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		//�s�{�b�g��ݒ�B
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// �s�{�b�g��擾�B
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}

		//�X�V����
		void Update()
		{
			m_sprite.Update(
				m_position,
				m_rotation,
				m_scale,
				m_pivot);
		}

		//�`�揈��
		void Draw(RenderContext& rc);

		//2D�̕`�揈��
		void OnRender2D(RenderContext& rc) override;

	private:
		Sprite		m_sprite;
		Vector3			m_position = Vector3::Zero;				//���W�B
		Quaternion		m_rotation = Quaternion::Identity;		//��]�B
		Vector3			m_scale = Vector3::One;

		Vector2			m_pivot = Sprite::DEFAULT_PIVOT;

	};

}

