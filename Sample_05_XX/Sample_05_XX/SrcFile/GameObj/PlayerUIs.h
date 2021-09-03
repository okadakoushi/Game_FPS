#pragma once

class GamePlayer;
class Rifle;
class RifleEnemy;

class PlayerUIs : public IGameObject
{
public:
	PlayerUIs();
	~PlayerUIs();
	bool Start() override;
	void Update() override;
	void RenderHUD() override;
public:
	/// <summary>
	/// �v���C���[�������Ă���G�l�~�[��ǉ��B
	/// </summary>
	/// <param name="enemy">Enemy</param>
	void AddFindList(RifleEnemy* enemy)
	{
		m_findPlayerEnemyList.push_back(enemy);
	}

	void ActiveProcess(bool flag);
private:
	//Finds�B
	GamePlayer* m_player = nullptr;								//playerPtr�B
	//UIs�B
	SpriteRender*				m_reticule = nullptr;			//���e�B�N���B
	SpriteRender*				m_mainWeponImage = nullptr;		//���C���E�F�|��UI�B
	SpriteRender*				m_damageEffectSprite = nullptr;	//�_���[�W�G�t�F�N�g�̃X�v���C�g�B
	SpriteRender*				m_LeftAmoImage[3];				//3�̃X�v���C�g�Ŏc�e��\���B
	SpriteRender*				m_uiBoxForHP = nullptr;			//HP�pUIBOX�B
	SpriteRender*				m_uiBoxForAmo = nullptr;		//Amo�pUIBOX�B
	Sprite						m_enemyDirectionSprite[5];		//�G�l�~�[�̕����X�v���C�g�B���X�g�̍폜�𑀍삷�邽��NotRender�B
	//texts�B
	myEngine::CFontRender*		m_hpUIText = nullptr;			//HP�e�L�X�g�B
	wchar_t						m_hpTex[16];					//HP�e�L�X�g�i�[�p�B
	myEngine::CFontRender*		m_LeftAmoText = nullptr;		//�c�e�e�L�X�g�B
	wchar_t						m_leftAmoTex[16];				//�c�e�e�L�X�g�i�[�p�B
	myEngine::CFontRender*		m_LeftEnemyCount = nullptr;		//�c��G���B
	wchar_t						m_leftEnemyCountTex[16];		//�c��G���e�L�X�g�i�[�p�B
	//Param�B
	float						m_damageEffectArea = 0.0f;		//�_���[�W�G�t�F�N�g�G���A�B
	std::vector<RifleEnemy*>	m_findPlayerEnemyList;			//�v���C���[�𔭌����Ă���G�l�~�[�̃��X�g�B
	const Vector2				m_saveFrame = { 30.0f, 30.0f };	//�Z�[�u�t���[���B
};

