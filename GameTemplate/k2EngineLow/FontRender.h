#pragma once
namespace nsK2EngineLow {
	class FontRender : public IRenderer
	{
	public:
		static const int MAX_TEXT_SIZE = 256;

		FontRender() {};
		~FontRender() {};

		//�\�����镶����ݒ�
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		//�\�����镶�����擾
		const wchar_t* GetText() const
		{
			return m_text;
		}

		//���W��ݒ�
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
		}
		void SetPosition(float x, float y)
		{
			SetPosition({ x, y, 0 });
		}
		//���W���擾
		Vector3 GetPosition() const
		{
			return m_position;
		}

		//�g�嗦��ݒ�
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		//�g�嗦���擾
		const float GetScale() const
		{
			return m_scale;
		}

		//��]��ݒ�
		//������������Ɣ����v���A����������Ǝ��v���ɉ�]����
		void SetRotation(const float rotation)
		{
			m_rotation = rotation;
		}
		//��]���擾
		const float GetRotation() const
		{
			return m_rotation;
		}

		//�F��ݒ�
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r,g,b,a });
		}

		//�F���擾
		const Vector4& GetColor() const
		{
			return m_color;
		}

		/// <summary>
		/// �e�̃p�����[�^��ݒ�B
		/// </summary>
		/// <param name="isDrawShadow">�e��`�悷��H</param>
		/// <param name="shadowOffset">�e��`�悷��Ƃ��̃s�N�Z���̃I�t�Z�b�g�ʁB</param>
		/// <param name="shadowColor">�e�̐F�B</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}

		//�`�揈���B
		void Draw(RenderContext& rc);

		//2D�̕`�揈��
		void OnRender2D(RenderContext& rc) override;


	private:
		Vector3							m_position = Vector3::Zero;			//���W�B
		float							m_rotation = 0.0f;					//��]�B
		float							m_scale = 1.0f;						//�����̑傫���B
		Vector4							m_color = g_vec4White;				//�����̐F�A�f�t�H���g�͔��B
		Vector2							m_pivot = Sprite::DEFAULT_PIVOT;	//�s�{�b�g�B

		wchar_t							m_text[MAX_TEXT_SIZE];				//�\�����镶���B
		Font							m_font;
	};
}
