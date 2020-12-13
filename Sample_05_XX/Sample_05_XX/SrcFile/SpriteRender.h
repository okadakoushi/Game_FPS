#pragma once

class Sprite;

/// <summary>
/// �X�v���C�g�����_�[�B
/// </summary>
class SpriteRender : public IGameObject
{
public:
	~SpriteRender();
	/// <summary>
	/// �������B
	/// </summary>
	void Init(const SpriteInitData& spriteData);
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
private:
	Sprite m_sprite;			//�X�v���C�g�B
	bool m_isInited = false;	//�ꉞ�������t���O�B
	Vector3 m_pos = g_vec3Zero;				//�ʒu�B
	Quaternion m_rot = g_quatIdentity;		//��]�B
	Vector3 m_scale = g_vec3One;			//�g��B
	Vector2 m_pivot = Sprite::DEFAULT_PIVOT;//�s�{�b�g�B
};

