/// <summary>
/// �t�H���g
/// </summary>
/// <remarks>
/// �t�H���g�̕`�揈���B
/// ��{�̓t�H���g�����_�[���g���ĕ`�悷�邱�ƁB
/// ������Draw����ꍇ�́ABegin��Draw��End�̏��ԂŌĂяo���Ă��������B
/// </remarks>

#pragma once

class FontEngine;

namespace myEngine {
	class CFont
	{
	public:
		CFont();
		~CFont();
		/// <summary>
		/// �`��J�n�����B
		/// <para>HUD�`��Ńt�H���g��`�悷��ۂ̓��[�U�[�͂��̊֐����Ăяo���K�v�͂���܂���B</para>
		/// <para>HUD�`��ȊO�ŌĂяo���ۂ̓��[�U�[�͂��̊֐����g�p���Ă��������B</para>
		/// </summary>
		void Begin();
		/// <summary>
		/// �`��I�������B
		/// <para>HUD�`��Ńt�H���g��`�悷��ۂ̓��[�U�[�͂��̊֐����Ăяo���K�v�͂���܂���B</para>
		/// <para>HUD�`��ȊO�ŌĂяo���ۂ̓��[�U�[�͂��̊֐����g�p���Ă��������B</para>
		/// </summary>
		void End();
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="text">�\������e�L�X�g</param>
		/// <param name="pos">�ʒu</param>
		/// <param name="color">�F</param>
		/// <param name="rotation">��]</param>
		/// <param name="scale">�傫��</param>
		/// <param name="pivot">��_</param>
		void Draw(
			wchar_t const* text,
			const Vector2& pos,
			const Vector4& color = Vector4::White,
			float rotation = 0.0f,
			float scale = 1.0f,
			Vector2 pivot = {0.5f, 0.5f}
		);
	private:
		FontEngine* m_fe;
	};
}

