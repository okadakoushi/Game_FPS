#pragma once
#include "SrcFile/util/Noncopyable.h"
#include "Model.h"
#include "RenderTarget.h"

/// <summary>
/// �J�X�P�[�h�V���h�E�}�b�v�B
/// </summary>
/// <code>
/// --func call step--
/// 1.RegisterShadowCaster();
/// 2.Update();	�v���P�B
/// 3.RenderToShadowMap();
/// </code>
class DirectionalShadowMap : Noncopyable
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="w">���̉𑜓x�B</param>
	/// <param name="h">�c�̉𑜓x�B</param>
	/// <param name="lightHeight">���z�̍����B</param>
	void Init(int w, int h, float lightHeight);
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
	/// <summary>
	/// ���C�g�̈ʒu���v�Z�B
	/// </summary>
	/// <param name="lightHeight">���C�g�J�����̍����B</param>
	/// <param name="viewFrustomCenterPosition">������̒��S�B</param>
	Vector3 CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition);
	/// <summary>
	/// �V���h�E�}�b�v��`��
	/// </summary>
	/// <remarks>
	/// �����Ăяo���Ǝ����I�ɃV���h�E�}�b�v�p��RenderTarget�ɕύX����B
	/// �`�悪�I��莟��A���̕`�悵�Ă����t���[���o�b�t�@�ɖ߂�܂��B
	/// ��X�A�X�L�����f�������_�[�ŏ���ɌĂ΂�鏈���ɂ���B
	/// </remarks>
	/// <param name="rc"></param>
	void RenderToShadowMap();
	/// <summary>
	/// �V���h�E�L���X�^�[�Ƃ��ēo�^�B
	/// </summary>
	/// <param name="model">�o�^���郂�f���B</param>
	void RegisterShadowCaster(Model* model)
	{
		m_shadowCasters.push_back(model);
		//�V���h�E�L���X�^�̃t���O�B
		model->SetShadwoCaster();
		//�V���h�E�����ŁI
		m_isEnable = true;
	}

private:
	static const int NUM_SHADOW_MAP = 3;			//�J�X�P�[�h�V���h�E�}�b�v�̐��B

	struct SShadowCb {
		Matrix mLVP[NUM_SHADOW_MAP];						//���C�g�r���[�v���W�F�N�V�����B
		float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//�J������Ԃł̉e�𗎂Ƃ��G���A�̐[�x�e�[�u���B
	};
	RenderTarget m_shadowMaps[NUM_SHADOW_MAP];	//�V���h�E�}�b�v�B
	std::vector<Model*> m_shadowCasters;		//�V���h�E�L���X�^�[
	Matrix m_LVPMatrix[NUM_SHADOW_MAP];			//���C�g�r���[�v���W�F�N�V�����s��B
	Matrix m_lightViewMatrix[NUM_SHADOW_MAP];	//���C�g�r���[�s��
	Matrix m_lightProjMatirx[NUM_SHADOW_MAP];	//���C�g�v���W�F�N�V�����s��
	SShadowCb m_shadowCBEntity;					//�V���h�E�̒l�B
	ConstantBuffer m_shadowCB;					//�V���h�E�̒萔�o�b�t�@�B
	Vector3 m_lightDirection = Vector3::Down;	//���C�g�̕����B
	float m_shadowAreas[NUM_SHADOW_MAP];		//�e�̗�����͈́B
	float m_lightHeight;						//���C�g�̍����B
	bool m_isEnable = false;					//�V���h�E�}�b�v���L���H
};

