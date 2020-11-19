#pragma once
class GamePlayer : public IGameObject
{
public:
	~GamePlayer();
	/// <summary>
	/// スタート。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
private:
	SkinModelRender* m_unityChan = nullptr;		//ユニティーちゃん。
	Vector3 m_pos = g_vec3Zero;					//位置。
};

