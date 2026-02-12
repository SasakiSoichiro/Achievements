//***************************************
// 
// ブラーマネージャー処理 [blurManager.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _BLURMANAGER_H_ // このマクロ定義がされていなかったら
#define _BLURMANAGER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"

//***************************
// ブラーマネージャークラス
//***************************
class CBlurManager
{
public:
	CBlurManager();	// コンストラクタ
	~CBlurManager();// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	static void Set(int nTime) { m_nTime = nTime; }// 時間の設定
	static bool GetBool(void) { return m_bTime; }// ブラー中か？情報取得
private:
	static bool m_bTime;	//ブラー中か？
	static int m_nTime;		// 時間
};
#endif