#pragma once

#include "WaveFile.h"

/// <summary>
/// �����N���X�B
/// <para>���̃N���X���C���X�^���X������ƁA������</para>
/// <para>�T�E���h�G���W���ɓo�^����܂��B</para>
/// </summary>
class SoundSource
{
public:
	SoundSource();
	~SoundSource();
	/// <summary>
	/// ����B
	/// </summary>
	void Release();
	/// <summary>
	/// �������i�I���������j
	/// <para>�����ȃT�C�Y�̃I�[�f�B�I�͂������Ŋ�{�I�ɍĐ��B</para>
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	void Init(const wchar_t* filePath);
	/// <summary>
	/// �������i�X�g���[�~���O�Đ��j
	/// <para>�傫�ȃT�C�Y�̃I�[�f�B�I�͂������Ŋ�{�I�ɍĐ��B</para>
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	/// <param name="ringBufferSize">�����O�o�b�t�@�̃T�C�Y�B</param>
	/// <param name="bufferingSize">�X�g���[�~���O�Đ��B</param>
	void InitStreaming(wchar_t* filePath, unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);
	/// <summary>
	/// �Đ��B
	/// </summary>
	/// <param name="isLoop">���[�v������H</param>
	void Play(bool isLoop = false);
	/// <summary>
	/// ��~�B
	/// </summary>
	void Stop()
	{
		m_sourceVoice->Stop();
		m_isPlaying = false;
	}
	/// <summary>
	/// �ꎞ��~�B
	/// </summary>
	void Pause()
	{
		m_sourceVoice->Stop();
	}
	/// <summary>
	/// �Đ������̔���B
	/// </summary>
	/// <returns></returns>
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/// <summary>
	/// �{�����[���̐ݒ�B
	/// </summary>
	/// <param name="vol"></param>
	void SetVolume(float vol)
	{
		m_sourceVoice->SetVolume(vol);
	}
	/// <summary>
	/// ���[�v�t���O�̎擾
	/// </summary>
	/// <returns></returns>
	bool GetLoopFlag() const
	{
		return m_isLoop;
	}
	/// <summary>
	/// �{�C�X�̎��g��������
	/// </summary>
	/// <remarks>
	/// �ڍׂ�IXAudio2SourceVoice��SetFrequencyRatio���Q�Ƃ��Ă��������B
	/// </remarks>
	/// <param name="ratio">���g����</param>
	void SetFrequencyRatio(float ratio)
	{
		if (m_sourceVoice != nullptr) {
			m_sourceVoice->SetFrequencyRatio(ratio);
		}
	}
	IXAudio2SourceVoice* GetXAudio2SourceVoice()
	{
		return m_sourceVoice;
	}

	void Update();
private:
	void InitCommon();
	//�X�g���[�~���O�Đ����̍X�V
	void UpdateStreaming();
	//�I���������Đ����̍X�V
	void UpdateOnMemory();
	void Play(char* buff, unsigned int bufferSize);
	void StartStreamingBuffring();
private:
	enum EnStreamingStatus {
		enStreamingBuffering,	//�o�b�t�@�����O�B
		enStreamingQueueing,	//�L���[�C���O�B
	};
	std::shared_ptr<WaveFile>		m_waveFile;						//waveFile
	IXAudio2SourceVoice*			m_sourceVoice = nullptr;		//�\�[�X�{�C�X�B
	bool							m_isLoop = false;				//���[�v�t���O�B
	bool							m_isPlaying = false;			//�Đ����t���O�B
	bool							m_isStreaming = false;			//�X�g���[�~���O�B
	unsigned int					m_streamingBufferSize = 0;		//�X�g���[�~���O�p�o�b�t�@�����O�B
	unsigned int					m_currentBufferingSize = 0;		//���݂̃o�b�t�@�����O�T�C�Y�B
	unsigned int					m_readStartPos = 0;				//�ǂݍ��݊J�n�ʒu�B
	unsigned int					m_ringBufferSize = 0;			//�����O�o�b�t�@�T�C�Y�B
	EnStreamingStatus				m_streamingState = enStreamingBuffering;		//�X�g���[�~���O�o�b�t�@�����O�B
	FLOAT32							m_emitterAzimuths[INPUTCHANNELS];				
	FLOAT32							m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	bool							m_isAvailable = false;			//���p�\�B
};

