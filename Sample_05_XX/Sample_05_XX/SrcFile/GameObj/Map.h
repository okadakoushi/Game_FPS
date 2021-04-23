#pragma once
class Map : public IGameObject
{
public:
	~Map();
	bool Start() override;
	void Update() override;
private:
	Vector3 m_pos = g_vec3Zero;
	Vector3 m_scale = g_vec3Zero;
	Vector2 m_pivot = g_vec2Zero;
	SpriteRender* m_spriteRender = nullptr;
};

