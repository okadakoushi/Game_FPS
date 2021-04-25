#pragma once

#include "IObjectCulling.h"

/// <summary>
/// ������J�����O�B
/// </summary>
class ObjectFrustumCulling : public IObjectCulling
{
public:
	ObjectFrustumCulling();
	~ObjectFrustumCulling();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="ViewProj">�r���[�v���W�F�B</param>
	void Init()
	{
		m_isAvtive = true;
	}
	/// <summary>
	/// �J�����O���s�B
	/// </summary>
	/// <param name="box">AABB�B</param>
	/// <param name="viewProj">viewProj�B</param>
	void Excute(const Box& box, const Matrix& viewProj) override;
private:
	bool m_isAvtive = false;		//�L�����ǂ����B
};

