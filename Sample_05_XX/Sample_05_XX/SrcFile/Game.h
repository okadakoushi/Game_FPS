#pragma once

class Stage;
class GamePlayer;
class UIs;
class Effect;
class SoldierMob;
class RifleEnemy;
class Rifle;
class GameCamera;
class Map;
class SkyBox;
class Guide;
class StageGenerator;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
	void RenderHUD() override;

private:
	StageGenerator* m_stageGenerator = nullptr;	//�X�e�[�W�����I�u�W�F�N�g�B
	Stage* m_stage = nullptr;			//�X�e�[�W�B
	GamePlayer* m_player = nullptr;		//�v���C���[�B
	GameCamera* m_camera = nullptr;		//�J�����B
	Rifle* m_wepon;						//����B
	RifleEnemy* m_enemy = nullptr;		//�G�B	
	Level m_level;						//���x���B
	UIs* m_UIs = nullptr;				//UI�B
	Map* m_map;							//�~�j�}�b�v�B
	SkyBox* m_sky = nullptr;			//��B
	myEngine::Effect* m_effect = nullptr;			//�G�t�F�N�g�B
	myEngine::CFont m_font;
};
