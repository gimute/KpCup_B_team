#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow {

    SceneLight::SceneLight()
    {

    }

    void SceneLight::Init()
    {
        // ���z��
        m_light.directionalLight[0].color.x = 1.0f;
        m_light.directionalLight[0].color.y = 1.0f;
        m_light.directionalLight[0].color.z = 1.0f;

        m_light.directionalLight[0].direction.x = 1.0f;
        m_light.directionalLight[0].direction.y = -1.0f;
        m_light.directionalLight[0].direction.z = -1.0f;
        m_light.directionalLight[0].direction.Normalize();
        m_light.directionalLight[0].castShadow = true;


        m_light.ambientLight.x = 0.3f;
        m_light.ambientLight.y = 0.3f;
        m_light.ambientLight.z = 0.3f;
        m_light.eyePos = g_camera3D->GetPosition();
        m_light.numPointLight = 0;

        // �S�Ẵ|�C���g���C�g�𖢎g�p�ɂ���
        for (auto& pt : m_light.pointLights) {
            pt.UnUse();
            pt.SetAffectPowParam(1.0f);
        }
        m_light.numPointLight = 0;

        // �S�ẴX�|�b�g���C�g�𖢎g�p�ɂ���B
        /*int spNo = 0;
        for (auto& sp : m_light.spotLights) {
            sp.UnUse();
            sp.SetNo(spNo);
            m_unuseSpotLightQueue.push_back(&sp);
            spNo++;
        }*/
    }

    void SceneLight::Update()
    {

    }
}
