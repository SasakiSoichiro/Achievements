//**************************************
//
// 状態管理処理 [state.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _STATE_H_ // このマクロ定義がされていなかったら
#define _STATE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include <iostream>

//********************
// ステート基底クラス
//********************
class CStateBase
{
public:
	CStateBase(){}			// コンストラクタ
	virtual ~CStateBase(){}	// デストラクタ
	virtual void Init(){}	// 初期化
	virtual void Update(){}	// 更新
	virtual void Uninit(){}	// 終了
};

//*********************
// ステート管理クラス
//*********************
class CStateMachine
{
public:
	CStateMachine();// コンストラクタ
	~CStateMachine();// デストラクタ
	void ChangeState(CStateBase* pNewState);// 状態切り替え
	void Uninit(void);						// 終了
	void Update(void);						// 更新
private:
	CStateBase* m_pNowState;	// ステートベースへのポインタ
};
#endif