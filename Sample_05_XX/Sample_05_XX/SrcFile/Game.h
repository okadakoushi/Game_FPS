#pragma once

class Stage;
class GamePlayer;
class UIs;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
private:
	Stage* m_stage = nullptr;		//�X�e�[�W�B
	GamePlayer* m_player = nullptr;	//�v���C���[�B
	Level m_level;					//���x���B
	UIs* m_UIs;						//UI�B
};

