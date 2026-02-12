//**************************************
//
// 状態管理処理 [state.cpp]
// Author:Sasaki Soichiro
//
//**************************************
//******************
// インクルード
//******************
#include "state.h"

//****************
// コンストラクタ
//****************
CStateMachine::CStateMachine()
{
	m_pNowState = NULL;
}

//****************
// デストラクタ
//****************
CStateMachine::~CStateMachine()
{
}

//********************
// ステートを変更する
//********************
void CStateMachine::ChangeState(CStateBase* pNewState)
{
	if (m_pNowState != NULL)
	{// NULLじゃなかったら
		
		m_pNowState->Uninit();	// 終了
		delete m_pNowState;		// 解放
		m_pNowState = NULL;		// NULLを代入
	}
	// 新しいステートをセットする
	m_pNowState = pNewState;

	// 新しいステートを開始する
	m_pNowState->Init();
}

//********************
// 終了処理
//********************
void CStateMachine::Uninit(void)
{
	if (m_pNowState != NULL)
	{// NULLじゃなかったら

		m_pNowState->Uninit();	// 終了
		delete m_pNowState;		// 解放
		m_pNowState = NULL;		// NULLを代入
	}
}

//**************
// 更新処理
//**************
void CStateMachine::Update(void)
{
	if (m_pNowState != NULL)
	{//NULLじゃなかったら
		m_pNowState->Update();	// 更新
	}
}