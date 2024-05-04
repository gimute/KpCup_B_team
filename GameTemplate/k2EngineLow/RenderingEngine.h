#pragma once

namespace nsK2EngineLow {
	class RenderingEngine : public Noncopyable
	{
	public:

		RenderingEngine();
		~RenderingEngine();

		//描画オブジェクトを追加
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		//3Dモデルの描画
		void Render3DModel(RenderContext& rc);

		//2D描画
		void Render2D(RenderContext& rc);

		void Execute(RenderContext& rc);

	private:
		std::vector< IRenderer* > m_renderObjects;                      // 描画オブジェクトのリスト。
	};
}


