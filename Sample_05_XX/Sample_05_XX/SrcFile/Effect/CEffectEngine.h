/// <summary>
/// �G�t�F�N�g�̊Ǘ���
/// �G�t�F�N�T�[���̊֐������b�v
/// </summary>

#pragma once
#include "srcFile/util/Noncopyable.h"
#include "srcFile/ResourceManager.h";

class CEffectEngine : Noncopyable
{
	using CEffectResourceManager = TResourceManager<Effekseer::EffectRef>;
public:
	CEffectEngine();
	~CEffectEngine();
	/// <summary>
	/// ���
	/// </summary>
	void Release();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Render();
	/// <summary>
	/// �G�t�F�N�g���쐬�Bwraper
	/// </summary>
	/// <param name="filepath">�G�t�F�N�g�̃t�@�C���p�X</param>
	/// <returns></returns>
	Effekseer::EffectRef CreateEffect(const wchar_t* filepath);
	/// <summary>
	/// �G�t�F�N�g�̍Đ��Bwraper
	/// <para>Effect��Update�ō��W���̕ϊ��͂��邱�ƁB</para>
	/// </summary>
	/// <param name="effet">�Đ�����G�t�F�N�g</param>
	/// <returns>�G�t�F�N�g�̃n���h��</returns>
	Effekseer::Handle Play(Effekseer::EffectRef& effet);
	/// <summary>
	/// �G�t�F�N�g�̍Đ����~�B
	/// </summary>
	/// <param name="handle">�G�t�F�N�g�̃n���h��</param>
	void Stop(Effekseer::Handle handle);
	/// <summary>
	/// �G�t�F�N�g�}�l�[�W���擾�B
	/// </summary>
	/// <returns></returns>
	Effekseer::ManagerRef& GetEffekseerManager()
	{
		return m_manager;
	}
	/// <summary>
	/// �G�t�F�N�T�[�̃����_���[�擾�B
	/// </summary>
	/// <returns></returns>
	EffekseerRenderer::RendererRef& GetEffekseerRenderer()
	{
		return m_effekseerRenderer;
	}
	/// <summary>
	/// �G�t�F�N�g���\�[�X�}�l�[�W���[�̎擾�B
	/// </summary>
	/// <returns></returns>
	CEffectResourceManager& GetEffectResourceManager()
	{
		return m_effectResourceManager;
	}
private:
	EffekseerRenderer::RendererRef m_effekseerRenderer = nullptr;				//�G�t�F�N�T�[�����_�[
	EffekseerRenderer::SingleFrameMemoryPool* m_sfMemoryPoolEfk = nullptr;	//�G�t�F�N�V�A�[�������v�[���B
	EffekseerRenderer::CommandList* m_commandListEfk = nullptr;				//�G�t�F�N�V�A�[�R�}���h���X�g�B
	Effekseer::ManagerRef m_manager = nullptr;								//�G�t�F�N�g�}�l�W���[
	CEffectResourceManager m_effectResourceManager;							//�G�t�F�N�g���\�[�X�}�l�W���[
};

