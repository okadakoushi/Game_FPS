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
		//フラグ初期化。
		SetCullingFlag(false);
		//aabbをスクリーン空間の正規化座標系に変換。
		//xyが1.0～-1.0、zが0.0～1.0の範囲なら画面内にある。
		//aabbの8頂点すべてが、画面外ならカリングする。
		Vector3 vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		Vector3 vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		for (int i = 0; i < 8; i++) {
			Vector4 vertPos(box.GetVertexPosition(i));
			viewProj.Apply(vertPos);
			//正規化座標系。
			vertPos.x /= vertPos.w;
			vertPos.y /= vertPos.w;
			vertPos.z /= vertPos.w;
			vMax.Max({ vertPos.x, vertPos.y, vertPos.z });
			vMin.Min({ vertPos.x, vertPos.y, vertPos.z });
		}
		//可視判定。
		if (vMax.x < -1.0f
			|| vMin.x > 1.0f
			|| vMax.y < -1.0f
			|| vMin.y > 1.0f) 
		{
			//カリングできるぅ。
			SetCullingFlag(true);
		}
	}
}
