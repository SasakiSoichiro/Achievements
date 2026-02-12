//*************************************************
//
// リザルト状態処理 [resultState.cpp]
// Author:Sasaki Soichiro
//
//*************************************************

//*******************
// インクルード
//*******************
#include "resultState.h"
#include "titleState.h"
#include "manager.h"
#include "camera.h"
#include "input.h"

//***************************
// コンストラクタ
//***************************
CResultState::CResultState()
{
}

//***************************
// デストラクタ
//***************************
CResultState::~CResultState()
{

}

//***************************
// 初期化処理
//***************************
void CResultState::Init(void)
{	// カメラの初期化
	CManager::GetCCamera()->SetType(CCamera::TYPE_RESULT);
	m_pGame->SetScene(CGame::SCENE_RESULT);
}

//***************************
// 更新
//***************************
void CResultState::Update(void)
{
	// シーン遷移
	if (CManager::GetCInputKeyboard()->Trigger(DIK_RETURN)
		|| CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START)
		|| CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A))
	{
		CTitleState* pTitleState = new CTitleState;
		m_pGame->ChangeState(pTitleState);
	}
}

//***************************
// 終了処理
//***************************
void CResultState::Uninit(void)
{

}