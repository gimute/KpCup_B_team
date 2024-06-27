#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow {

    SceneLight::SceneLight()
    {

    }

    void SceneLight::Init()
    {
        // ���z��
        m_light.directionalLight[0].color.x = 0.8f;
        m_light.directionalLight[0].color.y = 0.8f;
        m_light.directionalLight[0].color.z = 0.8f;

        m_light.directionalLight[0].direction.x = 1.0f;
        m_light.directionalLight[0].direction.y = -1.0f;
        m_light.directionalLight[0].direction.z = -1.0f;
        m_light.directionalLight[0].direction.Normalize();
        m_light.directionalLight[0].castShadow = true;


        m_light.ambientLight.x = 0.3f;
        m_light.ambientLight.y = 0.3f;
        m_light.ambientLight.z = 0.3f;
        m_light.eyePos = g_camera3D->GetPosition();

        // �S�Ẵ|�C���g���C�g�𖢎g�p�ɂ���
        for (auto& pt : m_light.pointLights) {
            pt.UnUse();
            pt.SetAffectPowParam(1.0f);
        }
        m_light.numPointLight = 0;

        // �S�ẴX�|�b�g���C�g�𖢎g�p�ɂ���B
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
        //�|�C���^�ɃA�h���X�������ĂȂ������牽�������Ԃ�
        if (m_pointlight == nullptr)
        {
            return;
        }
        //������V�[�����C�g���Ǘ����Ă��Ȃ��|�C���g���C�g�ɑ΂��č폜���������s����ƊǗ����o�O��(���Ɏg�p���̃��C�g�̐�)�̂�
        //�{���ɍ폜���������Ă������m�F����
        // 
        //�����Ă����|�C���^�ɓ����Ă���A�h���X�ƁA�z��̊e�v�f�̃A�h���X���Ƃ炵���킹�āA
        //�A�h���X���������̂���������A�폜���������s����
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
