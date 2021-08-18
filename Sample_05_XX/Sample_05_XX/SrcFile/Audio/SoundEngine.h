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
	/// 3dサウンドソースの追加。
	/// </summary>
	/// <param name="s">サウンドソース</param>
	void Add3DSoundSource(SoundSource* s)
	{
		m_3dSoundSources.push_back(s);
	}
	/// <summary>
	/// 3dサウンドソースを削除。
	/// </summary>
	/// <param name="s"></param>
	/// <returns>削除した要素の次を指すイテレーター</returns>
	auto Remove3DSoundSource(SoundSource* s)
	{
		auto it = std::find(m_3dSoundSources.begin(), m_3dSoundSources.end(), s);
		if (it != m_3dSoundSources.end()) {
			m_3dSoundSources.erase(it);
		}
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
	/// リスナーの位置を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetListnerPosition(const Vector3& pos)
	{
		// XAUDIOの座標の単位をmにする。
		m_listenerPosition = pos * 0.01f;
	}
	/// <summary>
	/// リスナーの前方向を設定。
	/// </summary>
	/// <param name="front"></param>
	void SetListenerFront(const Vector3& front)
	{
		m_listener.OrientFront.x = front.x;
		m_listener.OrientFront.y = front.y;
		m_listener.OrientFront.z = front.z;
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
	/// <summary>
	/// 初期化済みか。
	/// </summary>
	/// <returns></returns>
	const bool& IsInited() const 
	{
		return m_isInited;
	}

private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	IUnknown* m_reverbEffect = nullptr;						//リバープエフェクト
	IXAudio2SubmixVoice* m_submixVoice = nullptr;			//サブミックスボイス
	DWORD m_channelMask = 0;								//オーディオチャンネル
	DWORD m_nChannels = 0;									//チャンネル数
	bool m_isInited = false;								//初期化したか
	//3d
	X3DAUDIO_HANDLE m_hx3DAudio;							//3dAudio。
	X3DAUDIO_LISTENER m_listener;							//サウンドリスナー。
	std::list<SoundSource*>		m_3dSoundSources;			//3dサウンドソース
	Vector3 m_listenerPosition = g_vec3Zero;				//リスナーの位置。
	float m_flistenerAngle = 0.0f;							//リスナーアングル(float)。
	X3DAUDIO_CONE m_emitterCone;							//コーン(dspを計算する際のParam）
	WaveFileBank m_waveFileBank;
};