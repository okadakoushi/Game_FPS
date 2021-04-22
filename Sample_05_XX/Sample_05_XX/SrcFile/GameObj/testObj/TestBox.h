#pragma once

/// <summary>
/// テスト用のボックス。
/// うわぁ。テクスチャすけてるぅ。
/// </summary>
class TestBox : public IGameObject
{
public:
	bool Start();
	void Update();
private:
	SkinModelRender* m_render = nullptr;
	Vector3 m_pos = g_vec3Zero;
	Quaternion m_rot = g_quatIdentity;
	Vector3 m_scale = g_vec3One;
	PhysicsStaticObject m_psObj;
	RigidBody m_rb;				//剛体。
};

