#pragma once

#include "Bloom.h"
#include "ToneMap.h"

/// <summary>
/// �����_�����O�G���W���B
/// </summary>
/// <remarks>
/// �����_�����O�֌W�̏������܂Ƃ߂��N���X�B
/// �쐬���B
/// ���݂�RT��ێ��A�������̂݁Btodo�Ƃ��ĕ`�揇�Ԃ����̃N���X�ŊǗ�����B
/// </remarks>
class RenderingEngine : public Noncopyable
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// ���C��RT���t���[���o�b�t�@�ɃR�s�[����B
	/// </summary>
	/// <param name="rc"></param>
	void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
public:
	void ZPrePass(RenderContext& rc);
	void ToneMapRender();
	void BloomRender();
public:
	/// <summary>
	/// ���C�������_�[�^�[�Q�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetMainRenderTarget()
	{
		return m_mainRenderTarget;
	}
	/// <summary>
	/// ZPrePass�p�̃����_�[�^�[�Q�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetZPrePassRenderTarget()
	{
		return m_zPrePassRT;
	}
	/// <summary>
	/// �u���[�����擾�B
	/// </summary>
	/// <returns></returns>
	Bloom& GetBloom()
	{
		return m_bloom;
	}
	/// <summary>
	/// �g�[���}�b�v���擾�B
	/// </summary>
	/// <returns></returns>
	ToneMap& GetToneMap()
	{
		return m_toneMap;
	}
public:
	/// <summary>
	/// �����_�[��o�^�B
	/// </summary>
	/// <param name="render"></param>
	void RegistRender(SkinModelRender* render)
	{
		m_renders.push_back(render);
	}
	/// <summary>
	/// ���X�g���珜���B
	/// </summary>
	/// <param name="model"></param>
	void RemoveRender(SkinModelRender* model)
	{
		auto it = std::find(m_renders.begin(), m_renders.end(), model);
		if (it != m_renders.end()) {
			//���������B
			m_renders.erase(it);
		}
	}
	/// <summary>
	/// �����_�[�̃��X�g���擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<SkinModelRender*>& GetRenderList() const
	{
		return m_renders;
	}
private:
	RenderTarget m_zPrePassRT;						//zPrePass�pRT�B
	RenderTarget m_mainRenderTarget;				//���C���pRT�B
	Sprite m_copyMainRtToFrameBufferSprite;			//���C��RT����t���[���o�b�t�@�ɃR�s�[����X�v���C�g�B

	//postEffects//
	ToneMap		m_toneMap;							//�g�[���}�b�v�B
	Bloom		m_bloom;							//�u���[���B
	
	std::vector<SkinModelRender*> m_renders;		//�����_�[���X�g�B
};

