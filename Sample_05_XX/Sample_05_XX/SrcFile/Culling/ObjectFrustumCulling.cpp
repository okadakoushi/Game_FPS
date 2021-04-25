#include "stdafx.h"
#include "ObjectFrustumCulling.h"
#include "SrcFile/Shape/Box.h"

ObjectFrustumCulling::ObjectFrustumCulling()
{
}

ObjectFrustumCulling::~ObjectFrustumCulling()
{
}

void ObjectFrustumCulling::Excute(const Box& box, const Matrix& viewProj)
{
	if (m_isAvtive) {
		//�t���O�������B
		SetCullingFlag(false);
		//aabb���X�N���[����Ԃ̐��K�����W�n�ɕϊ��B
		//xy��1.0�`-1.0�Az��0.0�`1.0�͈̔͂Ȃ��ʓ��ɂ���B
		//aabb��8���_���ׂĂ��A��ʊO�Ȃ�J�����O����B
		Vector3 vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		Vector3 vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		for (int i = 0; i < 8; i++) {
			Vector4 vertPos(box.GetVertexPosition(i));
			viewProj.Apply(vertPos);
			//���K�����W�n�B
			vertPos.x /= vertPos.w;
			vertPos.y /= vertPos.w;
			vertPos.z /= vertPos.w;
			vMax.Max({ vertPos.x, vertPos.y, vertPos.z });
			vMin.Min({ vertPos.x, vertPos.y, vertPos.z });
		}
		//������B
		if (vMax.x < -1.0f
			|| vMin.x > 1.0f
			|| vMax.y < -1.0f
			|| vMin.y > 1.0f) 
		{
			//�J�����O�ł��那�B
			SetCullingFlag(true);
		}
	}
}
