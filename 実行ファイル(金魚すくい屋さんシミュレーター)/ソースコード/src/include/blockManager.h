//**********************************************
// 
// ブロックマネージャー処理 [blockManager.h]
// Author:Sasaki Soichiro
//
//**********************************************
#ifndef _BLOCKMANAGER_H_ // このマクロ定義がされていなかったら
#define _BLOCKMANAGER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include <vector>
#include "Xmodel.h"
#include "block.h"


//****************************
// ブロックマネージャークラス
//****************************
class CBlockManager
{
public:
	CBlockManager();	// コンストラクタ
	~CBlockManager();	// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Set(CBlock*block);// ブロック情報の設定
private:
	std::vector<CBlock*> m_Block;// ブロックのポインタ
};
#endif