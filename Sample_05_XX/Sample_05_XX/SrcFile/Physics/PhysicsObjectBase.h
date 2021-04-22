#pragma once
class PhysicsObjectBase
{
public:
	/// <summary>
	/// ����B
	/// </summary>
	virtual void Release() = 0;
private:
	/// <summary>
	/// �ÓI�����I�u�W�F�N�g�쐬�����̋��ʏ����B
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	virtual void CreateCommon(Vector3 pos, Quaternion rot) = 0;
public:
	PhysicsObjectBase()
	{
	}
	virtual ~PhysicsObjectBase()
	{
	}
	/// <summary>
	/// �{�b�N�X�`��̃S�[�X�g�I�u�W�F�N�g���쐬�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	/// <param name="size">�T�C�Y�B</param>
	void CreateBox(Vector3 pos, Quaternion rot, Vector3 size);
	/// <summary>
	/// ���b�V������S�[�X�g�I�u�W�F�N�g���쐬�B
	/// </summary>
	/// <param name="pos">�ʒu�B</param>
	/// <param name="rot">��]�B</param>
	/// <param name="tkmfile">�e�p�����[�^�[�Btkm/model/modelRender</param>
	void CreateMesh(Vector3 pos, Quaternion rot, const TkmFile& tkmfile);
	void CreateMesh(Vector3 pos, Quaternion rot, Vector3 scale, const Model& model);
	void CreateMesh(Vector3 pos, Quaternion rot, Vector3 scale, SkinModelRender* modelRender);
protected:
	std::unique_ptr<ICollider> m_collider;		//�R���C�_�[�B
};

