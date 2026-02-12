//**********************************************
// 
// カスタマー状態処理 [customerState.h]
// Author:Sasaki Soichiro
//
//**********************************************
#ifndef _CUSTOMERSTATE_H_ // このマクロ定義がされていなかったら
#define _CUSTOMERSTATE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "state.h"

class CCustomer;

//******************************
// 客専用のステート基底クラス
//******************************
class CCustomerStateBase :public CStateBase
{
public:
	CCustomerStateBase();
	~CCustomerStateBase();
	void SetOwner(CCustomer* pCustomer);// 客のポインタ設定
protected:
	CCustomer* m_pCustomer;	// 客へのポインタ
};

//*******************
// 注文机にいるとき
//*******************
class COrderWaitState :public CCustomerStateBase
{
public:
	COrderWaitState();	// コンストラクタ
	~COrderWaitState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	static constexpr int nMaxItem = 2;
	static constexpr int nPassCnt = 2;

};

//****************
// 地面にいるとき
//****************
class CPlaySeachState :public CCustomerStateBase
{
public:
	CPlaySeachState();	// コンストラクタ
	~CPlaySeachState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	static constexpr float fSpeed = 2.5f;
};

//****************
// 地面にいるとき
//****************
class CPlayState :public CCustomerStateBase
{
public:
	CPlayState();		// コンストラクタ
	~CPlayState();		// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
	void Scoop(void);	// すくう動作
	void ScoopEnd(void);// すくい終わり
	void Switch(void);
private:
	static constexpr int nAnimStep0 = 350;
	static constexpr int nAnimStep1 = 430;
	static constexpr int nAnimStep2 = 450;
	static constexpr int nScoopEndSwicth = 180;
	static constexpr float fSpeed = 2.5f;
	bool m_bScoopEnd;
	int m_nScoopBrake;
};

//*******************
// 追加注文するとき
//*******************
class CPlayOrderState :public CCustomerStateBase
{
public:
	CPlayOrderState();	// コンストラクタ
	~CPlayOrderState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	static constexpr int nMaxItem = 2;
	static constexpr int nPassCnt = 1;
};

//*******************
// 袋詰めしてるとき
//*******************
class CBaggingWaitState :public CCustomerStateBase
{
public:
	CBaggingWaitState();	// コンストラクタ
	~CBaggingWaitState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	static constexpr int nMaxItem = 2;
	static constexpr int nPassCnt = 1;
};

//*******************
// 帰るとき
//*******************
class CGoBackState :public CCustomerStateBase
{
public:
	CGoBackState();		// コンストラクタ
	~CGoBackState();	// デストラクタ
	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Uninit(void);	// 終了
private:
	static constexpr float moveX = 1.0f;
	static constexpr float moveZ = -0.4f;
};

#endif