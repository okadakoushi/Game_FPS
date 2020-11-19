#pragma once
class Stage;
class GamePlayer;

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
};

