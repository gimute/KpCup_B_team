#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

    RenderingEngine::RenderingEngine()
    {

    }
    RenderingEngine::~RenderingEngine()
    {

    }

    void RenderingEngine::Execute(RenderContext& rc)
    {
        Render3DModel(rc);

        Render2D(rc);

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

}
