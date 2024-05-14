#pragma once

namespace nsK2EngineLow {
	class SpotLight : public Noncopyable
	{
	public:
		SpotLight();
		~SpotLight();

		//座標を設定
		void SetPosition(const Vector3& position)
		{
			m_spotLight->SetPosition(position);
		}
		void SetPosition(float x, float y, float z)
		{
			m_spotLight->SetPosition(x, y, z);
		}
		//座標を取得
		const Vector3& GetPosition() const
		{
			return m_spotLight->GetPosition();
		}

		//カラーを設定
		void SetColor(const Vector3& color)
		{
			m_spotLight->SetColor(color);
		}
		void SetColor(float r, float g, float b)
		{
			m_spotLight->SetColor(r, g, b);
		}
		//カラーを取得
		const Vector3& GetColor() const
		{
			return m_spotLight->GetColor();
		}
		
		//射出方向を設定
		void SetDirection(const Vector3& direction)
		{
			m_spotLight->SetDirection(direction);
		}
		void SetDirection(float x, float y, float z)
		{
			m_spotLight->SetDirection(x, y, z);
		}
		//射出方向を取得
		const Vector3& GetDirection() const
		{
			return m_spotLight->GetDirection();
		}

		//影響範囲を設定
		void SetRange(float range)
		{
			m_spotLight->SetRange(range);
		}
		//影響範囲を取得
		float GetRange() const
		{
			return m_spotLight->GetRange();
		}

		//射出角度を設定
		//単位はDegree、要するに普段使ってる○○度
		void SetAngle(float angle)
		{
			//DegをRadに変換
			angle = Math::DegToRad(angle);
			m_spotLight->SetAngle(angle);
		}
		//射出角度を取得
		//単位はDegree、要するに普段使ってる○○度
		const float GetAngle()const
		{
			//RadをDegに変換してから渡す
			return Math::DegToRad(m_spotLight->GetAngle());
		}


		//距離による影響率の累乗数を設定
		void SetRangeAffectPowParam(float powParam)
		{
			m_spotLight->SetRangeAffectPowParam(powParam);
		}

		//角度による影響率の累乗数を設定
		void SetAngleAffectPowParam(float powParam)
		{
			m_spotLight->SetAngleAffectPowParam(powParam);
		}

		//生データを取得
		const SSpotLight& GetRawData() const
		{
			return *m_spotLight;
		}

	private:
		SSpotLight* m_spotLight = nullptr;

	};
}


