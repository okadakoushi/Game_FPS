#pragma once
#define _CRT_SECURE_NO_WARNINGS

//#define GAME
//#define PHYSICS_DEBUG


#ifndef MASTER
	#define NAV_DEBUG
#endif

const float DELTA_TIME = 1.0f / 120.0f;	//更新時間。後でGameTimeクラス作ること。
const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの数。
static const int NUM_SHADOW_MAP = 3;			//カスケードシャドウマップの数。

//昇順。小さいほうが早い。
static enum Priority {
	//EnPriority_3DRender,
	EnPriority_Game,
	EnPriority_Generator,
	EnPriority_3DModel,		//モデル。
	EnPriority_UI,			//UI(HDD描画等)。
	EnPriority_2DRender,
	Enpriority_Camera,
	EnPriority_Num = 9		//todo:マジックナンバー。
};

//オーディオのインクルード
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>

//physics
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

#include "Effekseer.h"
#include "EffekseerRendererDX12.h"
//utill
#include "SrcFile/util/Noncopyable.h"
//ゲームオブジェクト。
#include "SrcFile/GameObject/IGameObject.h"

#include "MiniEngine.h"

#include "SrcFile/InputKeyCode.h"

#include "SrcFile/Primitive.h"

#include "SrcFile/Physics/PhysicsWorld.h"

#include "Skeleton.h"

#include "SrcFile/2D/FontEngine.h"
//#include "SrcFile/util/Util.h"
#include "SrcFile/util/TNameKey.h"
#include "SrcFile/Timer/tkStopwatch.h"
#include "SrcFile/Timer/GameTime.h"

#include "SrcFile/GameObject/GameObjectManager.h"

#include "SrcFile/Audio/SoundSource.h"

//レンダー。
#include "SrcFile/SkinModelRender.h"
#include "SrcFile/SpriteRender.h"
#include "SrcFile/2D/Font.h"
#include "SrcFile/2D/FontRender.h"

//シャドウマップ。
#include "SrcFile/Shadow/DirectionalShadowMap.h"
//GBuffer
#include "SrcFile/GBuffer/GBufferRender.h"
//Physics
#include "SrcFile/Character/CharacterController.h"
#include "SrcFile/Physics/BoxCollider.h"
#include "SrcFile/Physics/PhysicsStaticObject.h"
//レベル関連。
#include "SrcFile/Level/MapChip.h"
#include "SrcFile/Level/Level.h"
//エフェクト。
#include "SrcFile/Effect/Effect.h"
//ナビ。
#include "SrcFile/NaviMesh/NaviMesh.h"
#include "SrcFile/NaviMesh/Astar.h"
#include "SrcFile/NaviMesh/NaviMeshAgent.h"
#include "SrcFile/Physics/PhysicsDebugDraw.h"

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
