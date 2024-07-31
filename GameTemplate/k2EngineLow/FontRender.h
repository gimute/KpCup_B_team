#pragma once
namespace nsK2EngineLow {
	class FontRender : public IRenderer
	{
	public:
		static const int MAX_TEXT_SIZE = 256;

		FontRender() {};
		~FontRender() {};

		//表示する文字を設定
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		//表示する文字を取得
		const wchar_t* GetText() const
		{
			return m_text;
		}

		//座標を設定
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
		}
		void SetPosition(float x, float y)
		{
			SetPosition({ x, y, 0 });
		}
		//座標を取得
		Vector3 GetPosition() const
		{
			return m_position;
		}

		//拡大率を設定
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		//拡大率を取得
		const float GetScale() const
		{
			return m_scale;
		}

		//回転を設定
		//数字が増えると反時計回り、数字が減ると時計回りに回転する
		void SetRotation(const float rotation)
		{
			m_rotation = rotation;
		}
		//回転を取得
		const float GetRotation() const
		{
			return m_rotation;
		}

		//色を設定
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r,g,b,a });
		}

		//色を取得
		const Vector4& GetColor() const
		{
			return m_color;
		}

		/// <summary>
		/// 影のパラメータを設定。
		/// </summary>
		/// <param name="isDrawShadow">影を描画する？</param>
		/// <param name="shadowOffset">影を描画するときのピクセルのオフセット量。</param>
		/// <param name="shadowColor">影の色。</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}

		//描画処理。
		void Draw(RenderContext& rc);

		//2Dの描画処理
		void OnRender2D(RenderContext& rc) override;


	private:
		Vector3							m_position = Vector3::Zero;			//座標。
		float							m_rotation = 0.0f;					//回転。
		float							m_scale = 1.0f;						//文字の大きさ。
		Vector4							m_color = g_vec4White;				//文字の色、デフォルトは白。
		Vector2							m_pivot = Sprite::DEFAULT_PIVOT;	//ピボット。

		wchar_t							m_text[MAX_TEXT_SIZE];				//表示する文字。
		Font							m_font;
	};
}
