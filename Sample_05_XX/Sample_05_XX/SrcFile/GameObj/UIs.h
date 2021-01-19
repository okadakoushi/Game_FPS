#pragma once

/// <summary>
/// UI系管理オブジェクト。
/// </summary>
class UIs : public IGameObject
{
public:
	void OnDestroy() override;
	/// <summary>
	/// 初期化。
	/// </summary>
	bool Start() override;

	void Update();
	void RenderHUD() override;
private:
	Sprite m_reticle;
};

