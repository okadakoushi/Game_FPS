#pragma once

class Stage;
class GamePlayer;
class UIs;
class Effect;
class Enemy;
class Rifle;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
private:
	Stage* m_stage = nullptr;			//�X�e�[�W�B
	GamePlayer* m_player = nullptr;		//�v���C���[�B
	Rifle* m_wepon;						//����B
	Level m_level;						//���x���B
	UIs* m_UIs = nullptr;				//UI�B
	Enemy* m_enemy = nullptr;			//�G�l�~�[�B
	myEngine::Effect* m_effect = nullptr;	//�G�t�F�N�g�B
};

