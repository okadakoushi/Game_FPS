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
	/// 3d�T�E���h�\�[�X�̒ǉ��B
	/// </summary>
	/// <param name="s">�T�E���h�\�[�X</param>
	void Add3DSoundSource(SoundSource* s)
	{
		m_3dSoundSources.push_back(s);
	}
	/// <summary>
	/// 3d�T�E���h�\�[�X���폜�B
	/// </summary>
	/// <param name="s"></param>
	/// <returns>�폜�����v�f�̎����w���C�e���[�^�[</returns>
	auto Remove3DSoundSource(SoundSource* s)
	{
		auto it = std::find(m_3dSoundSources.begin(), m_3dSoundSources.end(), s);
		if (it != m_3dSoundSources.end()) {
			m_3dSoundSources.erase(it);
		}
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
	/// ���X�i�[�̈ʒu��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetListnerPosition(const Vector3& pos)
	{
		// XAUDIO�̍��W�̒P�ʂ�m�ɂ���B
		m_listenerPosition = pos * 0.01f;
	}
	/// <summary>
	/// ���X�i�[�̑O������ݒ�B
	/// </summary>
	/// <param name="front"></param>
	void SetListenerFront(const Vector3& front)
	{
		m_listener.OrientFront.x = front.x;
		m_listener.OrientFront.y = front.y;
		m_listener.OrientFront.z = front.z;
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
	/// <summary>
	/// �������ς݂��B
	/// </summary>
	/// <returns></returns>
	const bool& IsInited() const 
	{
		return m_isInited;
	}

private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	IUnknown* m_reverbEffect = nullptr;						//���o�[�v�G�t�F�N�g
	IXAudio2SubmixVoice* m_submixVoice = nullptr;			//�T�u�~�b�N�X�{�C�X
	DWORD m_channelMask = 0;								//�I�[�f�B�I�`�����l��
	DWORD m_nChannels = 0;									//�`�����l����
	bool m_isInited = false;								//������������
	//3d
	X3DAUDIO_HANDLE m_hx3DAudio;							//3dAudio�B
	X3DAUDIO_LISTENER m_listener;							//�T�E���h���X�i�[�B
	std::list<SoundSource*>		m_3dSoundSources;			//3d�T�E���h�\�[�X
	Vector3 m_listenerPosition = g_vec3Zero;				//���X�i�[�̈ʒu�B
	float m_flistenerAngle = 0.0f;							//���X�i�[�A���O��(float)�B
	X3DAUDIO_CONE m_emitterCone;							//�R�[��(dsp���v�Z����ۂ�Param�j
	WaveFileBank m_waveFileBank;
};