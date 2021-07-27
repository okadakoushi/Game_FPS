#pragma once
#include "WaveFile.h"
#include "WaveFileBank.h"

#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//最大出力チャンネル数。
class WaveFile;
class SoundSource;

class SoundEngine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SoundEngine();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SoundEngine();
	/// <summary>
	///	初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 明示的な解放
	/// </summary>
	void Release();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// サウンドソースの追加。
	/// </summary>
	/// <param name="s">サウンドソース</param>
	void AddSoundSource(SoundSource* s)
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it == m_soundSources.end()) {
			//未登録なら登録する。
			m_soundSources.push_back(s);
		}
	}
	/// <summary>
	/// サウンドソースを削除。
	/// </summary>
	/// <param name="s"></param>
	/// <returns>削除した要素の次を指すイテレーター</returns>
	auto RemoveSoundSource(SoundSource* s)
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it != m_soundSources.end()) {
			it = m_soundSources.erase(it);
		}
		return it;

	}
	/// <summary>
	/// XAudio2のソースボイスの作成。
	/// </summary>
	/// <param name="waveFile">波形データ</param>
	/// <param name="is3DSound">3Dサウンドか</param>
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
	/// 出力チャンネルの数を取得。
	/// </summary>
	/// <returns></returns>
	int GetNumChannel() const
	{
		return m_nChannels;
	}

	/// <summary>
	/// 波形データバンクを取得。
	/// </summary>
	WaveFileBank& GetWaveFileBank()
	{
		return m_waveFileBank;
	}


private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	IUnknown* m_reverbEffect = nullptr;						//リバープエフェクト
	IXAudio2SubmixVoice* m_submixVoice = nullptr;			//サブミックスボイス
	DWORD m_channelMask = 0;								//オーディオチャンネル
	DWORD m_nChannels = 0;									//チャンネル数
	bool m_isInited = false;								//初期化したか
	std::list<SoundSource*>		m_soundSources;				//サウンドソース
	WaveFileBank m_waveFileBank;
};