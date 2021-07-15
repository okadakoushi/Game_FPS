#pragma once

class Sprite;

/// <summary>
/// �X�v���C�g�����_�[�B
/// todo:isDraw3D���邪�@�\���ĂȂ����璼���B
/// </summary>
class SpriteRender : public IGameObject
{
public:
	~SpriteRender();
	/// <summary>
	/// �������B
	/// <para>isDraw�@�\�����ĂȂ������B</para>
	/// </summary>
	/// <param name="spriteData">�������f�[�^�[�B</param>
	/// <param name="isDraw3D">3D�`�悷��H</param>
	void Init(const SpriteInitData& spriteData, bool isDraw3D = false);
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �ʏ�`��B
	/// </summary>
	void ForwardRender() override;
	/// <summary>
	/// HUD�`��B
	/// </summary>
	void RenderHUD() override;
	/// <summary>
	/// �ʒu�����B
	/// </summary>
	/// <param name="v">�ʒu�B</param>
	void SetPos(const Vector3& v)
	{
		m_pos = v;
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
	/// �X�P�[�������B
	/// </summary>
	/// <param name="scale">�X�P�[���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �s�{�b�g�����B
	/// </summary>
	/// <param name="pivot">�s�{�b�g�B</param>
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}
	/// <summary>
	/// ��Z�J���[��ݒ�B
	/// </summary>
	/// <param name="color"></param>
	void SetMulColor(const Vector4& color)
	{
		m_sprite.SetMulColor(color);
	}
	/// <summary>
	/// �r���[��ݒ�B
	/// </summary>
	/// <param name="view"></param>
	void SetView(const Matrix& view) 
	{
		m_view = view;
	}
	/// <summary>
	/// �v���W�F�B
	/// </summary>
	/// <param name="proj"></param>
	void SetProj(const Matrix& proj)
	{
		m_proj = proj;
	}
	/// <summary>
	/// �ʒu�擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos() const
	{
		return m_pos;
	}
	/// <summary>
	/// �e�N�X�`�����擾�B
	/// </summary>
	/// <param name="index">�Y�����B</param>
	/// <returns></returns>
	Texture* GetTexture(int index) 
	{
		return m_sprite.GetTexture(index);
	}
private:
	Sprite m_sprite;			//�X�v���C�g�B
	bool m_isInited = false;	//�ꉞ�������t���O�B
	Vector3 m_pos = g_vec3Zero;				//�ʒu�B
	Quaternion m_rot = g_quatIdentity;		//��]�B
	Vector3 m_scale = g_vec3One;			//�g��B
	Vector2 m_pivot = Sprite::DEFAULT_PIVOT;//�s�{�b�g�B
	bool m_isDraw3D = false;					//3D�`��H
	Matrix m_view;							//�r���[�B
	Matrix m_proj;							//�v���W�F�B
};

