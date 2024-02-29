/*!
 * @brief	�V���v���ȃ��f���V�F�[�_�[�B
 */



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
    float3 dirLigDirection; // ���C�g�̕���
    float3 dirLigColor;     // ���C�g�̐F
    
    float3 ptLigPosition;   // �|�C���g���C�g�̈ʒu
    float3 ptLigColor;      // �|�C���g���C�g�̃J���[
    float ptLigRange;       // �|�C���g���C�g�̉e���͈�
    
    float3 spLigPosition;   // �X�|�b�g���C�g�̈ʒu   
    float3 spLigColor;      // �X�|�b�g���C�g�̃J���[
    float spLigRange;       // �X�|�b�g���C�g�̎ˏo�͈�
    float3 spLigDirection;  // �X�|�b�g���C�g�̎ˏo����
    float spLigAngle;       // �X�|�b�g���C�g�̎ˏo�p�x
    
    float3 eyePos;          // ���_�̈ʒu
    float specPow;          // �X�y�L�����̍i��
    
    float3 groundColor;     // �Ƃ�Ԃ��̃��C�g
    float3 skyColor;        // �V�����C�g
    float3 groundNormal;    // �n�ʂ̖@��
};

////////////////////////////////////////////////
// �\����
////////////////////////////////////////////////
//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn
{
    float4 pos : POSITION; //���f���̒��_���W�B
    float3 normal : NORMAL; //�@��
    
    float3 tangent : TANGENT; //�ڃx�N�g��
    float3 biNormal : BINORMAL; //�]�x�N�g��
    
    float2 uv : TEXCOORD0; //UV���W�B
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float3 normal : NORMAL; //�@��
    
    float3 tangent : TANGENT;   //�ڃx�N�g��
    float3 biNormal : BINORMAL; //�]�x�N�g��
    
    float2 uv : TEXCOORD0; //uv���W�B
    float3 worldPos : TEXCOORD1;    //���[���h��ԍ��W
    float3 normalInView : TEXCOORD2;    //�J������Ԃ̖@��
};

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
Texture2D<float4> g_normalMap : register(t1);   //�@���}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B
sampler g_sampler : register(s0); //�T���v���X�e�[�g�B

////////////////////////////////////////////////
// �֐��錾�B
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromDirectionLight(SPSIn psIn);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromSpotLight(SPSIn psIn);
float3 CalcLigFromLimLight(float3 lightDirection, float3 lightColor, float3 normal, float3 normalInView);
float3 CalcLigFormhemiLight(SPSIn psIn);

/// <summary>
//�X�L���s����v�Z����B
/// </summary>
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

/// <summary>
/// ���_�V�F�[�_�[�̃R�A�֐��B
/// </summary>
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
    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(mWorld, vsIn.normal));
	
    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    
    psIn.uv = vsIn.uv;

    //�J������Ԃ̖@��
    psIn.normalInView = mul(mView, psIn.normal);
    
    return psIn;
}

/// <summary>
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}
/// <summary>
/// �s�N�Z���V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{   
    //�@���}�b�v�Ŗ@���̌�����ς���
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv);
    
    localNormal = (localNormal - 0.5) * 2.0f;
    
    psIn.normal = psIn.tangent * localNormal.x
                + psIn.biNormal * localNormal.y
                + psIn.normal * localNormal.z;
    
    psIn.normal = normalize(psIn.normal);
    
    
    //�f�B���N�V�������C�g�ɂ�郉�C�e�B���O���v�Z
    float3 directionLig = CalcLigFromDirectionLight(psIn);
    //�|�C���g���C�g�ɂ�郉�C�e�B���O���v�Z
    float3 pointLig = CalcLigFromPointLight(psIn);
    //�X�|�b�g���C�g�ɂ�郉�C�e�B���O���v�Z
    float3 spotLig = CalcLigFromSpotLight(psIn);
    //�������C�g�ɂ�郉�C�e�B���O���v�Z
    float3 hemiLig = CalcLigFormhemiLight(psIn);
    
    
    
    
    //�ŏI�I�Ȍ������߂�
    float3 finalLig = directionLig + hemiLig;+ pointLig + spotLig;
    
    //float3 finalLig = directionLig;
    //float3 finalLig = pointLig;
    //float3 finalLig = spotLig;
    //float3 finalLig = spotLig + ambientLight;
    
    float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //�e�N�X�`���J���[�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
    finalColor.xyz *= finalLig;
    
    return finalColor;
}



////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////

// Lambert�g�U���ˌ����v�Z����
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    //�s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
    float t = dot(normal, lightDirection) * -1.0f;
    
    //���ς̌��ʂ�0��菬��������0�ɂ���
    t = max(0.0f, t);
    
    //�g�U���ˌ����v�Z����
    return lightColor * t;
}


//Phonng���ʔ��ˌ����v�Z����
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
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
    return lightColor * t;
}

//�f�B���N�V�������C�g�ɂ�锽�ˌ����v�Z
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
    //�f�B���N�V�������C�g�ɂ��Lambert�g�U���ˌ����v�Z����
    float3 diffDirection = CalcLambertDiffuse(dirLigDirection, dirLigColor, psIn.normal);
    
    //�f�B���N�V�������C�g�ɂ��Phong���ʔ��ˌ����v�Z����
    float3 specDirection = CalcPhongSpecular(dirLigDirection, dirLigColor, psIn.worldPos, psIn.normal);
    
    //�f�B���N�V�������C�g�ɂ�郊�����C�g���v�Z����
    float3 limLig = CalcLigFromLimLight(dirLigDirection,dirLigColor,psIn.normal,psIn.normalInView);
    
    //�f�B���N�V�������C�g�̍ŏI�I�Ȕ��ˌ���Ԃ�
    return diffDirection + specDirection + limLig;
}

//�|�C���g���C�g�ɂ�锽�ˌ����v�Z
float3 CalcLigFromPointLight(SPSIn psIn)
{
    //�T�[�t�F�C�X�ɓ��˂��Ă���A�|�C���g���C�g�̌��̌������v�Z����
    float3 ligDir = psIn.worldPos - ptLigPosition;
    
    //���K������
    ligDir = normalize(ligDir);
    
    //Lambert�g�U���ˌ����v�Z����
    float3 diffPoint = CalcLambertDiffuse(
        ligDir,     //���C�g�̕���
        ptLigColor, //���C�g�̃J���[
        psIn.normal //�T�[�t�F�C�X�̖@��
    );
    
    //Phong���ʔ��ˌ����v�Z����
    float3 specPoint = CalcPhongSpecular(
        ligDir,         //���C�g�̕���
        ptLigColor,     //���C�g�̃J���[
        psIn.worldPos,  //�T�[�t�F�C�X�̃��[���h���W
        psIn.normal     //�T�[�t�F�C�X�̖@��
    );

    //�������C�g���v�Z����
    float3 limLig = CalcLigFromLimLight(
    ligDir,             //���C�g�̕���
    ptLigColor,         //���C�g�̃J���[
    psIn.normal,        //�T�[�t�F�C�X�̖@��
    psIn.normalInView   //�J������Ԃ̃T�[�t�F�C�X�̖@��
    );
    
    
    //�����ɂ��e�������v�Z����
    //�|�C���g���C�g�Ƃ̋������v�Z����
    float3 distance = length(psIn.worldPos - ptLigPosition);
    
    //�e�����������ɂ���ď���������
    float affect = 1.0f - 1.0f / ptLigRange * distance;
    
    //�e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
    if(affect < 0.0f)
    {
        affect = 0.0f;
    }
    
    affect = pow(affect, 3.0f);
    
    diffPoint *= affect;
    specPoint *= affect;
    limLig *= affect;
    
    
    return diffPoint + specPoint + limLig;
}

float3 CalcLigFromSpotLight(SPSIn psIn)
{
    //�T�[�t�F�C�X�ɓ��˂���X�|�b�g���C�g�̌��̌��������߂�
    float3 ligDir = psIn.worldPos - spLigPosition;
    
    //���K������
    ligDir = normalize(ligDir);
    
    //Lambert�g�U���ˌ����v�Z����
    float3 diffSpotLight = CalcLambertDiffuse(
    ligDir,         //���C�g�̕���
    spLigColor,     //���C�g�̃J���[
    psIn.normal     //�T�[�t�F�C�X�̖@��
    );

    //Phong���ʔ��ˌ����v�Z����
    float3 specSpotLight = CalcPhongSpecular(
    ligDir,         //���C�g�̕���
    spLigColor,     //���C�g�̃J���[
    psIn.worldPos,  //�T�[�t�F�C�X�̃��[���h���W
    psIn.normal     //�T�[�t�F�C�X�̖@��
    );
    
    //�������C�g���v�Z����
    float3 limLig = CalcLigFromLimLight(
    ligDir,             //���C�g�̕���
    spLigColor,         //���C�g�̃J���[
    psIn.normal,        //�T�[�t�F�C�X�̖@��
    psIn.normalInView   //�J������Ԃ̃T�[�t�F�C�X�̖@��
    );
    
    //�����ɂ��e�������v�Z����
    //�X�|�b�g���C�g�Ƃ̋������v�Z����
    float3 distance = length(psIn.worldPos - spLigPosition);
    
    //�e�����������ɉ����ď���������
    float affect = 1.0f - 1.0f / spLigRange * distance;
    
    //�e�������}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
    if(affect < 0.0f)
    {
        affect = 0.0f;
    }
    
    //�e���̎d�����w���֐��I�ɂ���
    affect = pow(affect, 3.0f);
    
    //�e��������Z���Ĕ��ˌ�����߂�
    diffSpotLight *= affect;
    specSpotLight *= affect;
    limLig *= affect;
    
    //�p�x�ɂ��e���������߂�
    //���ˌ��Ǝˏo�����̊p�x�����߂�
    float angle = dot(ligDir, spLigDirection);
    angle = abs(acos(angle));
    
    //�p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
    affect = 1.0f - 1.0f / spLigAngle * angle;
    
    //�e�������}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
    if(affect < 0.0f)
    {
        affect = 0.0f;
    }
    
    affect = pow(affect, 0.5f);
    
    //�e��������Z���Ĕ��ˌ�����߂�
    diffSpotLight *= affect;
    specSpotLight *= affect;
    limLig *= affect;
    
    //�ŏI�I�Ȕ��ˌ���Ԃ�
    return diffSpotLight + specSpotLight + limLig;
}

float3 CalcLigFromLimLight(float3 lightDirection, float3 lightColor, float3 normal, float3 normalInView)
{
    //float power1 = 1.0f - max(0.0f, dot(lightDirection, normal));
   
    //float power2 = 1.0f - max(0.0f, normalInView.z * -1.0f);
    
    float power1 = 1.0f - abs(dot(lightDirection, normal));
   
    float power2 = 1.0f - abs(normalInView.z * -1.0f);
   
    
    float limPower = power1 * power2;
    
    limPower = pow(limPower, 1.3f);
    
    return limPower * lightColor;
}

float3 CalcLigFormhemiLight(SPSIn psIn)
{
    //�T�[�t�F�C�X�̖@���ƒn�ʂ̖@���Ƃ̓��ς��v�Z����
    float t = dot(psIn.normal, groundColor);
    
    //���ς̌��ʂ�0�`1�͈̔͂ɕϊ�����
    t = (t + 1.0f) / 2.0f;
    
    //�n�ʐF�ƓV���F��⊮��t�Ő��`�⊮����
    float3 hemiLight = lerp(groundColor, skyColor, t);
    
    return hemiLight;
}
