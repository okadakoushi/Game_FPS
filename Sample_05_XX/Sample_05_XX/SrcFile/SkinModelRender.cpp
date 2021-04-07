#include "stdafx.h"
#include "SkinModelRender.h"
#include <fstream>

SkinModelRender::SkinModelRender()
{
}

void SkinModelRender::OnDestroy()
{
	//モデルレンダー消えたからGbufferのリストから削除する。
	GraphicsEngineObj()->GetGBuffer().RemoveModel(this);
}

void SkinModelRender::AnimInit()
{
	bool inited = false;

	while (!inited) {
		switch (m_initStep)
		{
		case enInitStep_None:
			//モデルの初期化まち。
			break;
		case enInitStep_LoadSkelton:
			//swich caseによりコンストラクタがスキップされることがあるので
			//{...}で囲む。
		{
			//スケルトンをロードする。
			//モデルの初期化、メッシュ作成が終わっている。
			//スケルトンデータの読み込み。
			std::string skeltonFilePath = m_filePath;
			int pos = (int)skeltonFilePath.find(".tkm");
			//.tkmを.tksに置き換え。
			skeltonFilePath.replace(pos, 4, ".tks");
			//ファイルストリーム。
			std::ifstream ifs(skeltonFilePath);
			if (ifs.is_open() && m_renderMode != enRenderMode_NonSkin) {
				//tksファイルがあるので読み込む。
				m_skeleton.Init(skeltonFilePath.c_str());
				m_initStep = enInitStep_LoadAnimation;
			}
			else {
				//なかった。
				m_initStep = enInitStep_Completed;
			}
		}
		break;
		case enInitStep_LoadAnimation:
			//バインドポーズとかの計算。
			m_skeleton.BuildBoneMatrices();
			//スケルトンとモデルの関連付け。
			if (m_skeleton.IsInited()) {
				//スケルトンが初期化されてた。
				//モデルとスケルトンを結び付け。
				m_model.BindSkeleton(m_skeleton);
			}

			//アニメーションクリップのロード。
			if (m_tkaFilePaths.empty() == false) {
				//アニメーションクリップがあった。
				for (auto& m_tkaFilePath : m_tkaFilePaths) {
					//アニメーションのファイルパス分回す。
					auto animClip = make_unique<AnimationClip>();
					//ファイルパスをロード。
					animClip->Load(m_tkaFilePath.c_str());
					//アニメーションクリップにメモリを移譲。
					m_animationClips.push_back(move(animClip));
				}
			}

		case enInitStep_InitAnimationClip: {
			bool isLoaded = true;
			for (auto& animClip : m_animationClips) {
				if (animClip->IsLoaded() == false) {
					isLoaded = false;
					break;
				}
			}
			if (isLoaded == true) {
				//全アニメーションクリップのロード終了。
				//キーフレームとアニメーションクリップの構築。
				for (auto& animClip : m_animationClips) {
					animClip->SetLoopFlag(true);
					animClip->BuildKeyFrames();
				}
				//アニメーションを初期化。
				m_animation.Init(m_skeleton, m_animationClips);

				//ループフラグ設定。
				for (auto& list : m_loopMap) {
					//２次元配列でいいなこれ。
					m_animation.SetAnimationClipLoopFlag(list.first, list.second);
				}
				//アニメーションクリップのロード終了。
				m_initStep = enInitStep_Completed;
			}
		}
		break;
		case enInitStep_Completed:
			//初期化終わり！
			inited = true;
			break;
		}
	}
}

void SkinModelRender::Update()
{
	if (m_animation.IsInited()) {
		//アニメーション初期化されてた。
		m_animation.Progress(DELTA_TIME, m_waitTime);
	}
	//ワールド座標更新。
	m_model.Update(m_pos, m_rot, m_scale, m_renderMode);

	if (!m_isForwardRender) {
		//フォワードレンダーしないのでGBuffer描画。
		GraphicsEngineObj()->GetGBuffer().AddModel(this);
	}

	if (m_skeleton.IsInited()) {
		//スケルトン初期化されてた。
		m_skeleton.Update(m_model.GetWorldMatrix());
	}
}

void SkinModelRender::ForwardRender()
{
	auto& rc = GraphicsEngineObj()->GetRenderContext();
	auto& camera3D = GraphicsEngineObj()->GetCamera3D();
	//モデルのDraw
	if (m_isForwardRender) {
		m_model.Draw(rc, camera3D.GetViewMatrix(), camera3D.GetProjectionMatrix(), m_renderMode);
	}
}

