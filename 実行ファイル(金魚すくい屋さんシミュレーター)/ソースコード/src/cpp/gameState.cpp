//*************************************************
//
// ゲーム状態処理 [gameState.cpp]
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
#include "startAndend.h"
#include "money.h"
#include "timer.h"
#include "goldfish.h"
#include "sound.h"
#include "customerManager.h"
#include "resultState.h"
#include "mathUtil.h"

// 定数
namespace GamePos
{// 静的オブジェクトの位置
	const D3DXVECTOR3 STARTANDEND = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
	const D3DXVECTOR3 MONEY = { 1200.0f, 680.0f, 0.0f };
	const D3DXVECTOR3 TIMER = { 125.0f, 30.0f, 0.0f };
};


//***************************
// コンストラクタ
//***************************
CGameState::CGameState()
{
	m_pMoney = nullptr;
	m_pStartAndEnd = nullptr;
	m_pTimer = nullptr;
	m_nCnt = 0;
}

//***************************
// デストラクタ
//***************************
CGameState::~CGameState()
{

}

//***************************
// 初期化処理
//***************************
void CGameState::Init(void)
{
	// カメラの初期化
	CManager::GetCCamera()->SetType(CCamera::TYPE_GAME);
	m_pStartAndEnd = CStartAndEnd::Create(GamePos::STARTANDEND);

	// UI
	m_pMoney = CMoney::Create(GamePos::MONEY);
	CMoney::SetScore(0);
	m_pTimer = CTimer::Create(GamePos::TIMER, nTimer);

	// 金魚の生成
	for (int nCnt = 0; nCnt < nGoldFishNum; nCnt++)
	{
		float X = (float)(rand() % nRandXZ) - MathUtil::Half<float>(nRandXZ);
		float Z = (float)(rand() % nRandXZ) - MathUtil::Half<float>(nRandXZ);
		int nRand = rand() % nRandMax;
		if (nRand > nBlackGoldFish)
		{// 赤色金魚生成
			m_pGoldFish.push_back(CGoldFish::Create(D3DXVECTOR3(X, fGoldFishHeight, Z), CGoldFish::TYPE_RED));
		}
		else
		{// 黒色金魚生成
			m_pGoldFish.push_back(CGoldFish::Create(D3DXVECTOR3(X, fGoldFishHeight, Z), CGoldFish::TYPE_BLACK));
		}
	}

	// マネージャの初期化
	CCustomerManager* pCustomerM = m_pGame->GetCCustomerManager();
	pCustomerM->Init();
	// 状態の初期化
	m_pGame->SetState(CGame::STATE_NONE);
	m_pGame->SetScene(CGame::SCENE_GAME);

	// 音
	CSound* pSound = CManager::GetCSound();
	pSound->Play(CSound::LABEL_SE06);
}

//***************************
// 更新
//***************************
void CGameState::Update(void)
{
	CGame::STATE GameState = m_pGame->GetState();
	// マネージャの更新
	CCustomerManager* pCustomerM = m_pGame->GetCCustomerManager();
	pCustomerM->Update();

	// 状態ごとの処理
	switch (GameState)
	{
	case CGame::STATE_NORMAL:
		break;
	case CGame::STATE_END:
		m_nCnt++;
		if (m_nCnt > nEndInterval)
		{
			m_nCnt = 0;
			CResultState* pResultState = new CResultState;
			m_pGame->ChangeState(pResultState);
			return;
		}
		break;
	}

#ifdef _DEBUG
	// シーン遷移
	if (CManager::GetCInputKeyboard()->Trigger(DIK_SPACE) || CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START))
	{
		CResultState* pResultState = new CResultState;
		m_pGame->ChangeState(pResultState);
	}
#endif // !_DEBUG

}

//***************************
// 終了処理
//***************************
void CGameState::Uninit(void)
{
	// 開店閉店のUI終了
	m_pStartAndEnd->Death();
	m_pStartAndEnd = nullptr;

	// タイマーの終了
	m_pTimer->Death();
	m_pTimer = nullptr;

	// お金の2DUI
	m_pMoney->Death();
	m_pMoney = nullptr;

	// 金魚の消去
	for (auto cust = m_pGoldFish.begin(); cust != m_pGoldFish.end(); )
	{
		CGoldFish* goldFish = *cust;
		goldFish->Death();
		cust = m_pGoldFish.erase(cust);
	}

	// マネージャーのリセット
	CCustomerManager* pCustomerM = m_pGame->GetCCustomerManager();
	pCustomerM->Death();
}

