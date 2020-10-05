#pragma once

/// <summary>
/// �Q�[���I�u�W�F�N�g�̊��N���X
/// �I�u�W�F�N�g�}�l�[�W���[�̊֐����g���ꍇ�͌p�����ĂˁI�I
/// </summary>
/// <remarks>
/// /*---�eRender�Ŏg�p���]�܂�����---*/
/// PreRender���V���h�E�}�b�v�A�V���G�b�g
/// PostRender��2D�A�G�t�F�N�g�A�t�H���g
/// Draw����L�ȊO
/// /*----------------------------------*/
/// </remarks>


class GameObjectManager;
//���O�ύX�����Ă邾��
typedef unsigned char	GameObjPrio;
class IGameObject : Noncopyable
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	IGameObject() :
		m_priority(0),
		m_isStart(false),
		m_isDead(false),
		m_isActive(true),
		m_isUpdate(true)
	{
	};
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	/// <remarks>
	/// virtual�͒�`���Ă邾���ł��A�����͔h���N���X�ɏ����Ă�������
	/// </remarks>
	virtual ~IGameObject()
	{
	};
	/// <summary>
	/// �v�������_�[
	/// </summary>
	/// <remarks>
	/// PreRender�ŕ`�悪�]�܂����́B
	/// 1.�V���h�E�}�b�v�`��@��RenderToShadowMap
	/// 2.�V���G�b�g�`�� ��
	/// </remarks>
	virtual void PreRender()
	{
	}
	/// <summary>
	/// �|�X�g�����_�[
	/// </summary>
	/// <remarks>
	/// PostRender�ŕ`�悪�]�܂����́B
	/// 1.�G�t�F�N�g 
	/// </remarks>
	virtual void PostRender()
	{
	}
	/// <summary>
	/// <para>Update�̒��O�ɌĂ΂��J�n����</para>
	/// <para>true���Ԃ����ƍX�V����(Update)���J�n������</para>
	/// </summary>
	/// <returns>
	/// �I�u�W�F�N�g�̏�����1�t���[���łł���Ȃ�true�A�ł��Ă��Ȃ��Ȃ�false
	/// </returns>
	virtual bool Start() { return true; }
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() {};
	/// <summary>
	/// �D��x�擾
	/// </summary>
	/// <returns>�D��x</returns>
	GameObjPrio GetPriority() const
	{
		return m_priority;
	}


	virtual bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief �A�N�e�B�u���ǂ����𔻒�B
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	void SetActive(bool act) {
		m_isActive = act;
	}

	void SetUpdate(bool Up) {
		m_isUpdate = Up;
	}
	bool IsUpdate() {
		return m_isUpdate;
	}
public:
	/// <summary>
	/// �֐������b�v���Ă邾������
	/// �e�I�u�W�F�N�g�̊֐��̏������J�n�������
	/// </summary>
	void PreRenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead)
		{
			PreRender();
		}
	}
	void StartWrapper()
	{
		if (m_isActive && !m_isStart)
		{
			if (Start()) {
				//�������I��
				m_isStart = true;
			}
		}
	}
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && m_isUpdate)
		{
			Update();
		}
	}
	void DrawWrapper()
	{
		if (m_isActive && m_isStart)
		{
			Draw();
		}
	}
	void PostRenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead)
		{
			PostRender();
		}
	}
	friend class GameObjectManager;
protected:
	GameObjPrio		m_priority;		//�D��x
	bool			m_isStart;		//Start�t���O
	bool			m_isDead;		//���S�t���O
	bool			m_isUpdate;		//�X�V�t���O
	bool			m_isActive;		//Active�t���O
	unsigned int	m_tags = 0;		//�^�O
	unsigned int	m_nameKey = 0;	//���O�L�[
};


