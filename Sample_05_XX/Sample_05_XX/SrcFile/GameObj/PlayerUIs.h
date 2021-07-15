#pragma once

class GamePlayer;
class Rifle;

class PlayerUIs : public IGameObject
{
public:
	PlayerUIs();
	~PlayerUIs();
	bool Start() override;
	void Update() override;
	void RenderHUD() override;
private:
	//Finds�B
	GamePlayer* m_player = nullptr;						//playerPtr�B
	//UIs�B
	SpriteRender*		m_reticule = nullptr;			//���e�B�N���B
	SpriteRender*		m_mainWeponImage = nullptr;		//���C���E�F�|��UI�B
	myEngine::CFont		m_hpUIText;						//HP�e�L�X�g�B
	myEngine::CFont		m_LeftAmoText;					//�c�e�e�L�X�g�B
	SpriteRender*		m_LeftAmoImage[2];				//3�̃X�v���C�g�Ŏc�e��\���B
	myEngine::CFont		m_LeftEnemyCount;				//�c��G���B
};

