#pragma once
#pragma once

namespace nsK2EngineLow {
	/// <summary>
	/// レンダーラーの基底クラス。
	/// </summary>
	class IRenderer : public Noncopyable {
	public:

		// 3Dモデルの描画
		virtual void OnRenderModel(RenderContext& rc)
		{

		}

		// 2D描画
		virtual void OnRender2D(RenderContext& rc)
		{

		}

		//事前2D描画
		virtual void OnPreRender2D(RenderContext& rc)
		{

		}

		//シャドウマップへの描画処理
		virtual void OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)
		{

		}
	};
}
