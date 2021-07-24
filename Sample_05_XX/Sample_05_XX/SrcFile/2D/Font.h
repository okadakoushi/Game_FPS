/// <summary>
/// フォント
/// </summary>
/// <remarks>
/// フォントの描画処理。
/// 基本はフォントレンダーを使って描画すること。
/// こいつでDrawする場合は、Begin→Draw→Endの順番で呼び出してください。
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
		/// 描画開始処理。
		/// <para>HUD描画でフォントを描画する際はユーザーはこの関数を呼び出す必要はありません。</para>
		/// <para>HUD描画以外で呼び出す際はユーザーはこの関数を使用してください。</para>
		/// </summary>
		void Begin();
		/// <summary>
		/// 描画終了処理。
		/// <para>HUD描画でフォントを描画する際はユーザーはこの関数を呼び出す必要はありません。</para>
		/// <para>HUD描画以外で呼び出す際はユーザーはこの関数を使用してください。</para>
		/// </summary>
		void End();
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="text">表示するテキスト</param>
		/// <param name="pos">位置</param>
		/// <param name="color">色</param>
		/// <param name="rotation">回転</param>
		/// <param name="scale">大きさ</param>
		/// <param name="pivot">基点</param>
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

