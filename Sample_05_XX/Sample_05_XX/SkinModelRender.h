#pragma once

enum EnRenderMode {
	enRenderMode_Skin,				//�X�L������B
	enRenderMode_NonSkin,			//�X�L���Ȃ��B
	enRenderMode_DrawShadow,		//�V���h�E�}�b�v�`��B
	enRenderMode_NonSkinDrawShadow,	//�X�L���Ȃ��V���h�E�}�b�v�`��B
	enRenderMode_Num				//����񂫂����邪�ꉞ�B
};


/// <summary>
/// �X�L�����f�������_�[�B
/// </summary>
/// <code>
/// 1.NewGO();
/// 2.(if ForwardRender)SetShader() & SetForwardRender todo:wrap;
/// 3.Init();
/// 4.(if Needed)Set~~();
/// 5.(if Shadow)SetShadow~~();
/// 6.(if Anim)PlayAnimation();
/// 7.(if ChangeRM)SetRenderMode();
/// </code>
class SkinModelRender : public IGameObject
{
public:
	SkinModelRender();
	~SkinModelRender();
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	bool Start() override;
	/// <summary>
	/// �������B
	/// </summary>
	void Init(const char* tkmFile, const char* tkaFile = nullptr);
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`��B
	/// </summary>
	void ForwardRender() override;
	/// <summary>
	/// �A�j���[�V�������Đ��B
	/// </summary>
	void PlayAnimation(int clipNo, float interpolateTime)
	{
		m_model.PlayAnim(clipNo, interpolateTime);
	}
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="qRot"></param>
	void SetRotation(Quaternion& qRot)
	{
		m_rot = qRot;
	}
	/// <summary>
	/// �g�嗦��ݒ�B
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3& scale)
	{	
		m_scale = scale;
	}
	/// <summary>
	/// �����_�[���[�h�̐ݒ�B
	/// <para>���ɐݒ肵�Ȃ��ꍇ�A�X�L���Ȃ��`��B</para>
	/// </summary>
	/// <param name="rm">RenderMode</param>
	void SetRenderMode(const EnRenderMode& rm)
	{
		m_renderMode = rm;
	}
	/// <summary>
	/// �����_�[���[�h���擾�B
	/// </summary>
	/// <returns></returns>
	EnRenderMode& GetRenderMode() 
	{
		return m_renderMode;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�Ƃ��ēo�^�B
	/// </summary>
	/// <param name="flag">�t���O�B</param>
	void SetShadowReciever(bool flag)
	{
		m_model.SetShadowReciever();
	}
	/// <summary>
	/// �V���h�E�L���X�^�[�t���O��ݒ�B
	/// </summary>
	/// <param name="flag">�t���O�B</param>
	void SetShadwoCaster(bool flag)
	{
		ShadowCaster = flag;
	}
	/// <summary>
	/// �t�H���[�h�����_�[�`��ɂ���H
	/// <para>����ȃ����_�����O���s���ꍇ��True�ɂ���B</para>
	/// <para>todo:Shader���ꏏ��deferred�p�ɕύX�B</para>
	/// </summary>
	/// <param name="flag">�t���O�B</param>
	void SetForwardRender(bool flag)
	{
		m_isForwardRender = flag;
	}
	/// <summary>
	/// �V�F�[�_�[��ݒ�B
	/// </summary>
	/// <param name="fxPath">�V�F�[�_�[�̃p�X�B</param>
	void SetShader(const wchar_t* fxPath)
	{
		m_model.SetShader(fxPath);
	}
	/// <summary>
	/// ���f�����擾�B
	/// </summary>
	/// <returns></returns>
	Model& GetModel() 
	{
		return m_model;
	}
private:
	/// <summary>
	/// ���f���̏������X�e�b�v�B
	/// </summary>
	enum EnModelInitStep {
		enInitStep_None,
		enInitStep_LoadModel,
		enInitStep_LoadSkelton,
		enInitStep_LoadAnimation,
		enInitStep_InitAnimationClip,
		enInitStep_Completed,
		initStep,
	};
	Model m_model;						//���f���B
	Skeleton m_skeleton;				//�X�P���g���B
	std::vector<std::string>		m_tkaFilePaths;		//tka�t�@�C���̃t�@�C���p�X���X�g�B
	std::vector<AnimationClipPtr>	m_animationClips;	//�A�j���[�V�����N���b�v�B
	Animation						m_animation;		//�A�j���[�V����
	std::string						m_filePath;			//tkm�t�@�C���p�X�B
	bool ShadowCaster = false;							//�V���h�E�L���X�^�[�B
	Vector3 m_pos = g_vec3Zero;			//���W�B
	Quaternion m_rot = g_quatIdentity;	//��]�B
	Vector3 m_scale = g_vec3One;		//�g��B
	EnRenderMode m_renderMode = enRenderMode_NonSkin;	//�`�惂�[�h�B
	EnModelInitStep m_initStep = enInitStep_None;		//�������X�e�b�v�B
	bool m_isForwardRender = false;		//�t�H���[�h�����_�����O����H
};

