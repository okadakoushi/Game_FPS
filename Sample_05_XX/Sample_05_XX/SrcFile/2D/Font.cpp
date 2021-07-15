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
		//現在のレンダーステートを保存
		//*****
		//描画設定。
		m_fe->BeginDraw();
	}

	void CFont::End()
	{
		//描画終了。
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
			//なにもないから、なにもせん！
			return;
		}

		pivot.y = 1.0f - pivot.y;
		DirectX::XMFLOAT2 tkFloat2Zero(0, 0);
		//座標系をスプライトと合わせる。
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