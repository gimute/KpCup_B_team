#include "stdafx.h"
#include "RemainingBulletsUi.h"

RemainingBulletsUi::RemainingBulletsUi()
{

}

RemainingBulletsUi::~RemainingBulletsUi()
{

}

namespace {
	const Vector3 UI_BASE_POS = { -640.0f,250.0f,0.0f };
	const Vector3 REMAINING_BULLET_UI_POS = { -820.0f,345.0f,0.0f };
	const float REMAINING_BULLET_UI_SCALE = 0.8;
	const Vector4 REMAINING_BULLET_UI_COLOR = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float REMAINING_BULLET_RELOAD_UI_WIPEMAX = -286.0f;
}

bool RemainingBulletsUi::Start()
{
	m_RemainingBulletUi.Init("Assets/modelData/ui_remainingbullet/ui_base.DDS", 986.0f / 4, 286.0f / 4);

	m_RemainingBulletUi.SetPosition(UI_BASE_POS);

	SpriteInitData spriteInitDataReloadUi;

	spriteInitDataReloadUi.m_ddsFilePath[0] = "Assets/modelData/ui_remainingbullet/ui_reload.DDS";

	//m_RemainingBulletReloadTexture.InitFromDDSFile(L"Assets/modelData/ui_remainingbullet/ui_reload.DDS");
	//spriteInitDataReloadUi.m_expandShaderResoruceView[0] = &m_RemainingBulletReloadTexture;

	spriteInitDataReloadUi.m_fxFilePath = "Assets/shader/remainingBulletsUi.fx";

	m_remainingBulletReloadUiWipeParam.m_remainingBulletReloadUiWipeDir.Set(-1.0f, 0.0f);
	m_remainingBulletReloadUiWipeParam.m_remainingBulletReloadUiWipeSize = 0;

	spriteInitDataReloadUi.m_expandConstantBuffer = &m_remainingBulletReloadUiWipeParam;
	spriteInitDataReloadUi.m_expandConstantBufferSize = sizeof(m_remainingBulletReloadUiWipeParam);

	spriteInitDataReloadUi.m_width = static_cast<UINT>(986.0f / 4);
	spriteInitDataReloadUi.m_height = static_cast<UINT>(286.0f / 4);
	
	m_RemainingBulletReloadUi.Init(spriteInitDataReloadUi);

	m_RemainingBulletReloadUi.SetPosition(UI_BASE_POS);

	m_RemainingBulletNumberUi.SetPosition(REMAINING_BULLET_UI_POS);

	m_RemainingBulletNumberUi.SetScale(REMAINING_BULLET_UI_SCALE);

	m_RemainingBulletNumberUi.SetColor(REMAINING_BULLET_UI_COLOR);

	return true;
}

void RemainingBulletsUi::Update()
{
	RemainingBulletNumberUiUpdate();

	WipeCalc();

	m_RemainingBulletUi.Update();
	m_RemainingBulletReloadUi.Update();

	m_RemainingBulletNumberUi.SetPosition(REMAINING_BULLET_UI_POS);
}

void RemainingBulletsUi::RemainingBulletNumberUiUpdate()
{
	if (m_RemainingBulletNumber == nullptr)
	{
		return;
	}

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%02d/%02d",int(*m_RemainingBulletNumber)
		,int(m_RemainingBulletNumberFull));
	//表示するテキストを設定
	m_RemainingBulletNumberUi.SetText(wcsbuf);
}

void RemainingBulletsUi::WipeCalc()
{
	float wipeSize = m_remainingBulletReloadUiWipeParam.GetWipeSize();

	if (m_isReload && wipeSize >= REMAINING_BULLET_RELOAD_UI_WIPEMAX)
	{
		m_remainingBulletReloadUiWipeParam.m_remainingBulletReloadUiWipeSize -= 15.0f;
		return;
	}

	if (!m_isReload && wipeSize <= 0)
	{
		m_remainingBulletReloadUiWipeParam.m_remainingBulletReloadUiWipeSize += 15.0f;
		return;
	}
}

void RemainingBulletsUi::Render(RenderContext& rc)
{
	m_RemainingBulletUi.Draw(rc);
	m_RemainingBulletNumberUi.Draw(rc);
	m_RemainingBulletReloadUi.Draw(rc);
}

