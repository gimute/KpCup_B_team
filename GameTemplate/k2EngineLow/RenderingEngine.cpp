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
        float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_shadowMap.Create(
            1024,
            1024,
            1,
            1,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT_D32_FLOAT,
            clearColor
        );
    }

    void RenderingEngine::Execute(RenderContext& rc)
    {
        auto& renderContext = g_graphicsEngine->GetRenderContext();



        //シャドウマップ用レンダリングターゲットが使用可能になるまで待機
        renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
        //レンダリングターゲットをshadowMapに変更
        renderContext.SetRenderTargetAndViewport(m_shadowMap);
        //レンダリングターゲットをクリア
        renderContext.ClearRenderTargetView(m_shadowMap);

        RenderShadowMap(rc);

        renderContext.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

        ///////////////////////////////////////////////////////////////////////////
        /*Sprite shadowSP;

        SpriteInitData SPInitData;
        
        SPInitData.m_fxFilePath = "Assets/shader/sprite.fx";

        SPInitData.m_width = 1024;
        SPInitData.m_height = 1024;

        SPInitData.m_textures[0] = &m_shadowMap.GetRenderTargetTexture();

        shadowSP.Init(SPInitData);

        shadowSP.Update({ 600.0f,300.0f,0.0f }, Quaternion::Identity, { 0.3f,0.3f,0.3f });*/

        /////////////////////////////////////////////////////////////////////////////////////////////

        renderContext.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );
        renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

        PreRender2D(rc);

        Render3DModel(rc);

        Render2D(rc);

        //shadowSP.Draw(renderContext);

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

    void RenderingEngine::PreRender2D(RenderContext& rc)
    {
        for (auto& renderObj : m_renderObjects)
        {
            renderObj->OnPreRender2D(rc);
        }
    }

    void RenderingEngine::RenderShadowMap(RenderContext& rc)
    {
        // 影描画用のライトカメラを作成する
        Camera lightCamera;

        lightCamera.SetAspectOneFlag(true);

        lightCamera.SetViewAngle(Math::DegToRad(80.0f));

        // カメラの位置を設定。これはライトの位置
        lightCamera.SetPosition(-2000, 2000, 2000);

        // カメラの注視点を設定。これがライトが照らしている場所
        lightCamera.SetTarget(0, 0, 0);

        // 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
        //lightCamera.SetUp(1, 0, 0);

        // ライトビュープロジェクション行列を計算している
        lightCamera.Update();

        for (auto& renderObj : m_renderObjects)
        {
            renderObj->OnRenderShadowMap(rc, lightCamera.GetViewProjectionMatrix());
        }
    }
}
