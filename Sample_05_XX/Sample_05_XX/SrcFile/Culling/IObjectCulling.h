#pragma once

#include "SrcFile/Shape/Box.h"

/// <summary>
/// �J�����O�����̃C���^�[�t�F�[�X�N���X�B
/// </summary>
class IObjectCulling
{
public:
	IObjectCulling() {};
	virtual ~IObjectCulling() {};
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="aabb"></param>
	virtual void Excute(const Box& aabb, const Matrix& viewProj) = 0;
	/// <summary>
	/// �J�����O����Ă��邩�̔���B
	/// </summary>
	/// <returns>�J�����O�t���O�B</returns>
	bool IsCulling() const
	{
		return m_isCulling;
	}

protected:
	/// <summary>
	/// �J�����O�t���O��ݒ�B
	/// </summary>
	/// <param name="flag">�t���O�B</param>
	void SetCullingFlag(bool flag)
	{
		m_isCulling = flag;
	}
private:
	bool m_isCulling = false;	//�J�����O�t���O�B
};

