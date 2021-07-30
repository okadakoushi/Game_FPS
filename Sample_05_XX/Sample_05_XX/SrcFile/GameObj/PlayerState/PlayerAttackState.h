#pragma once

#include "IPlayerState.h"

class PlayerAttackState : public IPlayerState
{
public:
	PlayerAttackState(GamePlayer* player) :
		IPlayerState(player)
	{
		//SE�������B
		m_shootSE = NewGO<SoundSource>(0);
		m_shootSE->Init(L"Assets/Audio/AK47Shoot.wav");
		//�G�t�F�N�g�����B
		m_effect = NewGO<myEngine::Effect>(EnPriority_3DModel);
		m_effect->SetScale({ 3.0f, 3.0f, 3.0f });
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
private:
	const float RAY_RANGE = 8000.0f;
	SoundSource* m_shootSE = nullptr;			//�e�̔��ˉ��B
	myEngine::Effect* m_effect = nullptr;		//�G�t�F�N�g�B
};

