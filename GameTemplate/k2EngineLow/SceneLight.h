#pragma once
#define MAX_DIRECTIONAL_LIGHT  4
#define MAX_POINT_LIGHT  256
#define MAX_SPOT_LIGHT  256

namespace nsK2EngineLow {

    //�f�B���N�V�������C�g�\����
    struct DirectionalLight
    {
        Vector3 direction;  // ���C�g�̕����B
        int castShadow = true;     // �e���L���X�g����H�A����������ł����񂩁H
        Vector3 color;      // ���C�g�̃J���[�B
        float pad2;
    };

    //�|�C���g���C�g�\����
    struct SPointLight
    {
    private:
        Vector3 position;       // ���W
        int isUse = false;      // �g�p���t���O
        Vector3 color;          // ���C�g�̃J���[
        float range;            // �e���͈�
        Vector3 positionInView; // �J������Ԃł̍��W�A�g������m��Ȃ����ǌ�ŕK�v�ɂȂ�����ʓ|�Ȏ��ɂȂ肻���u���Ă���
        float affectPow;        // �e�����ɗݏ悷��p�����[�^�B

    public:
        //���W��ݒ�B
        void SetPosition(const Vector3& position)
        {
            this->position = position;
        }
        void SetPosition(float x, float y, float z)
        {
            SetPosition({ x,y,z });
        }

        //�J���[��ݒ�B
        void SetColor(const Vector3& color)
        {
            this->color = color;
        }
        void SetColor(float r, float g, float b)
        {
            SetColor({ r,g,b });
        }

        //�����͂��͈͂�ݒ�B
        void SetRange(float range)
        {
            this->range = range;
        }
        //�e�����̗ݏ搔��ݒ�B
        //���l��傫������قǌ��̌����̎d�����}�ɂȂ�B(�킩��Ȃ������璼�ڕ����Ă�������)
        void SetAffectPowParam(float powParam)
        {
            affectPow = powParam;
        }

        //���W���擾�B
        const Vector3& GetPosition() const
        {
            return position;
        }
        //�J���[���擾�B
        const Vector3& GetColor() const
        {
            return color;
        }
        //�e���͈͂��擾�B
        float GetRange() const
        {
            return range;
        }

        //�|�C���g���C�g���g�p���ɂ���B
        //�G���W�����Ŏg���p�̊֐��Ȃ̂ŃQ�[��������g�p���Ȃ��ł��������B
        void Use()
        {
            isUse = true;
        }
        //�|�C���g���C�g�𖢎g�p�ɂ���B
        //�G���W�����Ŏg���p�̊֐��Ȃ̂ŃQ�[��������g�p���Ȃ��ł��������B
        void UnUse()
        {
            isUse = false;
        }

        //�X�V
        //�G���W�����Ŏg���p�̊֐��Ȃ̂ŃQ�[��������g�p���Ȃ��ł��������B
        void Update();
    };

    //�X�|�b�g���C�g�\����
    struct SSpotLight {
    private:
        Vector3 position;           // ���W�B
        int isUse = false;          // �g�p���t���O�B
        Vector3 positionInView;     // �J������Ԃł̍��W�B
        int no = 0;                 // ���C�g�̔ԍ��B
        Vector3 color;              // �J���[
        float range;                // �e���͈�
        Vector3 direction;          // �ˏo����
        float angle;                // �ˏo�p�x(�P�ʂ̓��W�A��)
        Vector3 directionInView;    // �J������Ԃł̎ˏo�����B
        float pad1;
        Vector3 pow = g_vec3One;    // �e�����ɗݏ悷��p�����[�^�A
                                    //x�������ɂ��e�����ɁAy���p�x�ɂ��e�����ɓK�p����
        float pad2;

    public:
        //�X�|�b�g���C�g�̔ԍ����擾�B
        int GetNo() const
        {
            return no;
        }
        //�X�|�b�g���C�g�̔ԍ���ݒ�B
        void SetNo(int no)
        {
            this->no = no;
        }

        //�ˏo������ݒ�B
        void SetDirection(const Vector3& direction)
        {
            this->direction = direction;
            this->direction.Normalize();
        }
        //�ˏo�������擾�B
        const Vector3& GetDirection() const
        {
            return direction;
        }

        //���W��ݒ�B
        void SetPosition(const Vector3& position)
        {
            this->position = position;
        }
        void SetPosition(float x, float y, float z)
        {
            SetPosition({ x,y,z });
        }

        //���C�g�̃J���[��ݒ�B
        void SetColor(const Vector3& color)
        {
            this->color = color;
        }
        void SetColor(float r, float g, float b)
        {
            SetColor({ r,g,b });
        }

        //�X�|�b�g���C�g�̉e���͈͂�ݒ�B
        void SetRange(float range)
        {
            this->range = range;
        }

        //�����ɂ��e�����ɗݏ悷��l��ݒ�
        void SetRangeAffectPow(float powParam)
        {
            pow.x = powParam;
        }


        //�p�x�ɂ��e�����ɗݏ悷��l��ݒ�
        void SetAngleAffectPow(float powParam)
        {
            pow.y = powParam;
        }

        //���̎ˏo�p�x��ݒ�B
        void SetAngle(float angle)
        {
            this->angle = angle;
        }
        //���̎ˏo�p�x���擾�B
        float GetAngle() const
        {
            return angle;
        }

        //���W���擾�B
        const Vector3& GetPosition() const
        {
            return position;
        }

        //�e���͈͂��擾�B
        float GetRange() const
        {
            return range;
        }

        //�|�C���g���C�g���g�p���ɂ���B
        //�G���W�����Ŏg���p�̊֐��Ȃ̂ŃQ�[��������g�p���Ȃ��ł��������B
        void Use()
        {
            isUse = true;
        }

        //�|�C���g���C�g�𖢎g�p�ɂ���B
        //�G���W�����Ŏg���p�̊֐��Ȃ̂ŃQ�[��������g�p���Ȃ��ł��������B
        void UnUse()
        {
            isUse = false;
        }

        //�X�V�B
        //�G���W�����Ŏg���p�̊֐��Ȃ̂ŃQ�[��������g�p���Ȃ��ł��������B
        void Update();
    };

    //���C�g�\����
    struct Light
    {
        DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHT];   // �f�B���N�V���i�����C�g�̔z��B
        SPointLight pointLights[MAX_POINT_LIGHT];                   // �|�C���g���C�g�̔z��B
        SSpotLight spotLights[MAX_SPOT_LIGHT];                      // �X�|�b�g���C�g�̔z��
        Matrix mViewProjInv;    // �r���[�v���W�F�N�V�����s��̋t�s��
        Vector3 eyePos;         // �J�����̈ʒu
        int numPointLight;      // �|�C���g���C�g�̐�
        Vector3 ambientLight;   // ����
        int numSpotLight;       // �X�|�b�g���C�g�̐�
    };

    //�V�[�����C�g�N���X�B
    class SceneLight : public Noncopyable
    {
    public:
        SceneLight();
        //������
        void Init();

        //�V�[�����C�g���擾
        Light& GetSceneLight()
        {
            return m_light;
        }

        //�f�B���N�V�������C�g�̃p�����[�^��ݒ�
        void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
        {
            m_light.directionalLight[lightNo].direction = direction;
            m_light.directionalLight[lightNo].color = color;
        }

        //�e���L���X�g����H
        bool IsCastShadow(int ligNo)
        {
            return m_light.directionalLight[ligNo].castShadow;
        }

        //�V�[���Ƀ|�C���g���C�g��ǉ�
        SPointLight* NewPointLight()
        {
            return NewDynamicLight<SPointLight>(m_unusePointLightQueue);
        }
        //�V�[������|�C���g���C�g���폜
        void DeletePointLight(SPointLight* pointLight)
        {
            DeleteDynamicLight<SPointLight>(pointLight, m_unusePointLightQueue);
        }

        //�V�[���ɃX�|�b�g���C�g��ǉ�
        SSpotLight* NewSpotLight()
        {
            return NewDynamicLight<SSpotLight>(m_unuseSpotLightQueue);
        }
        //�V�[������X�|�b�g���C�g���폜
        void DeleteSpotLight(SSpotLight* spotLight)
        {
            DeleteDynamicLight<SSpotLight>(spotLight, m_unuseSpotLightQueue);
        }

        //������ݒ�
        void SetAmbient(Vector3 ambient)
        {
            m_light.ambientLight = ambient;
        }
        
        //�X�V
        void Update();

    private:
        //�V�������I���C�g��ǉ�
        template<class TDynamicLight>
        TDynamicLight* NewDynamicLight(std::deque<TDynamicLight*>& que)
        {
            if (que.empty()) {
                // ����ȏド�C�g��ǉ����邱�Ƃ͂ł��Ȃ��B
                return nullptr;
            }
            // ���g�p�̃��C�g���ŃL���[������o���B
            TDynamicLight* newPt = que.front();
            // �g�p���ɂ���B
            newPt->Use();
            // ���o�����v�f��擪���珜���B
            que.pop_front();
            return newPt;
        }
        //�V�[�����瓮�I���C�g���폜
        template<class TDynamicLight>
        void DeleteDynamicLight(TDynamicLight* light, std::deque< TDynamicLight*>& que)
        {
            if (light != nullptr) {
                // �t���O�𖢎g�p�ɕύX����B
                light->UnUse();
                // ���g�p���X�g�ɒǉ�����B
                que.push_back(light);
            }
        }

    private:
        Light m_light;  //�V�[�����C�g
        std::deque<SPointLight*> m_unusePointLightQueue;    //���g�p�̃|�C���g���C�g�̃L���[�B
        std::deque<SSpotLight*> m_unuseSpotLightQueue;      //���g�p�̃X�|�b�g���C�g�̃L���[�B

    };
}

