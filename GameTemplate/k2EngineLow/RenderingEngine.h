#pragma once

namespace nsK2EngineLow {
	class RenderingEngine : public Noncopyable
	{
	public:

		RenderingEngine();
		~RenderingEngine();

		void Init();

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
		RenderTarget m_luminanceRenderTarget;	//輝度抽出用のレンダリング
		Sprite m_luminanceSprite;				//輝度抽出用のスプライト
		RenderTarget m_mainRenderTarget;
		GaussianBlur m_gaussianBulur;

		std::vector< IRenderer* > m_renderObjects;                      // 描画オブジェクトのリスト。
	};
}


