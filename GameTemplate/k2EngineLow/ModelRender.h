#pragma once

namespace nsK2EngineLow {
	class ModelRender : public Noncopyable
	{
	public:
		struct Light
		{
			Vector3 dirLigDirection;	//ディレクションライトの方向
			float pad0;					//パディング
			Vector3 dirColor;			//ディレクションライトのカラー
			float pad1;

			Vector3 ambientLight;		//アンビエントライト
			float pad2;

			Vector3 eyePos;				//視点の位置
		};

		ModelRender();
		~ModelRender();

		void Init(const char* tkmfilePath);

		void Update();
		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion rot);
		void SetScale(Vector3 scale);

		void Draw(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

	private:
		Light m_light;
		Model m_model;
		Vector3 m_pos = Vector3::Zero;
		Quaternion m_rot = Quaternion::Identity;
		Vector3 m_scale = Vector3::One;
	};

}

