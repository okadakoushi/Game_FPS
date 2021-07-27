#pragma once
#include "WaveFile.h"
#include "WaveFileBank.h"

#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//�ő�o�̓`�����l�����B
class WaveFile;
class SoundSource;

class SoundEngine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SoundEngine();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SoundEngine();
	/// <summary>
	///	������
	/// </summary>
	void Init();
	/// <summary>
	/// �����I�ȉ��
	/// </summary>
	void Release();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �T�E���h�\�[�X�̒ǉ��B
	/// </summary>
	/// <param name="s">�T�E���h�\�[�X</param>
	void AddSoundSource(SoundSource* s)
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it == m_soundSources.end()) {
			//���o�^�Ȃ�o�^����B
			m_soundSources.push_back(s);
		}
	}
	/// <summary>
	/// �T�E���h�\�[�X���폜�B
	/// </summary>
	/// <param name="s"></param>
	/// <returns>�폜�����v�f�̎����w���C�e���[�^�[</returns>
	auto RemoveSoundSource(SoundSource* s)
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it != m_soundSources.end()) {
			it = m_soundSources.erase(it);
		}
		return it;

	}
	/// <summary>
	/// XAudio2�̃\�[�X�{�C�X�̍쐬�B
	/// </summary>
	/// <param name="waveFile">�g�`�f�[�^</param>
	/// <param name="is3DSound">3D�T�E���h��</param>
	/// <returns></returns>
	IXAudio2SourceVoice* CreateXAudio2SourceVoice(WaveFile* waveFile, bool is3DSound);
	IXAudio2MasteringVoice* GetMasteringVoice()
	{
		return m_masteringVoice;
	}
	IXAudio2SubmixVoice* GetSubmixVoice()
	{
		return m_submixVoice;
	}

	/// <summary>
	/// �o�̓`�����l���̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetNumChannel() const
	{
		return m_nChannels;
	}

	/// <summary>
	/// �g�`�f�[�^�o���N���擾�B
	/// </summary>
	WaveFileBank& GetWaveFileBank()
	{
		return m_waveFileBank;
	}


private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	IUnknown* m_reverbEffect = nullptr;						//���o�[�v�G�t�F�N�g
	IXAudio2SubmixVoice* m_submixVoice = nullptr;			//�T�u�~�b�N�X�{�C�X
	DWORD m_channelMask = 0;								//�I�[�f�B�I�`�����l��
	DWORD m_nChannels = 0;									//�`�����l����
	bool m_isInited = false;								//������������
	std::list<SoundSource*>		m_soundSources;				//�T�E���h�\�[�X
	WaveFileBank m_waveFileBank;
};