//******************************************
//
// リザルト状態処理 [resultState.h]
// Author:Sasaki Soichiro
//
//******************************************
#ifndef _RESULTSTATE_H_ // このマクロ定義がされていなかったら
#define _RESULTSTATE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "game.h"

//*******************
// リザルト
//*******************
class CResultState :public CGameStateBase
{
public:
	CResultState();		// コンストラクタ
	~CResultState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
};

#endif