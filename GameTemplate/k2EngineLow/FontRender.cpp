#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

namespace nsK2EngineLow {

	//Font��Draw�Ƀf�[�^���Ԃ����ނ���
	//�\������e�L�X�g���ݒ肳��Ă��Ȃ������牽�������Ԃ�
	void FontRender::Draw(RenderContext& rc)
	{
		if (m_text == nullptr)
		{
			return;
		}

		m_font.Begin(rc);
		m_font.Draw(
			m_text,
			Vector2(m_position.x, m_position.y),
			m_color,
			m_rotation,
			m_scale,
			Sprite::DEFAULT_PIVOT
		);
		m_font.End(rc);
	}
}
