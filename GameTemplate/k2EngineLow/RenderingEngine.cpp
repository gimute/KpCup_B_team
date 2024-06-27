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



        //�V���h�E�}�b�v�p�����_�����O�^�[�Q�b�g���g�p�\�ɂȂ�܂őҋ@
        renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
        //�����_�����O�^�[�Q�b�g��shadowMap�ɕύX
        renderContext.SetRenderTargetAndViewport(m_shadowMap);
        //�����_�����O�^�[�Q�b�g���N���A
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

        // �o�^����Ă���`��I�u�W�F�N�g���N���A
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
        // �e�`��p�̃��C�g�J�������쐬����
        Camera lightCamera;

        lightCamera.SetAspectOneFlag(true);

        lightCamera.SetViewAngle(Math::DegToRad(80.0f));

        // �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
        lightCamera.SetPosition(-2000, 2000, 2000);

        // �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
        lightCamera.SetTarget(0, 0, 0);

        // �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
        //lightCamera.SetUp(1, 0, 0);

        // ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
        lightCamera.Update();

        for (auto& renderObj : m_renderObjects)
        {
            renderObj->OnRenderShadowMap(rc, lightCamera.GetViewProjectionMatrix());
        }
    }
}
