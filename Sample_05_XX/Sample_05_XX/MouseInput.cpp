#include "stdafx.h"
#include "MouseInput.h"
#include "MiniEngine.h"

void MouseInput::Init()
{
	SetCursorPos(DEFAULT_POSITION.x, DEFAULT_POSITION.y);
}

void MouseInput::Update()
{
	//マウスでのXY移動。
	//マウスの位置。
	POINT pt;
	GetCursorPos(&pt);
	int mauseX = pt.x;
	int mauseY = pt.y;
	//回転量を初期値と比較して求める。
	m_xyVal.x = (mauseX - DEFAULT_POSITION.x) * m_Kando;
	m_xyVal.y = (mauseY - DEFAULT_POSITION.y) * m_Kando * 0.5f;
	if (m_xyVal.x != 0.0f && m_xyVal.y != 0.0f) {
		printf("%d, %d\n", mauseX, mauseY);
	}
}

void MouseInput::End()
{
	SetCursorPos((int)DEFAULT_POSITION.x, (int)DEFAULT_POSITION.y);
}
