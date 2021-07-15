/// <summary>
/// �G�t�F�N�g
/// DirectX�V���Q�l�ɍ쐬
/// </summary>

#include "stdafx.h"
#include "Effect.h"

namespace myEngine {
	Effect::Effect()
	{
	}
	Effect::~Effect()
	{
		//���
		Release();
	}
	
	void Effect::Release()
	{
		if (m_handle != -1) {
			EffectEngineObj().GetEffekseerManager()->StopEffect(m_handle);
			m_handle = -1;
		}
	}

	void Effect::Play(const wchar_t* filePath)
	{
		auto effect = EffectEngineObj().CreateEffect(filePath);
		//�n�b�V���̍쐬
		int nameKey = Util::MakeHash(filePath);
		//���\�[�X�ɃG�t�F�N�g���o�^����Ă��邩�̌���
		m_effect = EffectEngineObj().GetEffectResourceManager().GetResource(nameKey);
		if (m_effect == nullptr) {
			//�o�^����Ă��Ȃ�����
			//�G�t�F�N�g�̍쐬
			m_effect = &Effekseer::Effect::Create(EffectEngineObj().GetEffekseerManager(), (const EFK_CHAR*)filePath );
			if (m_effect == nullptr) {
				//���[�h���s�I�I
				//�H�H�t�@�C���p�X�ԈႦ�ĂȂ��H�H
				throw;
			}
			//�G�t�F�N�g�����\�[�X�ɓo�^
			EffectEngineObj().GetEffectResourceManager().RegistResource(nameKey, m_effect);
		}
		m_handle = EffectEngineObj().GetEffekseerManager()->Play(effect, 0, 0, 0);
	}

	void Effect::Update()
	{
		//�e�s���`
		Matrix mTrans, mRot, mScale, mBase;
		mTrans.MakeTranslation(m_position);
		mRot.MakeRotationFromQuaternion(m_rotation);
		mScale.MakeScaling(m_scale);
		//���[���h�s��ɕϊ�
		mBase = mScale * mRot;
		mBase = mBase * mTrans;
		
		//�G�t�F�N�g�̍��W��ݒ�B
		EffectEngineObj().GetEffekseerManager()->SetBaseMatrix(m_handle, mBase);

		if (IsPlay() == false) {
			//�Đ����I��������폜
			//DeleteGO(this);
		}

	}
}