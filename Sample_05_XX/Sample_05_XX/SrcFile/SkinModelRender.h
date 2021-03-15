#pragma once


/// <summary>
/// �X�L�����f�������_�[�B
/// </summary>
/// <code>
/// 1.NewGO();
/// 2.(if ForwardRender)SetForwardRender;
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
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	bool Start() override;
	/// <summary>
	/// �������B
	/// <para>todo:T�����p�g��Ȃ��C���B</para>
	/// </summary>
	/// <param name="tkmFile">tkm�t�@�C���p�X�B</param>
	/// <param name="tkaFilePaths">tka�t�@�C���p�X�B</param>
	template<typename TKA_FILE_ARRAY, std::size_t NUM_TKA_FILE>
	void Init(const char* tkmFile, TKA_FILE_ARRAY(&tkaFilePaths)[NUM_TKA_FILE])
	{
		if (!m_isForwardRender) {
			//���񏉊�������̂Ŗʓ|�Ȃ̂�Defferd�̏ꍇ�͂����ŏ����������Ⴄ�B
			SetDefferedRender();
		}
		//���f���������B
		m_model.InitModel(tkmFile);
		//todo : ����pushBuck
		for (auto i = 0; i < NUM_TKA_FILE; i++) {
			m_tkaFilePaths.push_back(tkaFilePaths[i]);
		}
		//���f���̓ǂݍ��ݏI���B
		m_initStep = enInitStep_LoadSkelton;
		m_filePath = tkmFile;
	}
	void Init(const char* tkmFile) {
		if (!m_isForwardRender) {
			//���񏉊�������̂Ŗʓ|�Ȃ̂�Defferd�̏ꍇ�͂����ŏ����������Ⴄ�B
			SetDefferedRender();
		}
		//���f���������B
		m_model.InitModel(tkmFile);
		//���f���̓ǂݍ��ݏI���B
		m_initStep = enInitStep_LoadSkelton;
		m_filePath = tkmFile;
	}
	/// <summary>
	/// �f�X�g���C�B
	/// </summary>
	void OnDestroy();
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
		m_animation.Play(clipNo, interpolateTime);
	}
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// ���W�擾�B
	/// </summary>
	/// <returns></returns>
	Vector3& GetPosition()
	{
		return m_pos;
	}
	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="qRot"></param>
	void SetRotation(const Quaternion& qRot)
	{
		m_rot = qRot;
	}
	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <returns></returns>
	Quaternion& GetRotation() 
	{
		return m_rot;
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
	/// �g����擾�B
	/// </summary>
	/// <returns></returns>
	Vector3& GetScale()
	{
		return m_scale;
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
		m_model.SetShadowReciever(flag);
	}
	/// <summary>
	/// �V���h�E�L���X�^�[�t���O��ݒ�B
	/// </summary>
	/// <param name="flag">�t���O�B</param>
	void SetShadwoCaster(bool flag)
	{
		m_model.SetShadwoCaster(flag);
	}
	/// <summary>
	/// �t�H���[�h�����_�[�`��ɂ���B
	/// <para>todo:ZBuffer����Ƀo�O����B</para>
	/// </summary>
	void SetForwardRender()
	{
		m_isForwardRender = true;
		//�t�H���[�h�����_�[�ݒ肷��̂ŃV�F�[�_�[���ꏏ�ɕύX����B
		m_model.SetShader(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx");
	}
	/// <summary>
	/// �f�B�t�@�[�h�����_�[�`��ɂ���B
	/// <para>�ʏ�̕`��ɖ߂��ۂɎg�p�B</para>
	/// </summary>
	void SetDefferedRender()
	{
		m_isForwardRender = false;
		//�f�B�t�@�[�h�����_�[�ݒ肷��̂ŃV�F�[�_�[���ꏏ�ɕύX����B
		m_model.SetShader(L"Assets/shader/DefeardModel.fx");
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
	Vector3 m_pos = g_vec3Zero;			//���W�B
	Quaternion m_rot = g_quatIdentity;	//��]�B
	Vector3 m_scale = g_vec3One;		//�g��B
	EnRenderMode m_renderMode = enRenderMode_NonSkin;	//�`�惂�[�h�B
	EnModelInitStep m_initStep = enInitStep_None;		//�������X�e�b�v�B
	bool m_isForwardRender = false;		//�t�H���[�h�����_�����O����H
};

