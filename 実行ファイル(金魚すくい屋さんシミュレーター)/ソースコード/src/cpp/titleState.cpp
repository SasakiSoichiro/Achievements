//**********************************
//
// タイトル状態処理 [titleState.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "titleState.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "titleUI.h"
#include "flowerworks.h"
#include "tutorialState.h"
#include "mathUtil.h"

// 定数
namespace TitlePos
{// 静的オブジェクトの位置
	const D3DXVECTOR3 TITLE = { MathUtil::Half<float>(SCREEN_WIDTH), 350.0f, 0.0f};
};

//***************************
// コンストラクタ
//***************************
CTitleState::CTitleState()
{
	m_pTitle = nullptr;
	m_nCnt = 0;
}

//***************************
// デストラクタ
//***************************
CTitleState::~CTitleState()
{

}

//***************************
// 初期化処理
//***************************
void CTitleState::Init(void)
{
	// カメラの初期化
	CManager::GetCCamera()->SetType(CCamera::TYPE_TITLE);
	m_pTitle = CTitleUI::Create(TitlePos::TITLE);
	m_pGame->SetScene(CGame::SCENE_TITLE);
}

//***************************
// 更新
//***************************
void CTitleState::Update(void)
{
	// 花火
	FlowerWorks();

	// シーン遷移
	if (CManager::GetCInputKeyboard()->Trigger(DIK_RETURN) || CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START))
	{
		m_pTitle->Death();
		CTutorialState* pTutorialState = new CTutorialState;
		m_pGame->ChangeState(pTutorialState);
	}
}

//***************************
// 終了処理
//***************************
void CTitleState::Uninit(void)
{
	m_pTitle = nullptr;
}

//***************************
// 花火
//***************************
void CTitleState::FlowerWorks(void)
{
	m_nCnt++;
	// 花火を打ち上げる処理
	if (m_nCnt > nInterval)
	{
		float fY = (float)(rand() % nRandZ) + fOffsetY;
		float fZ = (float)(rand() % nRandZ) - fOffsetZ;
		CFlowerworks::Create(D3DXVECTOR3(fOffsetX, fY, fZ), nType0Num, nType1Num, nType2Num);
		m_nCnt = 0;
	}
}