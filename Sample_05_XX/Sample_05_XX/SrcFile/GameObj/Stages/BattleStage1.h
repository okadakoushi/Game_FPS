#pragma once

class GamePlayer;
class NaviMesh;
class RifleEnemy;

class BattleStage1 : public IGameObject
{
public:
	BattleStage1();
	bool Start() override;
	void Update() override;
	void ForwardRender() override;
	/// <summary>
	/// �v���C���[�ݒ�B
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayer(GamePlayer* pl) {
		m_player = pl;
	}
private:
	Level m_level;
	GamePlayer* m_player = nullptr;
	RifleEnemy* m_rifleEnemy[8];
	NaviMesh* m_naviMesh = nullptr;
};

