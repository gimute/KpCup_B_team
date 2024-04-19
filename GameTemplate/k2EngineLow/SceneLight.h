#pragma once
#define MAX_DIRECTIONAL_LIGHT  4
#define MAX_POINT_LIGHT  32
#define MAX_SPOT_LIGHT  32

namespace nsK2EngineLow {

	//ディレクションライトの構造体
	struct DirectionLight
	{
		Vector3 direction;
		int castShadow = true;
		Vector3 color;
		float pad;
	};

	//ポイントライトの構造体
	struct SPointLight
	{
	private:
		Vector3 position;	//座標
		int isUse = false;	//使用中フラグ
		Vector3 color;		//ライトのカラー
		float pad1;
		Vector3 attn;		//減衰パラメータ。xに影響範囲、yには影響率に累乗するパラメータ
		float pad2;
	public:
		//座標を設定
		void SetPosition(const Vector3& position)
		{
			this->position = position;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}

		//カラーを設定
		void SetColor(const Vector3& color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			SetColor({ r, g, b });
		}

		//範囲を設定
		void SetRange(float range)
		{
			attn.x = range;
		}

		//影響率の累乗数を設定。
		void SetAffectPowParam(float powParam)
		{
			attn.y = powParam;
		}

		//座標を取得
		const Vector3& GetPosition() const
		{
			return position;
		}

		//カラーを取得
		const Vector3& GetColor() const
		{
			return color;
		}

		//影響範囲を取得
		float GetRange() const
		{
			return attn.x;
		}

		//ポイントライトを使用中にする
		//エンジンで使用するための関数なのでゲーム側から呼び出さないよう注意
		void Use()
		{
			isUse = true;
		}
		//ポイントライトを未使用にする
		//エンジンで使用するための関数なのでゲーム側から呼び出さないよう注意
		void UnUse()
		{
			isUse = false;
		}
		//ポイントライトが使用中か調べる
		int GetUse()
		{
			return isUse;
		}

	};

	//スポットライトの構造体
	struct SSpotLight
	{

	};

	//ライト構造体
	struct Light
	{
		DirectionLight directionalLight[MAX_DIRECTIONAL_LIGHT];
		SPointLight pointLights[MAX_POINT_LIGHT];                    // ポイントライトの配列。
		//SSpotLight spotLights[MAX_SPOT_LIGHT];
		Vector3 eyePos;			//視点の位置
		int numPointLight;		//使用中のポイントライトの数
		Vector3 ambientLight;	//環境光
		int numSpotLight;		//使用中のスポットライトの数
	};

	//シーンライトクラス
	class SceneLight : public Noncopyable
	{
	public:
		SceneLight();

		//初期化
		void Init();


		//シーンライトを取得
		Light& GetSceneLight()
		{
			return m_light;
		}

		//ディレクションライトのパラメータを設定
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			direction.Normalize();
			m_light.directionalLight[lightNo].direction = direction;
			m_light.directionalLight[lightNo].color = color;
		}

		//影をキャストする？
		bool IsCastShadow(int ligNo)
		{
			return m_light.directionalLight[ligNo].castShadow;
		}

		//環境光を設定
		void SetAmbinet(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

		//更新、現状何もしてない
		void Update();

		//ポイントライト配列から未使用のライトのポインタを提供
		SPointLight* NewPointLight()
		{
			if (m_light.numPointLight >= MAX_POINT_LIGHT)
			{
				return nullptr;
			}

			for (int i = 0; i < MAX_POINT_LIGHT; i++)
			{
				if (m_light.pointLights[i].GetUse() != true)
				{
					m_light.numPointLight++;
					m_light.pointLights[i].Use();
					return &m_light.pointLights[i];
				}
			}
		}
		//使用中のポイントライトを削除
		void DeletePointLight(SPointLight* m_pointlight)
		{
			if (m_pointlight == nullptr)
			{
				return;
			}
			for (int i = 0; i < MAX_POINT_LIGHT; i++)
			{
				if (m_pointlight == &m_light.pointLights[i])
				{
					m_light.numPointLight--;

					m_pointlight->UnUse();
					m_pointlight->SetColor(0.0f, 0.0f, 0.0f);
					m_pointlight->SetPosition(0.0f, 0.0f, 0.0f);
					m_pointlight->SetRange(0.0f);
					m_pointlight->SetAffectPowParam(1.0f);
				}
			}

		}

	private:



	private:
		Light m_light;	//シーンライト

	};
}

