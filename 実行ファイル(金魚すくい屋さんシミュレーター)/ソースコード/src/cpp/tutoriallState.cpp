//*************************************************
//
// チュートリアル状態処理 [tutorialState.cpp]
// Author:Sasaki Soichiro
//
//*************************************************

//*******************
// インクルード
//*******************
#include "tutorialState.h"
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "tutorialSkip.h"
#include "controllTutorial.h"
#include "gameState.h"
#include "mathUtil.h"

// 定数
namespace TutorialPos
{// 静的オブジェクトの位置
	const D3DXVECTOR3 TUTORIALSKIP = { 1100.0f, 650.0f, 0.0f };
	const D3DXVECTOR3 CONTROLLTUTORIAL = { MathUtil::Half<float>SCREEN_WIDTH, SCREEN_HEIGHT * 0.2f, 0.0f };
};

//***************************
// コンストラクタ
//***************************
CTutorialState::CTutorialState()
{
	m_pTutorialSkip = nullptr;
	m_pControllTutorial = nullptr;
}

//***************************
// デストラクタ
//***************************
CTutorialState::~CTutorialState()
{

}

//***************************
// 初期化処理
//***************************
void CTutorialState::Init(void)
{
	// カメラの初期化
	CManager::GetCCamera()->SetType(CCamera::TYPE_GAME);
	m_pGame->SetScene(CGame::SCENE_TUTORIAL);
	m_pTutorialSkip = CTutorialSkip::Create(TutorialPos::TUTORIALSKIP);
	m_pControllTutorial = CControllTutorial::Create(TutorialPos::CONTROLLTUTORIAL);
}

//***************************
// 更新
//***************************
void CTutorialState::Update(void)
{
	// シーン遷移
	if (CManager::GetCInputKeyboard()->Trigger(DIK_SPACE) || CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START))
	{
		m_pTutorialSkip->Death();
		m_pControllTutorial->Death();
		CGameState* pGameState = new CGameState;
		m_pGame->ChangeState(pGameState);
	}
}

//***************************
// 終了処理
//***************************
void CTutorialState::Uninit(void)
{
}
