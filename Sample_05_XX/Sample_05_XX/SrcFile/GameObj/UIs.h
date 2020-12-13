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
private:
	SpriteRender* m_testSprite = nullptr;	//テストレンダー。
};

