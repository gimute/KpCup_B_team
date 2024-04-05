//���C�g�̊Ǘ��@�\��ǉ����Ă��Ȃ��̂�
//���C�g���f�B���N�V�������C�g�Ɗ����݂̂̃V�F�[�_�[
//���[�V�����Ή�

///////////////////////////////////////////
// �\����
///////////////////////////////////////////
//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos      : POSITION; //���f���̒��_���W�B
    float3 normal   : NORMAL;   //�@��
    
    float3 tangent  : TANGENT;  //�ڃx�N�g��
    float4 biNormal : BINORMAL; //�]�x�N�g��
    
    float2 uv       : TEXCOORD0;//UV���W�B
    
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos      : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float3 normal   : NORMAL;   //�@��
    
    float3 tangent  : TANGENT;  //�ڃx�N�g��
    float3 biNormal : BINORMAL; //�]�x�N�g��
    
    //float3 normalInView : TEXCOORD2; //�J������Ԃ̖@��
    
    float2 uv       : TEXCOORD0; //uv���W�B
    float3 worldPos : TEXCOORD1; //���[���h��ԍ��W
};


////////////////////////////////////////////////
// �萔�o�b�t�@�B
////////////////////////////////////////////////
//���f���p�̒萔�o�b�t�@
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

cbuffer LightCb : register(b1)
{
    float3 dirLigDirection; // �f�B���N�V�������C�g�̕���
    float3 dirLigColor;     // �f�B���N�V�������C�g�̐F
    
    float3 ambientLig;      //�A���r�G���g���C�g
    
    float3 eyePos;          //���_�̈ʒu
};

///////////////////////////////////////////
// �V�F�[�_�[���\�[�X
///////////////////////////////////////////
// ���f���e�N�X�`��
Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_normalMap : register(t1); //�@���}�b�v
Texture2D<float4> g_speclarMap : register(t2); //�X�y�L�����}�b�v

StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B

///////////////////////////////////////////
// �T���v���[�X�e�[�g
///////////////////////////////////////////
sampler g_sampler : register(s0);


///////////////////////////////////////////
// �֐��錾
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float specPow);
float3 CalcLigFromDirectionLight(SPSIn psIn, float specPow);

//�X�L���s����v�Z����B
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}



//���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    
    psIn.pos = mul(m, vsIn.pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.normal = mul(m, vsIn.normal); // �@������]������
    
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    
    psIn.uv = vsIn.uv;

    //psIn.normalInView = mul(mView, psIn.normal);
    
    return psIn;
}

/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}

/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

//���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv);
    localNormal = (localNormal - 0.5) * 2.0f;
    
    psIn.normal = psIn.tangent * localNormal.x
                + psIn.biNormal * localNormal.y
                + psIn.normal * localNormal.z;
    
    psIn.normal = normalize(psIn.normal);
    
    //�X�y�L�����}�b�v����X�y�L�����̔��˂̋������T���v�����O
    float specPow = g_speclarMap.Sample(g_sampler, psIn.uv);
    
    
    //�f�B���N�V�������C�g�ɂ�郉�C�e�B���O���v�Z
    float3 directionLig = CalcLigFromDirectionLight(psIn, specPow);
    

    //�ŏI�I�Ȍ������߂�
    float3 finalLig = directionLig + ambientLig;
    
    
    float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //�e�N�X�`���J���[�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
    finalColor.xyz *= finalLig;
    
    return finalColor;
}


////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////

// Lambert�g�U���ˌ��̌v�Z
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    //�s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
    float t = dot(normal, lightDirection) * -1.0f;
    
    //���ς̌��ʂ�0��菬��������0�ɂ���
    t = max(0.0f, t);
    
    //�g�U���ˌ����v�Z����
    return lightColor * t;
}

//Phonng���ʔ��ˌ��̌v�Z
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float specPow)
{
    //���˃x�N�g�������߂�
    float3 refVec = reflect(lightDirection, normal);
    
    //�������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);
    
    //���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);
    
    //���ς̌��ʂ�0��菬��������0�ɂ���
    t = max(0.0f, t);
    
    //���ʔ��˂̋������i��
    t = pow(t, 5.0f);
    
    //���ʔ��ˌ������߂�
    float3 specularLig = lightColor * t;
    
    return specularLig * specPow;
}

//�f�B���N�V�������C�g�ɂ�锽�ˌ����v�Z
float3 CalcLigFromDirectionLight(SPSIn psIn, float specPow)
{
    //�f�B���N�V�������C�g�ɂ��Lambert�g�U���ˌ����v�Z����
    float3 diffDirection = CalcLambertDiffuse(dirLigDirection, dirLigColor, psIn.normal);
    
    //�f�B���N�V�������C�g�ɂ��Phong���ʔ��ˌ����v�Z����
    float3 specDirection = CalcPhongSpecular(dirLigDirection, dirLigColor, psIn.worldPos, psIn.normal,specPow);
    
    //�f�B���N�V�������C�g�̍ŏI�I�Ȕ��ˌ���Ԃ�
    return diffDirection + specDirection;
}
