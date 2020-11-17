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

/// <summary>
/// GBufferRender�B
/// GBuffer�������ݗp�N���X�B
/// </summary>
/// <code>
/// ModelRender Reff.
/// </code>
class GBufferRender : Noncopyable
{
public:
	GBufferRender() {};
	~GBufferRender();

	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �`��B
	/// </summary>
	void Render(RenderContext& rc);
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

private:
	std::vector<SkinModelRender*> m_models;			//���f���̃��X�g�B
	RenderTarget m_GBuffers[Gbuffer_Num];			//GBuffer�B
};

