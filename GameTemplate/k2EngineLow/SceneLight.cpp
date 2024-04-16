#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow {
    void SPointLight::Update()
    {
        // 使用中のライトはカメラ空間での座標を計算する。
        if (!isUse) {
            return;
        }
        positionInView = position;
        g_camera3D->GetViewMatrix().Apply(positionInView);
    }
    void SSpotLight::Update()
    {
        if (!isUse) {
            return;
        }
        positionInView = position;
        g_camera3D->GetViewMatrix().Apply(positionInView);
        Matrix cameraRotInv = g_camera3D->GetCameraRotation();
        cameraRotInv.Inverse();
        cameraRotInv.Apply(directionInView);
    }

    void SceneLight::Init()
    {
        // 太陽光
        m_light.directionalLight[0].color.x = 1.2f;
        m_light.directionalLight[0].color.y = 1.2f;
        m_light.directionalLight[0].color.z = 1.2f;

        m_light.directionalLight[0].direction.x = 1.0f;
        m_light.directionalLight[0].direction.y = -1.0f;
        m_light.directionalLight[0].direction.z = -1.0f;
        m_light.directionalLight[0].direction.Normalize();
        m_light.directionalLight[0].castShadow = true;

        m_light.ambientLight.x = 0.1f;
        m_light.ambientLight.y = 0.1f;
        m_light.ambientLight.z = 0.1f;
        m_light.eyePos = g_camera3D->GetPosition();
        m_light.numPointLight = 0;
        //なんかポイントライトの数の初期化してなくね？

        // 全てのポイントライトを未使用にする
        for (auto& pt : m_light.pointLights) {
            pt.UnUse();
            pt.SetAffectPowParam(1.0f);
            m_unusePointLightQueue.push_back(&pt);
        }
        // 全てのスポットライトを未使用にする。
        int spNo = 0;
        for (auto& sp : m_light.spotLights) {
            sp.UnUse();
            sp.SetNo(spNo);
            m_unuseSpotLightQueue.push_back(&sp);
            spNo++;
        }
    }

    void SceneLight::Update()
    {
        m_light.numPointLight = MAX_POINT_LIGHT - static_cast<int>(m_unusePointLightQueue.size());
        m_light.numSpotLight = MAX_SPOT_LIGHT - static_cast<int>(m_unuseSpotLightQueue.size());
    }
}
