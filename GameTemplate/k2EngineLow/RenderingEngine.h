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

		//事前2D描画
		void PreRender2D(RenderContext& rc);

		//シャドウマップ描画処理
		void RenderShadowMap(RenderContext& rc);

		//実行
		void Execute(RenderContext& rc);


		RenderTarget* GetShadowMapRenderTarget()
		{
			return &m_shadowMap;
		}

	private:
		std::vector< IRenderer* > m_renderObjects;	// 描画オブジェクトのリスト。

		RenderTarget m_shadowMap;
	};
}


