//***************************************
// 
// 客マネージャー処理 [customerManager.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _CUSTOMERMANAGER_H_ // このマクロ定義がされていなかったら
#define _CUSTOMERMANAGER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include <vector>

//******************
// 前方宣言
//******************
class CCustomer;
class CItem;


//************************
// 客マネージャークラス
//************************
class CCustomerManager
{
public:

	typedef enum
	{// 遊ぶエリアの種類
		PLAYAREA_TYPE_NONE = 0,
		PLAYAREA_TYPE_NULL,
		PLAYAREA_TYPE_FILL,
		PLAYAREA_TYPE_MAX

	}PLAYAREA_TYPE;

	typedef struct
	{// 遊べるマスの構造体
		D3DXVECTOR3 pos;
		PLAYAREA_TYPE type;
	}PLAYAREA;

	CCustomerManager();	// コンストラクタ
	~CCustomerManager();// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void OrderUpdate(void);	// 注文時の更新
	void PlayUpdate(void);	// 遊んでいるときの更新
	void PlayOrderUpdate(void);	// 追加注文しているときの更新
	void BaggingUpdate(void);	// 袋詰めしているときの更新
	void Order(CItem* pItem);	// 注文時
	void Play(CItem* pItem);	// 注文時
	void Bagging(CItem* pItem);	// 注文時
	void SetPlayAreaGrid(int nCntX, int nCntZ, PLAYAREA_TYPE type);
	PLAYAREA GetPlayAreaGrid(int nCntX, int nCntZ);
	void Death(void);
	void SetDeath(void) { m_bDeath = true; }
	std::vector<CCustomer*> GetPlayCustomer(void) { return m_Play; }
	bool GetIsFill(void);
private:
	// 定数
	static constexpr int nMaxPlayCustomer = 6;
	static constexpr int nMinPlayAreaGrid = 1;
	static constexpr int nMaxPlayAreaGrid = 7;
	static constexpr int nMaxGridX = 2;
	static constexpr int nMaxGridZ = 7;
	static constexpr int nMaxCustomer = 10;
	static constexpr int nMoneyAdd = 400;
	static constexpr float fDistanceLine = 30.0f;
	static constexpr float fGridSize = 30.0f;
	static constexpr float fFullCustoUISizeX = 30.0f;
	static constexpr float fFullCustoUISizeZ = 10.0f;
	// ポインタ
	std::vector<CCustomer*> m_Customer;
	std::vector<CCustomer*> m_Order;
	std::vector<CCustomer*> m_Play;
	std::vector<CCustomer*> m_PlayOrder;
	std::vector<CCustomer*> m_Bagging;
	// 構造体
	PLAYAREA m_playAreaGrid[nMaxGridX][nMaxGridZ];
	// フラグ
	bool m_bDeath;
	// 数値
	int m_nCntOrder;
	int m_nCntPlay;
	int m_nCntPlayOrder;
	int m_nCntBagging;
};
#endif