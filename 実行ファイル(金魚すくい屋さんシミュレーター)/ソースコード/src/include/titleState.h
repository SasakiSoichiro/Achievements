//************************************
//
// タイトル状態処理 [titleState.h]
// Author:Sasaki Soichiro
//
//************************************
#ifndef _TITLESTATE_H_ // このマクロ定義がされていなかったら
#define _TITLESTATE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "game.h"

//******************
// タイトル
//******************
class CTitleState :public CGameStateBase
{
public:
	CTitleState();		// コンストラクタ
	~CTitleState();		// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
	void FlowerWorks(void);
private:
	// 定数
	static constexpr int nInterval = 12;
	static constexpr int nRandY = 50;
	static constexpr int nRandZ = 4000;
	static constexpr int nType0Num = 7;
	static constexpr int nType1Num = 5;
	static constexpr int nType2Num = 4;
	static constexpr float fOffsetX = 2000.0f;
	static constexpr float fOffsetY = 1300.0f;
	static constexpr float fOffsetZ = 2000.0f;
	// ポインタ
	CTitleUI* m_pTitle;
	// 数値
	int m_nCnt;
};

#endif