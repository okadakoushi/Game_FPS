#pragma once

#include "SrcFile/InputKeyCode.h"

class SpriteRender;

class Map : public IGameObject
{
public:
	~Map();
	bool Start();
	void Update();
	/// <summary>
	/// �~�j�}�b�v�p�̃����_�[�^�[�Q�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetRenderTarget()
	{
		return m_miniMap;
	}
	/// <summary>
	/// �~�j�}�b�v�̒��S�Ƃ�����W�B
	/// </summary>
	/// <param name="pos"></param>
	void SetTargetPos(const Vector3& pos)
	{
		m_targetPos = pos;
	}
private:
	/// <summary>
	/// �J�����X�V�B
	/// </summary>
	void UpdateCamera();
	/// <summary>
	/// �~�j�}�b�v��`��B
	/// </summary>
	void Draw();
	/// <summary>
	/// �~�j�}�b�v�̃T�C�Y��؂�ւ��B
	/// </summary>
	void SwichMiniMapSize();
private:
	const float m_CAMERA_HEIGHT = 300.0f;		//�J�����̍����B
	const float m_SHRINK_WITH_HEIGHT = 1300.0f;	//�k�����̃}�b�v�̉��c�B
	const int m_WIDTH = 300.0f;					//�X�v���C�g�̉��B
	const int m_HEIGHT = 300.0f;				//�X�v���C�g�̏c�B
	const Vector4 m_COLOR = Vector4::White;		//��Z�J���[�B
	RenderTarget m_miniMap;						//mini�}�b�v�pRTV�B
	Matrix m_viewMatrix;						//�r���[�B
	Matrix m_projMatirx;						//�v���W�F�B
	Vector3 m_targetPos = g_vec3Zero;			//�~�j�}�b�v�̒��S�Ƃ���^�[�Q�b�g�̍��W�B
	Vector3 m_cameraPos = g_vec3Zero;			//�~�j�}�b�v�J�����̈ʒu	
	bool m_isExpansion = false;					//�g�傳��Ă�?
	float m_currentWith_Height = m_SHRINK_WITH_HEIGHT;	//���݂̃}�b�v�̉��c�B
	//��{�p�����B
	Vector3 m_pos = g_vec3Zero;
	Vector3 m_scale = g_vec3One;
	Vector2 m_pivot = g_vec2Zero;
	SpriteRender* m_spriteRender = nullptr;
};

