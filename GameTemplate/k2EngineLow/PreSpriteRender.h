#pragma once

namespace nsK2EngineLow {
	class PreSpriteRender : public IRenderer
	{
	public:
		PreSpriteRender();
		~PreSpriteRender();

		//������
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);

		//�p�ӂ����f�[�^�ŏ�����
		void Init(SpriteInitData initData)
		{
			m_sprite.Init(initData);
		}

		//���W��ݒ�
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		//���W���擾
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		//�X�P�[����ݒ�
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		//�X�P�[�����擾
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		//��]��ݒ�
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		//��]���擾
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		//�s�{�b�g��ݒ�
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		//�s�{�b�g���擾
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}

		//��Z�J���[��ݒ�
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}
		//��Z�J���[���擾
		const Vector4& GetMulColor() const
		{
			return m_sprite.GetMulColor();
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

	private:
		//���O2D�`��p�X����Ă΂�鏈��
		void OnPreRender2D(RenderContext& rc) override
		{
			m_sprite.Draw(rc);
		}

	private:
		Sprite			m_sprite;							//�X�v���C�g
		Vector3			m_position = Vector3::Zero;			//���W
		Quaternion		m_rotation = Quaternion::Identity;	//��]
		Vector3			m_scale = Vector3::One;				//�傫��
		Vector2			m_pivot = Sprite::DEFAULT_PIVOT;	//�s�{�b�g

	};
}


