#pragma once

/// <summary>
/// ステージクラス。
/// </summary>
class Stage : public IGameObject
{
public:
	~Stage();
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
	SkinModelRender* m_bg = nullptr;			//ステージ。
	Level m_level;								//レベル。
};

