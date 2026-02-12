//******************************************
//
// チュートリアル状態処理 [tutorialState.h]
// Author:Sasaki Soichiro
//
//******************************************
#ifndef _TUTORIALSTATE_H_ // このマクロ定義がされていなかったら
#define _TUTORIALSTATE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "game.h"

//****************
// チュートリアル
//****************
class CTutorialState :public CGameStateBase
{
public:
	CTutorialState();	// コンストラクタ
	~CTutorialState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	CTutorialSkip* m_pTutorialSkip;
	CControllTutorial* m_pControllTutorial;
};

#endif