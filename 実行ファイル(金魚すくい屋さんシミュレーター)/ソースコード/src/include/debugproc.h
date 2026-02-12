//***************************************
//
// デバッグフォント処理 [debugproc.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _DEBUGPROC_H_ // このマクロ定義がされていなかったら
#define _DEBUGPROC_H_ // 二重インクルード防止のマクロ定義
//****************
// インクルード
//****************
#include "main.h"

//*********************
// デバッグ表示クラス
//*********************
class CDebugProc
{
public:
	CDebugProc();	// コンストラクタ
	~CDebugProc();	// デストラクタ
	void Init(void);						// 初期化
	void Uninit(void);						// 終了
	static void Print(const char*fmt,...);	// 文字列表示
	static void Draw(void);					// 描画
	static void SwitchDebug(void) { m_bDebug = !m_bDebug; }	// デバッグ表示切替
private:
	//　定数
	static constexpr int nMaxFont = 1024;
	static constexpr int nHeightFont = 18;
	static constexpr int nPrecision = 6;	// 精度
	static constexpr int nRadix = 10;		// 基数
	static constexpr int nMaxFloatDigit = 6;// 桁数
	// 構造体
	static LPD3DXFONT m_pFont;		// フォントへのポインタ
	//　フラグ
	static bool m_bDebug;			// デバッグ表示切替用
	// 文字
	static char m_aStr[nMaxFont];	// 文字列を格納するバッファ

};
#endif