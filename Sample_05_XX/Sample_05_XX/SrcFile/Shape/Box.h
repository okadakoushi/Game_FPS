#pragma once

/// <summary>
/// ���BAABB�B
/// </summary>
class Box
{
public:
	Box();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="halfSize"></param>
	void Init(const Vector3& halfSize);
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="worldMat"></param>
	void Update(const Matrix& worldMat);
	/// <summary>
	/// �o�E���e�B���O�{�b�N�X��`��B
	/// </summary>
	void DrawBox();
	/// <summary>
	/// �w�肳�ꂽ�{�b�N�X�̒��_���W���擾�B
	/// </summary>
	/// <param name="vertNo">���_�ԍ��B</param>
	/// <returns>�{�b�N�X�̒��_���W�B</returns>
	const Vector3& GetVertexPosition(unsigned int vertNo)const
	{
		if (vertNo > 8) {
			MessageBoxA(nullptr, "���̒��_�͑��݂��܂���B���_�ԍ����m�F���Ă��������B", "Box::GetVertexPosition", MB_OKCANCEL);
		}
		return m_vertexPosition[vertNo];
	}

private:
	Vector3 m_halfSize = g_vec3Zero;	//�n�[�t�T�C�Y�B
	Vector3 m_vertexPosition[8];		//�{�b�N�X���\������8���_�B
};

