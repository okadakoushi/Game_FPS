#include "stdafx.h"
#include "Box.h"

Box::Box()
{
	memset(m_vertexPosition, 0, sizeof(m_vertexPosition));
}

void Box::Init(const Vector3& halfSize)
{
	m_halfSize = halfSize;
}

void Box::Update(const Matrix& worldMat)
{
	//8頂点の座標を計算。
	m_vertexPosition[0] = Vector3::Zero;
	m_vertexPosition[0].x -= m_halfSize.x;
	m_vertexPosition[0].y -= m_halfSize.y;
	m_vertexPosition[0].z -= m_halfSize.z;

	m_vertexPosition[1] = Vector3::Zero;
	m_vertexPosition[1].x += m_halfSize.x;
	m_vertexPosition[1].y -= m_halfSize.y;
	m_vertexPosition[1].z -= m_halfSize.z;

	m_vertexPosition[2] = Vector3::Zero;
	m_vertexPosition[2].x -= m_halfSize.x;
	m_vertexPosition[2].y += m_halfSize.y;
	m_vertexPosition[2].z -= m_halfSize.z;

	m_vertexPosition[3] = Vector3::Zero;
	m_vertexPosition[3].x += m_halfSize.x;
	m_vertexPosition[3].y += m_halfSize.y;
	m_vertexPosition[3].z -= m_halfSize.z;

	m_vertexPosition[4] = Vector3::Zero;
	m_vertexPosition[4].x -= m_halfSize.x;
	m_vertexPosition[4].y -= m_halfSize.y;
	m_vertexPosition[4].z += m_halfSize.z;

	m_vertexPosition[5] = Vector3::Zero;
	m_vertexPosition[5].x += m_halfSize.x;
	m_vertexPosition[5].y -= m_halfSize.y;
	m_vertexPosition[5].z += m_halfSize.z;

	m_vertexPosition[6] = Vector3::Zero;
	m_vertexPosition[6].x -= m_halfSize.x;
	m_vertexPosition[6].y += m_halfSize.y;
	m_vertexPosition[6].z += m_halfSize.z;

	m_vertexPosition[7] = Vector3::Zero;
	m_vertexPosition[7].x += m_halfSize.x;
	m_vertexPosition[7].y += m_halfSize.y;
	m_vertexPosition[7].z += m_halfSize.z;

	for (auto& pos : m_vertexPosition) {
		worldMat.Apply(pos);
	}
}
