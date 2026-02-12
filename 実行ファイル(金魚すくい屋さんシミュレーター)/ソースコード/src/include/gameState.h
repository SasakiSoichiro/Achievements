//******************************************
//
// ゲーム状態処理 [gameState.h]
// Author:Sasaki Soichiro
//
//******************************************
#ifndef _GAMESTATE_H_ // このマクロ定義がされていなかったら
#define _GAMESTATE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "game.h"

//*******************
// ゲーム
//*******************
class CGameState :public CGameStateBase
{
public:
	CGameState();	// コンストラクタ
	~CGameState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	// 定数
	static constexpr int nEndInterval = 60;
	static constexpr int nGoldFishNum = 250;
	static constexpr int nBlackGoldFish = 20;
	static constexpr int nRandXZ = 80;
	static constexpr int nRandMax = 100;
	static constexpr int nTimer = 120;
	static constexpr float fGoldFishHeight = -6.0f;
	// ポインタ
	CStartAndEnd* m_pStartAndEnd;
	CTimer* m_pTimer;
	CMoney* m_pMoney;
	std::vector<CGoldFish*> m_pGoldFish;
	// 数値
	int m_nCnt;
};

#endif