#pragma once

#include "SrcFile/InputKeyCode.h"

class Map : public IGameObject
{
public:
	~Map();
	bool Start() override;
	void Update() override;
private:
	//��{�p�����B
	Vector3 m_pos = g_vec3Zero;
	Vector3 m_scale = g_vec3One;
	Vector2 m_pivot = g_vec2Zero;
	SpriteRender* m_spriteRender = nullptr;
	bool m_isExpansion = false;		//�g�傳��Ă�H
	const int m_WIDTH = 200.0f;		//���B
	const int m_HEIGHT = 200.0f;	//�c�B
};

