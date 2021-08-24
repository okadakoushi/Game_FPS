#include "stdafx.h"
#include "SoundEngine.h"
#include "SoundSource.h"

#define NUM_PRESETS 30

namespace {
	//マイクロソフトのサンプルから引っ張ってきたサウンドコーン。
	// Specify sound cone to add directionality to listener for artistic effect:
	// Emitters behind the listener are defined here to be more attenuated,
	// have a lower LPF cutoff frequency,
	// yet have a slightly higher reverb send level.
	static const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI * 5.0f / 6.0f, X3DAUDIO_PI * 11.0f / 6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

	// Specify LFE level distance curve such that it rolls off much sooner than
	// all non-LFE channels, making use of the subwoofer more dramatic.
	static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

	// Specify reverb send level distance curve such that reverb send increases
	// slightly with distance before rolling off to silence.
	// With the direct channels being increasingly attenuated with distance,
	// this has the effect of increasing the reverb-to-direct sound ratio,
	// reinforcing the perception of distance.
	static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

	XAUDIO2FX_REVERB_I3DL2_PARAMETERS PRESET_PARAMS[NUM_PRESETS] =
	{
		XAUDIO2FX_I3DL2_PRESET_FOREST,
		XAUDIO2FX_I3DL2_PRESET_DEFAULT,
		XAUDIO2FX_I3DL2_PRESET_GENERIC,
		XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
		XAUDIO2FX_I3DL2_PRESET_ROOM,
		XAUDIO2FX_I3DL2_PRESET_BATHROOM,
		XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
		XAUDIO2FX_I3DL2_PRESET_STONEROOM,
		XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
		XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
		XAUDIO2FX_I3DL2_PRESET_CAVE,
		XAUDIO2FX_I3DL2_PRESET_ARENA,
		XAUDIO2FX_I3DL2_PRESET_HANGAR,
		XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
		XAUDIO2FX_I3DL2_PRESET_HALLWAY,
		XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
		XAUDIO2FX_I3DL2_PRESET_ALLEY,
		XAUDIO2FX_I3DL2_PRESET_CITY,
		XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
		XAUDIO2FX_I3DL2_PRESET_QUARRY,
		XAUDIO2FX_I3DL2_PRESET_PLAIN,
		XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
		XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
		XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
		XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
		XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
		XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
		XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
		XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
		XAUDIO2FX_I3DL2_PRESET_PLATE,
	};
}

SoundEngine::SoundEngine()
{
	memset(m_hx3DAudio, 0, sizeof(m_hx3DAudio));
}

SoundEngine::~SoundEngine()
{
	//解放
	Release();
}

void SoundEngine::Init()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	unsigned int flags = 0;

	//XAUDIO2の初期化
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&m_xAudio2, flags)))
	{
		return;
	}
	//マスターボリュームの作成
	if (FAILED(hr = m_xAudio2->CreateMasteringVoice(&m_masteringVoice)))
	{
		Release();
		return;
	}

	XAUDIO2_VOICE_DETAILS voiceDetails;
	m_masteringVoice->GetVoiceDetails(&voiceDetails);
	m_masteringVoice->GetChannelMask(&m_channelMask);

	m_nChannels = voiceDetails.InputChannels;

	//リバーブエフェクトの作成
	flags = 0;

	if (FAILED(hr = XAudio2CreateReverb(&m_reverbEffect, flags))) {
		Release();
		return;
	}
	//サブミックスボイスを作成
	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { m_reverbEffect, TRUE, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

	if (FAILED(hr = m_xAudio2->CreateSubmixVoice(&m_submixVoice, voiceDetails.InputChannels,
		voiceDetails.InputSampleRate, 0, 0,
		NULL, &effectChain)))
	{
		Release();
		return;
	}
	//デフォルトのFXパラメータを設定。
	XAUDIO2FX_REVERB_PARAMETERS native;
	ReverbConvertI3DL2ToNative(&PRESET_PARAMS[0], &native);
	m_submixVoice->SetEffectParameters(0, &native, sizeof(native));
	//3Dオーディオの初期化。
	const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;
	X3DAudioInitialize(m_channelMask, SPEEDOFSOUND, m_hx3DAudio);
	m_listener.Position = { 0.0f, 0.0f, 0.0f };
	m_listener.OrientFront = { 0.0f, 0.0f, 1.0f };
	m_listener.OrientTop = { 0.0f, 1.0f, 0.0f };

	//初期化完了。
	m_isInited = true;
}

void SoundEngine::Release()
{
	//波形データバンクを解放。
	m_waveFileBank.ReleaseAll();

	if (m_submixVoice != nullptr) {
		m_submixVoice->DestroyVoice();
		m_submixVoice = nullptr;
	}
	if (m_reverbEffect != nullptr) {
		m_reverbEffect->Release();
		m_reverbEffect = nullptr;
	}
	if (m_masteringVoice != nullptr) {
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = nullptr;
	}
	if (m_xAudio2 != nullptr) {
		m_xAudio2->Release();
		m_xAudio2 = nullptr;
	}


	CoUninitialize();
}

IXAudio2SourceVoice* SoundEngine::CreateXAudio2SourceVoice(WaveFile* waveFile, bool is3DSound)
{
	IXAudio2SourceVoice* pSourceVoice;

	if (!is3DSound) {
		//ソースボイスの作成。
		if (FAILED(m_xAudio2->CreateSourceVoice(&pSourceVoice, waveFile->GetFormat())))
		{
			return nullptr;
		}
	}
	else {
		//3Dサウンド。
		XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
		sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
		sendDescriptors[0].pOutputVoice = m_masteringVoice;
		sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
		sendDescriptors[1].pOutputVoice = m_submixVoice;
		const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };
		if (FAILED(m_xAudio2->CreateSourceVoice(&pSourceVoice, waveFile->GetFormat(), 0, 2.0f, NULL, &sendList)))
		{
			return nullptr;
		}
	}

	return pSourceVoice;
}

void SoundEngine::Update()
{
	if (!m_isInited) {
		return;
	}

	const Vector3& cameraForward = GraphicsEngineObj()->GetCamera3D().GetForward();
	const Vector3& cameraUp = GraphicsEngineObj()->GetCamera3D().GetUp();
	cameraForward.CopyTo(m_listener.OrientFront);
	cameraUp.CopyTo(m_listener.OrientTop);
	//m_flistenerAngle = float(atan2(m_listener.OrientFront.x, m_listener.OrientFront.z));

	if (m_listener.Position.x != m_listenerPosition.x
		|| m_listener.Position.z != m_listenerPosition.z)
	{
		
		//リスナーが動いている。
		Vector3 listenerPos;
		listenerPos.Set(m_listener.Position);
		//動いた分。
		Vector3 vDelta;
		vDelta.Subtract(m_listenerPosition, listenerPos);
		m_flistenerAngle = float(atan2(m_listener.OrientFront.x, m_listener.OrientFront.z));

		m_listener.pCone = NULL;
		//todoUserParam
		//m_listener.pCone
	}

	float deltaTime = GameTime().GetFrameDeltaTime();
	if (deltaTime > 0.0f) {
		//リスナー移動速度を計算。
		Vector3 vel;
		vel.Set(m_listener.Position);
		vel.Subtract(m_listenerPosition, vel);
		vel.Div(deltaTime);
		m_listenerPosition.CopyTo(m_listener.Position);
		vel.CopyTo(m_listener.Velocity);
	}

	DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
		| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
		| X3DAUDIO_CALCULATE_REVERB;

	//3dサウンドの計算。
	for (auto& soundSource : m_3dSoundSources) {
		//dsp
		X3DAUDIO_EMITTER emitter;
		
		emitter.pCone = &m_emitterCone;
		emitter.pCone->InnerAngle = 0.0f;
		// Setting the inner cone angles to X3DAUDIO_2PI and
		// outer cone other than 0 causes
		// the emitter to act like a point emitter using the
		// INNER cone settings only.
		emitter.pCone->OuterAngle = 0.0f;
		// Setting the outer cone angles to zero causes
		// the emitter to act like a point emitter using the
		// OUTER cone settings only.
		emitter.pCone->InnerVolume = 0.0f;
		emitter.pCone->OuterVolume = 1.0f;
		emitter.pCone->InnerLPF = 0.0f;
		emitter.pCone->OuterLPF = 1.0f;
		emitter.pCone->InnerReverb = 0.0f;
		emitter.pCone->OuterReverb = 1.0f;

		emitter.OrientFront = { 0, 0, 1 };
		emitter.OrientTop = { 0, 1, 0 };
		emitter.ChannelCount = INPUTCHANNELS;
		emitter.ChannelRadius = 1.0f;
		emitter.pChannelAzimuths = soundSource->GetEmitterAzimuths();

		// Use of Inner radius allows for smoother transitions as
		// a sound travels directly through, above, or below the listener.
		// It also may be used to give elevation cues.
		emitter.InnerRadius = 2.0f;
		emitter.InnerRadiusAngle = X3DAUDIO_PI / 4.0f;

		emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
		emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
		emitter.pLPFDirectCurve = NULL; // use default curve
		emitter.pLPFReverbCurve = NULL; // use default curve
		emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
		emitter.CurveDistanceScaler = 14.0f;
		emitter.DopplerScaler = 1.0f;

		soundSource->GetPosition().CopyTo(emitter.Position);
		soundSource->GetVelocity().CopyTo(emitter.Velocity);
		emitter.Position.x *= 0.01f;
		emitter.Position.y *= 0.01f;
		emitter.Position.z *= 0.01f;
		emitter.Velocity.x *= 0.01f;
		emitter.Velocity.y *= 0.01f;
		emitter.Velocity.z *= 0.01f;

		//todoUserRadius。
		emitter.InnerRadius = 0.0f;
		emitter.InnerRadiusAngle = 0.0f;

		X3DAUDIO_DSP_SETTINGS* dspSettings = soundSource->GetDspSettings();
		X3DAudioCalculate(m_hx3DAudio, &m_listener, &emitter, dwCalcFlags, dspSettings);

		IXAudio2SourceVoice* voice;
		voice = soundSource->GetXAudio2SourceVoice();

		if (voice != nullptr) {
			voice->SetFrequencyRatio(dspSettings->DopplerFactor);
			voice->SetOutputMatrix(m_masteringVoice, soundSource->GetNumInputChannel(), m_nChannels,
				soundSource->GetMatrixCoefficients());

			voice->SetOutputMatrix(m_submixVoice, soundSource->GetNumInputChannel(), 1, &dspSettings->ReverbLevel);

			XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * dspSettings->LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			voice->SetOutputFilterParameters(m_masteringVoice, &FilterParametersDirect);
			XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * dspSettings->LPFReverbCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			voice->SetOutputFilterParameters(m_submixVoice, &FilterParametersReverb);
		}

		Vector3 listenerToSound = m_listenerPosition - soundSource->GetPosition();
		float ListenerToSoundLen = listenerToSound.Length();
		if (fabsf(ListenerToSoundLen) >= emitter.CurveDistanceScaler * 2.5f) {
		}
		else {
			//printf("なる\n");
		}
		//printf("%f\n", ListenerToSoundLen);
		//printf("%f, %f, %f\n", soundSource->GetPosition().x, soundSource->GetPosition().y, soundSource->GetPosition().z);
	}
}