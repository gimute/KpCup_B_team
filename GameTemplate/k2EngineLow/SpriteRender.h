#pragma once

namespace nsK2EngineLow {
	class SpriteRender : public Noncopyable
	{
	public:
		SpriteRender() {};
		~SpriteRender() {};

		// 初期化。
		// αブレンディングはとりあえず無しで
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_None);

		//座標を設定。zは0.0fで。
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		//座標を取得
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		//スケールを設定
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		//スケールを取得
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		//回転を設定。Z軸の回転以外は挙動がおかしくなるので注意
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		//回転を取得
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		//ピボットを設定。
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// ピボットを取得。
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}

		//更新処理
		void Update()
		{
			m_sprite.Update(
				m_position,
				m_rotation,
				m_scale,
				m_pivot);
		}

		//ただSpriteのDrawを呼び出すだけ
		void Draw(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}

	private:
		Sprite		m_sprite;
		Vector3			m_position = Vector3::Zero;				//座標。
		Quaternion		m_rotation = Quaternion::Identity;		//回転。
		Vector3			m_scale = Vector3::One;

		Vector2			m_pivot = Sprite::DEFAULT_PIVOT;
	};

}

