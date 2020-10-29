#pragma once

/// <summary>
/// �X�L�����f�������_�[�B
/// </summary>
/// <code>
/// NewGO();
/// Init();
/// (if Needed)Set~~();
/// Update();
/// Draw();
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
	void Init(const char* filepath);
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`��B
	/// </summary>
	void Draw() override;
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
	/// <param name="rm"></param>
	void SetRenderMode(const RenderMode& rm)
	{
		m_renderMode = rm;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�Ƃ��ēo�^�B
	/// </summary>
	void SetShadowReciever()
	{
		m_model.SetShadowReciever();
	}
private:
	Model m_model;						//���f���B
	Vector3 m_pos = g_vec3Zero;			//���W�B
	Quaternion m_rot = g_quatIdentity;	//��]�B
	Vector3 m_scale = g_vec3One;		//�g��B
	RenderMode m_renderMode = enRenderMode_NonSkin;	//�`�惂�[�h�B
};

