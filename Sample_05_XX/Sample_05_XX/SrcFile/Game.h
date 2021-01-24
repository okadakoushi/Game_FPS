#pragma once

class Stage;
class GamePlayer;
class UIs;
class Effect;

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
	Level m_level;						//���x���B
	UIs* m_UIs = nullptr;				//UI�B
	myEngine::Effect* m_effect = nullptr;	//�G�t�F�N�g�B
};

