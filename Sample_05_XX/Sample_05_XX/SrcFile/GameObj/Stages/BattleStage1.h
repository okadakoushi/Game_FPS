#pragma once

class GamePlayer;
class NaviMesh;
class RifleEnemy;
class StageGenerator;
class Fade;

class BattleStage1 : public IGameObject
{
public:
	BattleStage1();
	~BattleStage1();
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
	const int STAGE_ENEMY_XOUNT = 5;
	Level m_level;
	GamePlayer* m_player = nullptr;
	RifleEnemy* m_rifleEnemy[8];	//�G�l�~�[�B
	int m_enemyCount;
	NaviMesh* m_naviMesh = nullptr;	//�g�p����i�r���b�V���B
	StageGenerator* m_stageGenerator = nullptr;
	
	//CheatCode
	bool m_deleteStageFlag = false;
};

