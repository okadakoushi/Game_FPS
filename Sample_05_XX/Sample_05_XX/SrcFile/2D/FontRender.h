/// <summary>
/// フォントレンダー。
/// </summary>
/// <remarks>
/// GameObjectManager下で管理する、フォントのラッパークラス。
/// Drawのタイミングが、エンジンのデフォルト呼び出しでいいならこいつを使う。
/// 違う場合は、標準のフォントクラスを使う事。
/// </remarks>

#pragma once

#include "Font.h"

namespace myEngine {
	class CFontRender : public IGameObject
	{
	public:
		/// <summary>
		/// テキストを設定。
		/// <para>おもいでぇ。</para>
		/// <para>基本Unsafe版使えると思うから、使ってな。</para>
		/// </summary>
		/// <param name="text">テキスト。</param>
		void SetText(const wchar_t* text)
		{
			if (wcscmp(m_text.c_str(), text) != 0) {
				//文字列に変更があった。
				//ここでコピーが発生する？
				m_text = text;
			}
		}
		/// <summary>
		/// アンセーフ版、SetText。
		/// </summary>
		/// <remarks>
		/// メンバ変数で引数に渡すの定義したら、まずバグらないはず。
		/// ローカル変数で引数に渡すの定義したら、バグるよ。
		/// 意味わからない場合は、通常版使ってください。
		/// メンバ変数に書いておかしくなったら、教えてください。
		/// UpdateでSetTextしない場合は、変数の寿命の都合上Unsafe使わない方がいいかも。
		/// </remarks>
		/// <param name="text"></param>
		void SetTextUnsafe(const wchar_t* text)
		{
			//コピーが発生しない。
			m_textUnsafe = text;
		}
		/// <summary>
		/// フォントを表示する座標設定。
		/// <para>画面中央が0,0</para>
		/// </summary>
		/// <param name="pos">座標</param>
		void SetPosition(const Vector2& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// フォントの色を設定。
		/// </summary>
		/// <param name="color">カラー</param>
		void SetColor(const Vector3& color)
		{
			m_color = color;
		}
		/// <summary>
		/// フォントの色を設定。
		/// <para>α設定できる方。</para>
		/// </summary>
		/// <param name="color">カラー</param>
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rot">回転。</param>
		void SetRotation(float rot)
		{
			m_rotation = rot;
		}
		/// <summary>
		/// 拡大率。
		/// </summary>
		/// <param name="scale">スケール。</param>
		void SetScale(float scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// ピボットの設定。
		/// </summary>
		/// <param name="pivot">ピボット。</param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// シャドウのパラメーターを設定。
		/// </summary>
		/// <param name="isDrawShadow">シャドウを描画するか</param>
		/// <param name="shadowOffset">シャドウオフセット。</param>
		/// <param name="shadowColor">シャドウカラー。</param>
		void SetShadowParam(const bool& isDrawShadow, const float& shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}
	private:
		void RenderHUD() override;
	private:
		CFont			m_font;								//フォント。
		std::wstring	m_text;								//テキスト。
		const wchar_t*	m_textUnsafe;						//アンセーフテキスト。
		Vector2			m_position = Vector2::Zero;			//座標。(0.0f,0.0f)で真ん中。
		Vector4			m_color = Vector4::White;			//カラー。
		float			m_rotation = 0.0f;					//回転。
		float			m_scale = 1.0f;						//スケール。
		Vector2			m_pivot = {0.5f, 0.5f};				//ピボット。
	};
}
