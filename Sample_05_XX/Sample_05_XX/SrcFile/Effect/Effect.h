/// <summary>
/// �G�t�F�N�g
/// �`��̓G�t�F�N�g�G���W�����܂Ƃ܂čs���B	
/// </summary>

#pragma once
#include "srcFile/GameObject/IGameObject.h"

namespace myEngine {
	class Effect : public IGameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Effect();
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Effect();
		/// <summary>
		/// �����I�ȉ��
		/// </summary>
		void Release();
		/// <summary>
		/// �G�t�F�N�g�̍Đ�
		/// <para>float x y z �������œn�����ق������������H</para>
		/// </summary>
		/// <param name="filePath">�Đ�����G�t�F�N�g�̃p�X</param>
		void Play(const wchar_t* filePath);
		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rot">��]</param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		/// <summary>
		/// �g�嗦�̐ݒ�
		/// </summary>
		/// <param name="scale">�g�嗦</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// �G�t�F�N�g���Đ������̔���
		/// </summary>
		/// <returns>�Đ���/��~��</returns>
		bool IsPlay() const
		{
			return EffectEngineObj().GetEffekseerManager()->GetShown(m_handle);
		}
		/// <summary>
		/// �X�V
		/// </summary>
		void Update(); //override;
	private:
		Effekseer::EffectRef			m_effect = nullptr;			//�G�t�F�N�g
		Effekseer::Handle				m_handle = -1;				//�n���h��
		Vector3 m_position =			g_vec3Zero;			//���W
		Quaternion m_rotation =			g_quatIdentity;	//��]
		Vector3 m_scale =				g_vec3One;			//�g�嗦
	};
}

