//***************************************
// 
// 通行人マネージャー処理 [passerbyManager.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _PASSERBYMANAGER_H_ // このマクロ定義がされていなかったら
#define _PASSERBYMANAGER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"

//***************************
// 通行人マネージャークラス
//***************************
class CPasserbyManager
{
public:
	CPasserbyManager();	// コンストラクタ
	~CPasserbyManager();// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新


private:
	// 関数
	void Create(void);
	// 定数
	static constexpr int nMaxRand = 100;		// 確率
	static constexpr int nDashRand = 20;		// 走る確率
	static constexpr int nCoolDown = 200;		// クルータイム
	static constexpr int nRandNum = 2;			// 確率に出す値
	static constexpr float backPosX = 125.0f;	// 奥
	static constexpr float flontPosX = 120.0f;	// 手前
	static constexpr float rightPosZ = 330.0f;	// 右
	static constexpr float leftPosZ = -330.0f;	// 左
	// 数値
	int m_nCnt;
};
#endif