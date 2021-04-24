#pragma once

#include "SrcFile/InputKeyCode.h"

class Map : public IGameObject
{
public:
	~Map();
	bool Start() override;
	void Update() override;
private:
	//基本パラム。
	Vector3 m_pos = g_vec3Zero;
	Vector3 m_scale = g_vec3One;
	Vector2 m_pivot = g_vec2Zero;
	SpriteRender* m_spriteRender = nullptr;
	bool m_isExpansion = false;		//拡大されてる？
	const int m_WIDTH = 200.0f;		//横。
	const int m_HEIGHT = 200.0f;	//縦。
};

