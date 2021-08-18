#pragma once

#include "WaveFile.h"

/// <summary>
/// 音源クラス。
/// </summary>
class SoundSource : public IGameObject
{
public:
	SoundSource();
	~SoundSource();
	/// <summary>
	/// 解放。
	/// </summary>
	void Release();
	/// <summary>
	/// 初期化（オンメモリ）
	/// <para>小さなサイズのオーディオはこっちで基本的に再生。</para>
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	void Init(const wchar_t* filePath, bool is3dSound = false);
	/// <summary>
	/// 初期化（ストリーミング再生）
	/// <para>大きなサイズのオーディオはこっちで基本的に再生。</para>
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	/// <param name="ringBufferSize">リングバッファのサイズ。</param>
	/// <param name="bufferingSize">ストリーミング再生。</param>
	void InitStreaming(wchar_t* filePath, unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);
	/// <summary>
	/// 再生。
	/// </summary>
	/// <param name="isLoop">ループさせる？</param>
	void Play(bool isLoop = false);
	/// <summary>
	/// 停止。
	/// </summary>
	void Stop()
	{
		m_sourceVoice->Stop();
		m_isPlaying = false;
	}
	/// <summary>
	/// 一時停止。
	/// </summary>
	void Pause()
	{
		m_sourceVoice->Stop();
	}
	/// <summary>
	/// 再生中かの判定。
	/// </summary>
	/// <returns></returns>
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/// <summary>
	/// ボリュームの設定。
	/// </summary>
	/// <param name="vol"></param>
	void SetVolume(float vol)
	{
		m_sourceVoice->SetVolume(vol);
	}
	/// <summary>
	/// ループフラグの取得
	/// </summary>
	/// <returns></returns>
	bool GetLoopFlag() const
	{
		return m_isLoop;
	}
	/// <summary>
	/// 音源の座標を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		if (m_isSetPositionFirst) {
			m_lastFramePosition = m_position;
			m_isSetPositionFirst = false;
		}
	}
	/// <summary>
	/// 音源の座標習得。
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 入力チャンネル数を取得。
	/// </summary>
	/// <returns></returns>
	int GetNumInputChannel() const
	{
		return m_waveFile->GetFormat()->nChannels;
	}
	/// <summary>
	/// 音源の移動速度。
	/// </summary>
	/// <returns></returns>
	Vector3 GetVelocity() const
	{
		return m_velocity;
	}
	FLOAT32* GetEmitterAzimuths()
	{
		return m_emitterAzimuths;
	}
	FLOAT32* GetMatrixCoefficients()
	{
		return m_matrixCoefficients;
	}
	/// <summary>
	/// dspSettingを取得。
	/// </summary>
	/// <returns></returns>
	X3DAUDIO_DSP_SETTINGS* GetDspSettings()
	{
		return &m_dspSettings;
	}
	/// <summary>
	/// ボイスの周波数調整比
	/// </summary>
	/// <remarks>
	/// 詳細はIXAudio2SourceVoiceのSetFrequencyRatioを参照してください。
	/// </remarks>
	/// <param name="ratio">周波数比</param>
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
	//ストリーミング再生中の更新
	void UpdateStreaming();
	//オンメモリ再生中の更新
	void UpdateOnMemory();
	void Play(char* buff, unsigned int bufferSize);
	void StartStreamingBuffring();
	void Remove3DSound();
private:
	enum EnStreamingStatus {
		enStreamingBuffering,	//バッファリング。
		enStreamingQueueing,	//キューイング。
	};
	std::shared_ptr<WaveFile>		m_waveFile;						//waveFile
	IXAudio2SourceVoice*			m_sourceVoice = nullptr;		//ソースボイス。
	bool							m_isLoop = false;				//ループフラグ。
	bool							m_isPlaying = false;			//再生中フラグ。
	bool							m_isStreaming = false;			//ストリーミング。
	unsigned int					m_streamingBufferSize = 0;		//ストリーミング用バッファリング。
	unsigned int					m_currentBufferingSize = 0;		//現在のバッファリングサイズ。
	unsigned int					m_readStartPos = 0;				//読み込み開始位置。
	unsigned int					m_ringBufferSize = 0;			//リングバッファサイズ。
	EnStreamingStatus				m_streamingState = enStreamingBuffering;		//ストリーミングバッファリング。
	bool							m_isAvailable = false;			//利用可能。
	//3d
	Vector3							m_position = g_vec3Zero;			//音源座標。
	Vector3							m_lastFramePosition = g_vec3Zero;	//音源の1フレーム前の座標。
	Vector3							m_velocity = g_vec3Zero;			//速度。
	bool							m_isSetPositionFirst = true;		//1番最初のSET。
	FLOAT32							m_emitterAzimuths[INPUTCHANNELS];	//方位。				
	FLOAT32							m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	X3DAUDIO_DSP_SETTINGS			m_dspSettings;
	bool							m_is3dSound = false;				//3Dサウンド？
};

