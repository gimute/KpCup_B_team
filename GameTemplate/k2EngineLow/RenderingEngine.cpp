#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

    RenderingEngine::RenderingEngine()
    {

    }
    RenderingEngine::~RenderingEngine()
    {

    }

    void RenderingEngine::Init()
    {
        m_mainRenderTarget.Create(
            1280,
            720,
            1,
            1,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            DXGI_FORMAT_D32_FLOAT
        );

        m_luminanceRenderTarget.Create(
            1280,
            720,
            1,
            1,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            DXGI_FORMAT_D32_FLOAT
        );

        SpriteInitData spriteInitData;


    }

    void RenderingEngine::Execute(RenderContext& rc)
    {
        Render3DModel(rc);

        Render2D(rc);

        // 登録されている描画オブジェクトをクリア
        m_renderObjects.clear();
    }

    void RenderingEngine::Render3DModel(RenderContext& rc)
    {
        for (auto& renderObj : m_renderObjects)
        {
            renderObj->OnRenderModel(rc);
        }
    }

    void RenderingEngine::Render2D(RenderContext& rc)
    {
        for (auto& renderObj : m_renderObjects)
        {
            renderObj->OnRender2D(rc);
        }
    }

}
