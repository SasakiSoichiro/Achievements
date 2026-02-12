//**********************************************
// 
// プレイヤー処理 [player.h]
// Author:Sasaki Soichiro
//
//**********************************************
#ifndef _PLAYER_H_ // このマクロ定義がされていなかったら
#define _PLAYER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "state.h"
#include <string>
#include "motion.h"

//******************
// 前方宣言
//******************
class CModel;
class CPlayerStateBase;
class CItem;
class CFishingLine;
class CMotion;

//****************************
// プレイヤークラス
//****************************
class CPlayer :CObject
{
public:
	typedef enum
	{// 状態
		STATE_NONE=0,
		STATE_ORDER,
		STATE_BAGGING,
		STATE_MAX
	}STATE;

	typedef enum
	{// 止まる場所
		POINT_ORDER=0,
		POINT_CORNER0,
		POINT_PLAY,
		POINT_CORNER1,
		POINT_BAGGING,
		POINT_MAX
	}POINT;

	typedef enum
	{// モーションの種類
		MOTION_NONE=0,
		MOTION_WALK,
		MOTION_FISHING,
		MOTION_MAX
	}MOTION;

	CPlayer();	// コンストラクタ
	~CPlayer();	// デストラクタ

	static CPlayer* Create(D3DXVECTOR3 pos);	// インスタンス生成
	HRESULT Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void MoveControll(void);	// コントロール
	void Move(void);			// プレイヤーの動き
	void ItemPos(void);
	void ChangeState(CPlayerStateBase* pState);	// 状態を変える
	void StateControll(void);
	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetState(STATE state) { m_state = state; }
	void SetItem(CItem* pItem) { m_pItem = pItem; }
	void SetIsFishing(bool bFishing) { m_bFishing = bFishing; }
	void SetIsFishingItem(bool bFishingItem) { m_bFishingItem = bFishingItem; }
	// ゲッター
	CItem* GetItem(void) { return m_pItem; }
	CFishingLine* GetFishingLine(void) { return m_pFishingLine; }
	CMotion* GetMotion(void) { return m_pMotion; }
	const D3DXVECTOR3 GetPos(void) const{ return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	STATE GetState(void) { return m_state; }
	bool GetIsFishing(void) { return m_bFishing ; }
	bool GetIsFishingItem(void) { return m_bFishingItem; }
	bool GetIsFishingCancel(void);
	bool GetIsMove(void) { return m_bMove; }
	int GetSelect(void) { return m_nSelect; }
private:
	// 定数
	static constexpr int deltaPoint = 2;
	static constexpr int maxPoint = 5;
	static constexpr int maxModelParts = 12;
	static constexpr float fSpeed = 8.0f;
	static constexpr float fishingOffsetY = 15.0f;
	static constexpr float fRotRatio = 0.2f;
	static constexpr float fItemDistanceXZ = 20.0f;
	static constexpr float fItemDistanceY = 10.0f;
	static constexpr float fTurnTurnRadius = 50.0f;
	static const D3DXVECTOR3 POINT[maxPoint];
	// ポインタ
	CModel* m_apModel[maxModelParts];	// モデル
	CMotion* m_pMotion;					// モーション
	CFishingLine* m_pFishingLine;		// 釣り糸
	CStateMachine* m_Machine;			// ステートマシン
	CItem* m_pItem;						// アイテム
	// enum
	STATE m_state;	// 状態
	// 座標
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_rotDest;	// 目標の向き
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	//	フラグ
	bool m_bMove;			// 動いたか(チュートリアル用)
	bool m_bFishing;		// 釣り糸を出したか(チュートリアル用)
	bool m_bFishingItem;	// 釣り糸でアイテムを取ったか(チュートリアル用)
	// 数値
	int m_nNumModel;		// モデルのパーツ数
	int m_nCurrentPoint;	// 現在のポイント
	int m_nDestPoint;		// 目標のポイント
	int m_nSelect;			// 選択
};

//**************************************
// プレイヤー専用のステート基底クラス
//**************************************
class CPlayerStateBase :public CStateBase
{
public:
	CPlayerStateBase();				// コンストラクタ
	~CPlayerStateBase();			// デストラクタ
	void SetOwner(CPlayer* pPlayer);// プレイヤーのポインタ設定
protected:
	// 定数
	static constexpr int nFishingHaveMax = 2;// 釣り糸が持てるアイテムの最大数
	// ポインタ
	CPlayer* m_pPlayer;	// プレイヤーへのポインタ
};

//****************
// 地面にいるとき
//****************
class CNormalState :public CPlayerStateBase
{
public:
	CNormalState();		// コンストラクタ
	~CNormalState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	int m_nCnt;
};

//*******************
// 注文机にいるとき
//*******************
class COrderDeskState :public CPlayerStateBase
{
public:
	COrderDeskState();	// コンストラクタ
	~COrderDeskState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	int m_nCnt;
};

//*******************
// 袋詰めしてるとき
//*******************
class CBaggingState :public CPlayerStateBase
{
public:
	CBaggingState();	// コンストラクタ
	~CBaggingState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
};

#endif