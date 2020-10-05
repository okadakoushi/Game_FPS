#pragma once

#include "tkFile/TkmFile.h"
#include "tkFile/TkaFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "srcFile/Animation/AnimationClip.h"
#include "srcFile/Animation/Animation.h"

class IShaderResource;

/// <summary>
/// ���f���̏������f�[�^
/// </summary>
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;		//tkm�t�@�C���p�X�B
	const char* m_vsEntryPointFunc = "VSMain";	//���_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_psEntryPointFunc = "PSMain";	//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_fxFilePath = nullptr;			//.fx�t�@�C���̃t�@�C���p�X�B
	const char* m_tkaFilePath = nullptr;		//tka�t�@�C���p�X�B
	void* m_expandConstantBuffer = nullptr;		//���[�U�[�g���̒萔�o�b�t�@�B
	int m_expandConstantBufferSize = 0;			//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView = nullptr;	//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
};

/// <summary>
/// ���f���̏������X�e�b�v�B
/// </summary>
/// <remarks>
/// ������case�Ǘ����A���f���̏�������i�߂�B
/// </remarks>
enum ModelInitStep {
	enInitStep_None,
	enInitStep_LoadModel,
	enInitStep_LoadSkelton,
	enInitStep_LoadAnimationClips,
	enInitStep_Completed,
	initStep
};


/// <summary>
/// ���f���N���X�B
/// </summary>
class Model {

public:

	/// <summary>
	/// ���f���������X�e�b�v�B
	/// </summary>
	/// <param name="initData">�������f�[�^�B</param>
	/// <returns>�X�e�b�v</returns>
	int Init( const ModelInitData& initData );
	/// <summary>
	/// �X�P���g���f�[�^�ǂݍ��݃X�e�b�v�B
	/// </summary>
	/// <param name="initData">�������f�[�^</param>
	/// <returns>�X�e�b�v</returns>
	int LoadTks(const ModelInitData& initData);
	/// <summary>
	/// �X�P���g���̏����������B
	/// </summary>
	/// <returns></returns>
	int initSkeleton();
	/// <summary>
	/// �A�j���[�V�����̏������B
	/// </summary>
	/// <param name="initData"></param>
	int InitAnim();
	/// <summary>
	/// ���[���h���W�ƃA�j���[�V�����̍X�V�B
	/// </summary>
	void UpdateMatAndAnim();
	/// <summary>
	/// ���[���h�s��̍X�V�B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// �X�P���g�����֘A�t����B
	/// </summary>
	/// <param name="skeleton">�X�P���g��</param>
	void BindSkeleton(Skeleton& skeleton)
	{
		m_meshParts.BindSkeleton(skeleton);
	}
	/// <summary>
	/// �A�j���[�V�������Đ��B
	/// </summary>
	/// <param name="animNo">�N���b�v�ԍ��B</param>
	/// <param name="interpolateTime">��ԗ��B</param>
	void PlayAnim(int animNo, float interpolateTime) {
		m_animation.Play(animNo, interpolateTime);
	}
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	void Draw(RenderContext& renderContext);
	/// <summary>
	/// ���[���h�s����擾�B
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}
private:
	using AnimationClipPtr = std:: unique_ptr<AnimationClip>;
	Matrix			m_world;		//���[���h�s��B
	TkmFile			m_tkmFile;		//tkm�t�@�C���B
	TkaFile			m_tkaFile;		//tka�t�@�C���B
	Skeleton		m_skeleton;		//�X�P���g���B
	MeshParts		m_meshParts;	//���b�V���p�[�c�B
	std::vector<std::string>		m_tkaFilePaths;		//tka�t�@�C���̃t�@�C���p�X���X�g�B
	std::vector<AnimationClipPtr>	m_animationClips;	//�A�j���[�V�����N���b�v�B
	Animation		m_animation;	//�A�j���[�V����
	Vector3	m_pos = Vector3::Zero;				//���W�B
	Vector3	m_scale = Vector3::One;				//�g�嗦�B
	Quaternion m_rot = Quaternion::Identity;	//��]�B
};
