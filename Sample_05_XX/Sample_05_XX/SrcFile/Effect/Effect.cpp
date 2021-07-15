/// <summary>
/// エフェクト
/// DirectXⅢを参考に作成
/// </summary>

#include "stdafx.h"
#include "Effect.h"

namespace myEngine {
	Effect::Effect()
	{
	}
	Effect::~Effect()
	{
		//解放
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
		//ハッシュの作成
		int nameKey = Util::MakeHash(filePath);
		//リソースにエフェクトが登録されているかの検索
		m_effect = EffectEngineObj().GetEffectResourceManager().GetResource(nameKey);
		if (m_effect == nullptr) {
			//登録されていなかった
			//エフェクトの作成
			m_effect = &Effekseer::Effect::Create(EffectEngineObj().GetEffekseerManager(), (const EFK_CHAR*)filePath );
			if (m_effect == nullptr) {
				//ロード失敗！！
				//？？ファイルパス間違えてない？？
				throw;
			}
			//エフェクトをリソースに登録
			EffectEngineObj().GetEffectResourceManager().RegistResource(nameKey, m_effect);
		}
		m_handle = EffectEngineObj().GetEffekseerManager()->Play(effect, 0, 0, 0);
	}

	void Effect::Update()
	{
		//各行列定義
		Matrix mTrans, mRot, mScale, mBase;
		mTrans.MakeTranslation(m_position);
		mRot.MakeRotationFromQuaternion(m_rotation);
		mScale.MakeScaling(m_scale);
		//ワールド行列に変換
		mBase = mScale * mRot;
		mBase = mBase * mTrans;
		
		//エフェクトの座標を設定。
		EffectEngineObj().GetEffekseerManager()->SetBaseMatrix(m_handle, mBase);

		if (IsPlay() == false) {
			//再生が終了したら削除
			//DeleteGO(this);
		}

	}
}