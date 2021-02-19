#pragma once
#define _CRT_SECURE_NO_WARNINGS

const float DELTA_TIME = 1.0f / 120.0f;	//�X�V���ԁB���GameTime�N���X��邱�ƁB
const int NUM_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̐��B
static const int NUM_SHADOW_MAP = 3;			//�J�X�P�[�h�V���h�E�}�b�v�̐��B

//#define MODE_DEBUG

//�����B�������ق��������B
static enum Priority {
	EnPriority_Game,
	Enpriority_Camera,
	EnPriority_3DModel,		//���f���B
	EnPriority_3DRender,
	EnPriority_UI,			//UI(HDD�`�擙)�B
	EnPriority_2DRender,
	EnPriority_Num = 9		//todo:�}�W�b�N�i���o�[�B
};

#include "Effekseer.h"
#include "EffekseerRendererDX12.h"
//utill
#include "SrcFile/util/Noncopyable.h"
//�Q�[���I�u�W�F�N�g�B
#include "SrcFile/GameObject/IGameObject.h"

#include "MiniEngine.h"

//#include "SrcFile/util/Util.h"
#include "SrcFile/util/TNameKey.h"

#include "SrcFile/GameObject/GameObjectManager.h"

//�����_�[�B
#include "SrcFile/SkinModelRender.h"
#include "SrcFile/SpriteRender.h"

//�V���h�E�}�b�v�B
#include "SrcFile/Shadow/DirectionalShadowMap.h"
//GBuffer
#include "SrcFile/GBuffer/GBufferRender.h"
//���x���֘A�B
#include "SrcFile/Level/MapChip.h"
#include "SrcFile/Level/Level.h"
//�G�t�F�N�g�B
#include "SrcFile/Effect/Effect.h"
//�i�r�B
#include "SrcFile/NaviMesh/NaviMesh.h"
#include "AStar.h"

using namespace std;

/// <summary>
/// �f�B���N�V�������C�g�B
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//���C�g�̕����B
	float pad0;			//�p�f�B���O�B
	Vector4 color;		//���C�g�̃J���[�B
};
/// <summary>
/// ���C�g�\���́B
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//�f�B���N�V�������C�g�B
	Vector3 eyePos;					//�J�����̈ʒu�B
	float specPow;					//�X�y�L�����̍i��B
	Vector3 ambinetLight;			//�����B
};
