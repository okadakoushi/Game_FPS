#pragma once
#define _CRT_SECURE_NO_WARNINGS

const float DELTA_TIME = 1.0f / 120.0f;	//更新時間。後でGameTimeクラス作ること。
const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの数。
static const int NUM_SHADOW_MAP = 3;			//カスケードシャドウマップの数。

//#define MODE_DEBUG

//昇順。小さいほうが早い。
static enum Priority {
	EnPriority_Game,
	Enpriority_Camera,
	EnPriority_3DModel,		//モデル。
	EnPriority_3DRender,
	EnPriority_UI,			//UI(HDD描画等)。
	EnPriority_2DRender,
	EnPriority_Num = 9		//todo:マジックナンバー。
};

#include "Effekseer.h"
#include "EffekseerRendererDX12.h"
//utill
#include "SrcFile/util/Noncopyable.h"
//ゲームオブジェクト。
#include "SrcFile/GameObject/IGameObject.h"

#include "MiniEngine.h"

//#include "SrcFile/util/Util.h"
#include "SrcFile/util/TNameKey.h"

#include "SrcFile/GameObject/GameObjectManager.h"

//レンダー。
#include "SrcFile/SkinModelRender.h"
#include "SrcFile/SpriteRender.h"

//シャドウマップ。
#include "SrcFile/Shadow/DirectionalShadowMap.h"
//GBuffer
#include "SrcFile/GBuffer/GBufferRender.h"
//レベル関連。
#include "SrcFile/Level/MapChip.h"
#include "SrcFile/Level/Level.h"
//エフェクト。
#include "SrcFile/Effect/Effect.h"
//ナビ。
#include "SrcFile/NaviMesh/NaviMesh.h"
#include "AStar.h"

using namespace std;

/// <summary>
/// ディレクションライト。
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//ライトの方向。
	float pad0;			//パディング。
	Vector4 color;		//ライトのカラー。
};
/// <summary>
/// ライト構造体。
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//ディレクションライト。
	Vector3 eyePos;					//カメラの位置。
	float specPow;					//スペキュラの絞り。
	Vector3 ambinetLight;			//環境光。
};
