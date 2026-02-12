//**********************************************
// 
// カスタマー処理 [customer.h]
// Author:Sasaki Soichiro
//
//**********************************************
#ifndef _CUSTOMER_H_ // このマクロ定義がされていなかったら
#define _CUSTOMER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "item.h"
#include "customerState.h"
#include "motion.h"

//******************
// 前方宣言
//******************
class CModel;
class CItem;
class COrder;
class CMotion;
class CBowl;

//*******************
// 構造体
//*******************
typedef struct
{
	CItem::ITEM type;	// 欲しい種類
	bool bHave;			// 持っているか
}ORDER;

//*******************
// 構造体
//*******************
typedef struct
{
	int nX;
	int nZ;
}GRID;

//****************************
// 客クラス
//****************************
class CCustomer :CObject
{
public:
	// 状態
	typedef enum
	{
		STATE_ORDER=0,
		STATE_PLAY,
		STATE_BAGGING,
		STATE_GOBACK,
		STATE_PLAYORDER,
		STATE_MAX
	}STATE;

	// 探索時状態
	typedef enum
	{
		SEARCH_NULL0=0,
		SEARCH_NULL1,
		SEARCH_NONE,
		SEARCH_STAY,
		SEARCH_MAX
	}SEARCH;

	// モーション
	typedef enum
	{
		MOTION_NONE = 0,
		MOTION_MOVE,
		MOTION_PLAY,
		MOTION_LOSE,
		MOTION_WIN,
		MOTION_PLAYNUETORAL,
		MOTION_MAX
	}MOTION;

	CCustomer(int nPriority = 4);	// コンストラクタ
	~CCustomer();					// デストラクタ

	static CCustomer* Create(D3DXVECTOR3 pos);

	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	void ChangeState(CCustomerStateBase* pState);	// 状態を変える

	void UpdateRotDest(void);		// 目標の向きへの更新
	void ItemPos(void);				// アイテムの位置設定
	void InitPlay(void);			// オーダーを完了した瞬間の処理
	void Null0Update(void);			// すくう位置の探索更新:条件1
	void Null1Update(void);			// すくう位置の探索更新:条件2
	void NoneUpdate(void);			// すくう位置の探索更新:条件3
	void PlayPosSearch(void);		// すくう位置の探索
	bool IsNextGridUpdate(void);	// 1マスずつ進ませる処理
	void GoBagging(void);			// 袋詰めエリアに向かうときの更新
	void AngryCount(void);			// 怒るカウントの処理
	void Review(void);				// 評価する処理
	void AngryEffect(void);			// 怒るエフェクトの更新
	void PoiOut(void);				// ポイが破れた時の処理
	void RandPoiType(void);			// ポイ種類をランダムで決める
	void InitBagging(void);			// 袋詰めのための初期化処理
	void InitPlayOrder(void);		// 追加注文のための初期化処理
	bool IsPlay(void);				// 帰るか新しく注文するか
	void PosDestMove(void);			// 目標の位置に進む
	void ItemUninit(void);			// 手に持っているアイテムを消す

	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetRotDest(D3DXVECTOR3 rot) { m_rotDest = rot; }
	void SetState(STATE state) { m_state =state; }
	void SetBaggingPos(D3DXVECTOR3 baggingPos) { m_BaggingPos = baggingPos; }
	void SetPoiOut(bool bPoiOut) { m_bPoiOut = bPoiOut; }
	void SetMotion(int nType) { m_pMotion->SetMotion(nType); }
	void OrderScreen(bool bScreen);			// オーダーの表示切替
	void Receive(CItem* pItem);				// 受け取る
	void Death(void) { m_bDeath = true; }	// 更新途中で消すときの処理
	void SetUninitCnt(int nCnt) { m_UninitCnt = nCnt; }
	void SetScoop(bool bScoop) { m_bScoop = bScoop; }
	static void ResetNumAll(void) { m_nNumAll = 0; }
	// ゲッター
	CBowl* GetBowl(void) { return m_pBowl; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosDest(void) { return m_posDest; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }
	STATE GetState(void) { return m_state; }
	SEARCH GetSearch(void) { return m_Search; }
	ORDER GetOrder(int nCnt) { return m_Order[nCnt]; }
	bool ScoopFish(D3DXVECTOR3 pos);		// 金魚をすくう処理
	bool GetReceive(void) { return m_bReceive; }
	bool GetPlayReceive(void) { return m_bPlayReceive; }
	bool GetBagging(void) { return m_bBagging; }
	bool GetPoiOut(void) { return m_bPoiOut; }
	bool GetIsPlayArea(int nX, int nZ);
	bool GetScoop(void) { return m_bScoop; }
	int GetIdx(void) { return m_nIdx; }
	int GetMaxFishCnt(void) { return m_nMaxFishCnt; }
	int GetUninitCnt(void) { return m_UninitCnt; }
	static int GetNumAll(void) { return m_nNumAll; }
private:
	// 定数
	static constexpr int nMaxItemNum = 2;
	static constexpr int nMaxOrderNum = 2;
	static constexpr int maxModelParts = 12;
	static constexpr int nHandRIdx = 3;
	static constexpr int nItemModelIdx = 10;
	static constexpr int maxUninitCnt = 100;
	static constexpr int baggingLOrderNum = 4;
	static constexpr int angryLine0 = 4000;
	static constexpr int angryLine1 = 3500;
	static constexpr int angryLine2 = 2500;
	static constexpr int angryLine3 = 1500;
	static constexpr int AngryEffectCool = 50;
	static constexpr int AngryEffectRandXZ = 20;
	static constexpr int AngryEffectRandY = 8;
	static constexpr int nMinFishCnt = 2;
	static constexpr int nRandFishCnt = 4;
	static constexpr int nMaxSeachGridZ = 6;
	static constexpr int nMaxGridZ = 7;
	static constexpr int nAngryEffectNum = 3;
	static constexpr int nBaggingDestGridZ0 = 3;
	static constexpr int nBaggingDestGridZ1 = 7;
	static constexpr int nBaggingDestX0 = 1;
	static constexpr int nBaggingDestZ0 = 0;
	static constexpr int nBaggingDestX1 = 1;
	static constexpr int nBaggingDestZ1 = 6;
	static constexpr int nMaxReplayRand = 5;
	static constexpr int nReplayRand = 2;
	static constexpr int nMistakeAngryCnt = 500;
	static constexpr int nBaggingAngryCnt = 2;
	static constexpr float fSpeed = 2.5f;
	static constexpr float fRotDestRatio = 0.1f;
	static constexpr float ItemOffsetXZ = 20.0f;
	static constexpr float ItemOffsetY = 15.0f;
	static constexpr float BowlOffsetXZ = 30.0f;
	static constexpr float fMaxReviewScore = 5.0f;
	static constexpr float fOrderItemDistance = 20.0f;
	static constexpr float fDestPosRatio = 0.1f;
	static constexpr float fAngryEffectHeightOffset = 20.0f;
	static constexpr float fAngryEffectWidthOffset = 4.0f;
	static constexpr float fAngryEffectSize = 8.0f;
	static constexpr float fIsNextRadius = 5.0f;
	static constexpr float fScoopDist = 20.0f;
	static constexpr float fScoopRadius = 10.0f;
	static constexpr float fBaggingPosRatio = 0.01f;
	static constexpr float fReview0 = 1.0f;
	static constexpr float fReview1 = 2.0f;
	static constexpr float fReview2 = 1.0f;
	static constexpr float fReview3 = 0.3f;
	static constexpr float fAngryEffecrSize = 3.0f;
	static constexpr float fIsPlayRadius = 5.0f;
	// ポインタ
	CModel* m_apModel[maxModelParts];
	CMotion* m_pMotion;
	COrder* m_pOrder;
	CBowl* m_pBowl;
	CItem* m_pItem[nMaxItemNum];
	CStateMachine* m_Machine;
	// enum
	STATE m_state;
	SEARCH m_Search;
	// 構造体
	ORDER m_Order[nMaxOrderNum];
	GRID m_grid;
	// 座標
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_BaggingPos;
	D3DXMATRIX m_mtxWorld;	
	// フラグ
	bool m_bCorrect;
	bool m_bOrder;
	bool m_bReceive;
	bool m_bBagging;
	bool m_bPlayReceive;
	bool m_bScoop;			// すくっている途中か
	bool m_bScoopEnd;		// すくい終わり
	bool m_bQueue;			// 列に並んでいるか
	bool m_bDeath;
	bool m_bPoiOut;			// ポイが破れたか
	// 数値
	int m_nIdx;
	int m_nNumModel;
	int m_nScoopBrake;
	int m_nMaxFishCnt;		// 満足する魚のカウント
	int m_nAngryCnt;		// 怒るまでのカウント
	int m_nAngryEffectCnt;	// 怒りマークを出すカウント
	int m_UninitCnt;		// Uninitするまでのカウント
	static int m_nNumAll;
	float m_fReview;		// レビューする数値
};
#endif