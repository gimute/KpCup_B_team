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
		Vector3 position = g_vec3Zero;		//座標
		int isUse = false;					//使用中フラグ
		Vector3 color = g_vec3One;			//ライトのカラー
		float pad1;
		Vector3 attn = { 1.0f,1.0f,0.0f };	//減衰パラメータ。xに影響範囲、yには影響率に累乗するパラメータ
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
		//座標を取得
		const Vector3& GetPosition() const
		{
			return position;
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
		//カラーを取得
		const Vector3& GetColor() const
		{
			return color;
		}

		//範囲を設定
		void SetRange(float range)
		{
			attn.x = range;
		}
		//影響範囲を取得
		float GetRange() const
		{
			return attn.x;
		}

		//影響率の累乗数を設定。
		void SetAffectPowParam(float powParam)
		{
			attn.y = powParam;
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
		const int GetUse() const
		{
			return isUse;
		}

	};

	//スポットライトの構造体
	struct SSpotLight
	{
	private:
		Vector3 position = g_vec3Zero;	//座標
		int isUse = false;				//使用中フラグ
		Vector3 color = g_vec3One;		//ライトのカラー
		float range;					//影響範囲
		Vector3 direction = g_vec3Down;	//射出方向
		float angle;					//射出角度
		Vector3 pow = {1.0f,1.0f,0.0f};	//影響率に累乗するパラメータ
										//xが距離による影響率に乗算するパラメータ
										//yが角度による影響率に乗算するパラメータ
		float pad;

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
		//座標を取得
		const Vector3& GetPosition() const
		{
			return position;
		}

		//ライトのカラーを設定
		void SetColor(const Vector3 color)
		{
			this->color = color;
		}
		void SetColor(float r, float g, float b)
		{
			SetColor({ r, g, b });
		}
		//ライトのカラーを取得
		const Vector3& GetColor() const
		{
			return color;
		}

		//射出方向を設定
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
		//射出方向を取得
		const Vector3& GetDirection() const
		{
			return direction;
		}

		//スポットライトを使用中にする
		//エンジンで使用するための関数なのでゲーム側から呼び出さないよう注意
		void Use()
		{
			isUse = true;
		}
		//スポットライトを未使用にする
		//エンジンで使用するための関数なのでゲーム側から呼び出さないよう注意
		void UnUse()
		{
			isUse = false;
		}
		//スポットライトが使用中か調べる
		const int GetUse() const
		{
			return isUse;
		}


		//影響範囲を設定
		void SetRange(float range)
		{
			this->range = range;
		}
		//影響範囲を取得
		const float GetRange() const
		{
			return range;
		}

		//射出角度を設定
		void SetAngle(float angle)
		{
			this->angle = angle;
		}
		//射出角度を取得
		const float GetAngle() const
		{
			return angle;
		}

		//光の距離による影響率に累乗する値を設定
		void SetRangeAffectPowParam(float powParam)
		{
			pow.x = powParam;
		}

		//光の角度による影響率に累乗する値を設定
		void SetAngleAffectPowParam(float powParam)
		{
			pow.y = powParam;
		}
	};

	//ライト構造体
	struct Light
	{
		DirectionLight directionalLight[MAX_DIRECTIONAL_LIGHT];
		SPointLight pointLights[MAX_POINT_LIGHT];                    // ポイントライトの配列。
		SSpotLight spotLights[MAX_SPOT_LIGHT];
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
		SPointLight* NewPointLight();

		//使用中のポイントライトを削除
		void DeletePointLight(SPointLight* m_pointlight);


		//スポットライトの配列から未使用のライトのポインタを提供
		SSpotLight* NewSpotLight();

		//使用中のスポットライトを削除
		void DeleteSpotLight(SSpotLight* m_spotlight);

	private:



	private:
		Light m_light;	//シーンライト

	};
}

