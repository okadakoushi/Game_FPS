#pragma once

//GBuffer��`�B
enum EnGBuffer {
	GBuffer_albed,		//�A���x�h�B
	GBuffer_normal,		//�@���B
	GBuffer_spec,		//�X�y�L�����B
	GBuffer_worldPos,	//���[���h���W�B
	GBuffer_Shadow,		//�V���h�E�B
	Gbuffer_Num			//GBuffer�̐��B	
};

#include "Sprite.h"

class SkyBox;

/// <summary>
/// GBufferRender�B
/// GBuffer�������ݗp�N���X�B
/// </summary>
/// <code>
/// ModelRender Reff.
/// </code>
class DefferdRender : Noncopyable
{
public:
	DefferdRender() {};
	~DefferdRender();

	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �X�v���C�g�̏������B
	/// </summary>
	void SpriteInit();
	/// <summary>
	/// �`��B
	/// </summary>
	void Render(RenderContext& rc, const Matrix& view, const Matrix& proj, bool Clear = true);
	/// <summary>
	/// �`��B
	/// </summary>
	void DeffardRender(RenderContext& rc, const Matrix& view, const Matrix& proj);
	/// <summary>
	/// ���f����o�^�B
	/// <para>Dont call from User.</para>
	/// </summary>
	/// <param name="model">���f���B</param>
	void AddModel(SkinModelRender* model)
	{
		m_models.push_back(model);
	}
	/// <summary>
	/// ���f�������X�g����폜�B
	/// <para>Dont call from User.</para>
	/// </summary>
	/// <param name="model">���f���B</param>
	void RemoveModel(SkinModelRender* model)
	{
		auto it = std::find(m_models.begin(), m_models.end(), model);
		if (it != m_models.end()) {
			//���������B
			m_models.erase(it);
		}
	}
	/// <summary>
	///	GBuffer�̃e�N�X�`�����擾����B
	/// </summary>
	/// <param name="gbufferNum">Gbuffer�̔ԍ��B</param>
	/// <returns>�w�肵��Gbuffer�B</returns>
	Texture& GetTexture(const EnGBuffer& gbufferNum)
	{
		return m_GBuffers[gbufferNum].GetRenderTargetTexture();
	}
	/// <summary>
	/// Gbuffer�̃f�v�X��ԋp�B
	/// </summary>
	/// <returns></returns>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV() const 
	{
		return m_GBuffers[GBuffer_albed].GetDSVCpuDescriptorHandle();
	}
	/// <summary>
	/// �f�B�t�@�[�h�X�v���C�g���擾�B
	/// </summary>
	/// <returns></returns>
	Sprite& GetDefferdSprite()
	{
		return m_defferdSprite;
	}
	/// <summary>
	/// �_���[�W�G���A��ݒ�B
	/// </summary>
	/// <param name="area"></param>
	void SetDamageArea(float& area)
	{
		m_postEffectEntity.DamageArea = area;
	}
private:
	/// <summary>
	/// �|�X�g�G�t�F�N�g�I�p�����[�^�[�B
	/// </summary>
	struct PostEffectEntity {
		float DamageArea = 0.0f;
	};
private:
	std::vector<SkinModelRender*> m_models;			//���f���̃��X�g�B
	RenderTarget m_GBuffers[Gbuffer_Num];			//GBuffer�B
	Sprite m_defferdSprite;
	SkyBox* m_skyBox = nullptr;
	bool m_isInited = false;						//�������t���O�B
	PostEffectEntity m_postEffectEntity;			//�|�X�g�G�t�F�N�g�G���e�B�e�B�B

};

