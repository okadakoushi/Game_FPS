#pragma once

class GamePlayer;

/// <summary>
/// �v���C���[�̃X�e�[�g���N���X�B
/// </summary>
class IPlayerState {
public:
	IPlayerState(GamePlayer* player) : 
		m_player(player)
	{
	}
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Leave() = 0;
protected:
	GamePlayer* m_player = nullptr;
};