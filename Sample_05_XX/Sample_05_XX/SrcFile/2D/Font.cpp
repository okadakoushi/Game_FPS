#include "stdafx.h"
#include "Font.h"

namespace myEngine {
	CFont::CFont()
	{
		m_fe = &GraphicsEngineObj()->GetFontEngine();
	}

	CFont::~CFont()
	{
	}

	void CFont::Begin()
	{
		//���݂̃����_�[�X�e�[�g��ۑ�
		//*****
		//�`��ݒ�B
		m_fe->BeginDraw();
	}

	void CFont::End()
	{
		//�`��I���B
		m_fe->EndDraw();
	}

	void CFont::Draw(
		wchar_t const* text,
		const Vector2& pos,
		const Vector4& color,
		float rotation,
		float scale,
		Vector2 pivot
	)
	{
		if (text == nullptr) {
			//�Ȃɂ��Ȃ�����A�Ȃɂ�����I
			return;
		}

		pivot.y = 1.0f - pivot.y;
		DirectX::XMFLOAT2 tkFloat2Zero(0, 0);
		//���W�n���X�v���C�g�ƍ��킹��B
		Vector2 position = pos;
		float frameBufferHalfWidth = FRAME_BUFFER_W * 0.5f;
		float frameBufferHalfHeight = FRAME_BUFFER_H * 0.5f;
		position.x += frameBufferHalfWidth;
		position.y = -pos.y + frameBufferHalfHeight;

		m_fe->Draw(
			text,
			position,
			color,
			rotation,
			scale,
			pivot
		);
	}
}