#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow {

    SceneLight::SceneLight()
    {

    }

    void SceneLight::Init()
    {
        // 太陽光
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

        // 全てのポイントライトを未使用にする
        for (auto& pt : m_light.pointLights) {
            pt.UnUse();
            pt.SetAffectPowParam(1.0f);
        }
        m_light.numPointLight = 0;

        // 全てのスポットライトを未使用にする。
        for (auto& sp : m_light.spotLights) {
            sp.UnUse();
        }
        m_light.numSpotLight = 0;
    }

    void SceneLight::Update()
    {
        m_light.numPointLight = 0;
        for (int i = 0; i < MAX_POINT_LIGHT; i++)
        {
            if (m_light.pointLights[i].GetUse())
            {
                m_light.numPointLight++;
            }
        }

        m_light.numSpotLight = 0;
        for (int i = 0; i < MAX_SPOT_LIGHT; i++)
        {
            if (m_light.spotLights[i].GetUse())
            {
                m_light.numSpotLight++;
            }
        }
    }


    SPointLight* SceneLight::NewPointLight()
    {
        if (m_light.numPointLight >= MAX_POINT_LIGHT)
        {
            return nullptr;
        }

        for (int i = 0; i < MAX_POINT_LIGHT; i++)
        {
            if (m_light.pointLights[i].GetUse() != true)
            {
                m_light.pointLights[i].Use();
                return &m_light.pointLights[i];
            }
        }
    }

    void SceneLight::DeletePointLight(SPointLight* m_pointlight)
    {
        //ポインタにアドレスが入ってなかったら何もせず返す
        if (m_pointlight == nullptr)
        {
            return;
        }
        //万が一シーンライトが管理していないポイントライトに対して削除処理を実行すると管理がバグる(特に使用中のライトの数)ので
        //本当に削除処理をしていいか確認する
        // 
        //送られてきたポインタに入っているアドレスと、配列の各要素のアドレスを照らし合わせて、
        //アドレスが同じものを見つけたら、削除処理を実行する
        for (int i = 0; i < MAX_POINT_LIGHT; i++)
        {
            
            if (m_pointlight == &m_light.pointLights[i])
            {
                m_pointlight->UnUse();
                m_pointlight->SetColor(0.0f, 0.0f, 0.0f);
                m_pointlight->SetPosition(0.0f, 0.0f, 0.0f);
                m_pointlight->SetRange(0.0f);
                m_pointlight->SetAffectPowParam(1.0f);
            }
        }
    }


    SSpotLight* SceneLight::NewSpotLight()
    {
        if (m_light.numSpotLight >= MAX_SPOT_LIGHT)
        {
            return nullptr;
        }

        for (int i = 0; i < MAX_SPOT_LIGHT; i++)
        {
            if (m_light.spotLights[i].GetUse() != true)
            {
                m_light.spotLights[i].Use();
                return &m_light.spotLights[i];
            }
        }
    }

    void SceneLight::DeleteSpotLight(SSpotLight* m_spotlight)
    {
        if (m_spotlight == nullptr)
        {
            return;
        }

        for (int i = 0; i < MAX_SPOT_LIGHT; i++)
        {

            if (m_spotlight == &m_light.spotLights[i])
            {
                m_spotlight->UnUse();
                m_spotlight->SetColor(0.0f, 0.0f, 0.0f);
                m_spotlight->SetPosition(0.0f, 0.0f, 0.0f);
                m_spotlight->SetRange(0.0f);
                m_spotlight->SetRangeAffectPowParam(1.0f);
                m_spotlight->SetAngleAffectPowParam(1.0f);
            }
        }
    }
}
