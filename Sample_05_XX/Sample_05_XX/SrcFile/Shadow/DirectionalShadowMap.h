#pragma once

/// <summary>
/// �J�X�P�[�h�V���h�E�}�b�v�B
/// </summary>
class DirectionalShadowMap : Noncopyable
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="w"></param>
	/// <param name="h"></param>
	/// <param name="lightHeight"></param>
	void Init(int w, int h, float lightHeight);

	void RenderToShadowMap(RenderContext& rc);

private:
	struct SShadowCb {
		Matrix mLVP[NUM_SHADOW_MAP];						//���C�g�r���[�v���W�F�N�V�����B
		float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//�J������Ԃł̉e�𗎂Ƃ��G���A�̐[�x�e�[�u���B
	};
	RenderTarget m_shadowMaps[NUM_SHADOW_MAP];	//�V���h�E�}�b�v�B
	std::vector<Model*> m_shadowCasters;		//�V���h�E�L���X�^�[
	Matrix m_LVPMatrix;							//���C�g�r���[�v���W�F�N�V�����s��B
	SShadowCb m_shadowCBEntity;					//�V���h�E�̒l�B
	ConstantBuffer m_shadowCB;					//�V���h�E�̒萔�o�b�t�@�B
	Vector3 m_lightDirection;					//���C�g�̕����B
	float m_shadowAreas[NUM_SHADOW_MAP];		//�e�̗�����͈́B
	float m_lightHeight;						//���C�g�̍����B
	bool m_isEnable = false;					//�V���h�E�}�b�v���L���H
};

