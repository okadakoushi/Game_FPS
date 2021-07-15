#include "stdafx.h"
#include "MouseInput.h"
#include "MiniEngine.h"

void MouseInput::Init()
{
	SetCursorPos(DEFAULT_POSITION.x, DEFAULT_POSITION.y);
}

void MouseInput::Update()
{
	//�}�E�X�ł�XY�ړ��B
	//�}�E�X�̈ʒu�B
	POINT pt;
	GetCursorPos(&pt);
	int mauseX = pt.x;
	int mauseY = pt.y;
	//��]�ʂ������l�Ɣ�r���ċ��߂�B
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
