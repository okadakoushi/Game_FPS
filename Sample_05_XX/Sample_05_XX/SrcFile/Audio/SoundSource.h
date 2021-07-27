#pragma once

#include "WaveFile.h"

/// <summary>
/// 音源クラス。
/// <para>このクラスをインスタンス化すると、自動で</para>
/// <para>サウンドエンジンに登録されます。</para>
/// </summary>
class SoundSource
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
	void Init(const wchar_t* filePath);
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
	FLOAT32							m_emitterAzimuths[INPUTCHANNELS];				
	FLOAT32							m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	bool							m_isAvailable = false;			//利用可能。
};

