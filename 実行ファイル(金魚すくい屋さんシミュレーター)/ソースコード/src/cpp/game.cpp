//**********************************
//
// ゲーム処理 [game.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "scene.h"
#include "game.h"
#include "renderer.h"
#include "object2D.h"
#include <time.h>
#include "debugproc.h"
#include "object3D.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "mesh.h"
#include "manager.h"
#include "block.h"
#include "numbermulti.h"
#include "sound.h"
#include "blockManager.h"
#include "blurManager.h"
#include "input.h"
#include "fade.h"
#include "aquarium.h"
#include "goldfish.h"
#include "player.h"
#include "customer.h"
#include "customerManager.h"
#include "orderDesk.h"
#include "baggingDesk.h"
#include "itemBoxManager.h"
#include "fishingLine.h"
#include "order.h"
#include "camera.h"
#include "review.h"
#include "stall.h"
#include "whiteboard.h"
#include "tutorialSkip.h"
#include "controllTutorial.h"
#include "rStick.h"
#include "titleState.h"
#include "passerbyManager.h"
#include "pause.h"

//*******************
// 静的メンバ変数
//*******************
CObject3D* CGame::m_pObject3D = NULL;
CMesh* CGame::m_pMesh = NULL;
CPlayer* CGame::m_pPlayer = NULL;
CWave* CGame::m_pWave = NULL;
CBlockManager* CGame::m_pBlockManager = NULL;
CPauseManager* CGame::m_pPauseManager = NULL;
CCustomerManager* CGame::m_pCustomerManager = NULL;
COrderDesk* CGame::m_pOrderDesk = NULL;
CBaggingDesk* CGame::m_pBaggingDesk = NULL;
CItemBoxManager* CGame::m_pItemBoxManager = NULL;
CMoney* CGame::m_pMoney = NULL;
CGame::SCENE CGame::m_Scene = SCENE_TITLE;
CGame::STATE CGame::m_State = STATE_NONE;

// 定数
namespace StageConstPos
{// 静的オブジェクトの位置
	const D3DXVECTOR3 WATERSURFACE = { 0.0f,-2.0f,0.0f };
	const D3DXVECTOR3 AQUARIUM = { 0.0f,-23.0f,0.0f };
	const D3DXVECTOR3 ORDERBOARD = { 30.0f,0.0f,180.0f };
	const D3DXVECTOR3 BAGGINGBOARD = { 30.0f,0.0f,-180.0f };
	const D3DXVECTOR3 STALL0 = { 0.0f, 0.0f, -350.0f };
	const D3DXVECTOR3 STALL1 = { 0.0f, 0.0f, 350.0f };
	const D3DXVECTOR3 STALL2 = { 350.0f, 0.0f, -350.0f };
	const D3DXVECTOR3 STALL3 = { 350.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 STALL4 = { 350.0f, 0.0f, 350.0f };
	const D3DXVECTOR3 GROUND = { 0.0f, -26.0f, 0.0f };
	const D3DXVECTOR3 BLUESEAT = { 0.0f, -25.5f, 0.0f };
	const D3DXVECTOR3 MOUNTAIN = { 2000.0f, 300.0f, 0.0f };
	const D3DXVECTOR3 ORDERDESK = { 0.0f, 0.0f, 150.0f };
	const D3DXVECTOR3 BAGGINGDESK = { 0.0f, 0.0f, -150.0f };
	const D3DXVECTOR3 PLAYER = { -150.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 WHITEBOARD = { -120.0f, 0.0f, 160.0f };
	const D3DXVECTOR3 RSTICK = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
	const D3DXVECTOR3 PAUSE = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
};
namespace StageConstSize
{// 静的オブジェクトの大きさ
	const D3DXVECTOR2 GROUND = { 1000.0f, 1500.0f };
	const D3DXVECTOR2 BLUESEAT = { 200.0f, 300.0f };
	const D3DXVECTOR2 MOUNTAIN = { 8000.0f,1500.0f };
};
namespace StageConstTex
{// 静的オブジェクトのテクスチャ座標
	const D3DXVECTOR2 MAX = { 1.0f,1.0f };
	const D3DXVECTOR2 BLUESEAT = { 0.4f,0.4f };
}

//*******************
// コンストラクタ
//*******************
CGame::CGame():CScene(CScene::MODE_GAME)
{
	m_nCnt = 0;
	m_Machine = nullptr;
	m_pMountain = nullptr;
	m_pPasserbyManager = nullptr;
}

//*******************
// デストラクタ
//*******************
CGame::~CGame()
{
}

//*************
// 初期化処理
//*************
HRESULT CGame::Init(void)
{
	// BGM再生
	CSound* pSound = CManager::GetCSound();
	pSound->Play(CSound::LABEL_BGM00);
	pSound->Play(CSound::LABEL_SE03);
	// 全体のランダムのため
	srand((unsigned)time(NULL));

	m_State = STATE_NONE;
	m_nCnt = 0;
	m_pBlockManager = new CBlockManager;

	// 水面
	m_pMesh = CMesh::Create(StageConstPos::WATERSURFACE);

	// 水槽
	CAquarium::Create(StageConstPos::AQUARIUM);

	// 立て看板
	CBlock::Create(StageConstPos::BAGGINGBOARD,-PI_HALF,"data\\MODEL\\bagging.x", 0);
	CBlock::Create(StageConstPos::ORDERBOARD, PI_HALF, "data\\MODEL\\order.x", 0);

	// 屋台
	CStall::Create(StageConstPos::STALL0, 0.0f);
	CStall::Create(StageConstPos::STALL1, 0.0f);
	CStall::Create(StageConstPos::STALL2, D3DX_PI);
	CStall::Create(StageConstPos::STALL3, D3DX_PI);
	CStall::Create(StageConstPos::STALL4, D3DX_PI);

	// 地面
	CObject3D::Create(StageConstPos::GROUND,
		StageConstSize::GROUND,
		VECTOR2_NULL,
		StageConstTex::MAX,
		"data\\TEXTURE\\floor.jpg");

	// ブルーシート
	CObject3D::Create(StageConstPos::BLUESEAT,
		StageConstSize::BLUESEAT,
		VECTOR2_NULL,
		StageConstTex::BLUESEAT,
		"data\\TEXTURE\\blueSeat.jpg");

	// RスティックUI
	CRStick::Create(StageConstPos::RSTICK);

	// ポーズ
	CPause::Create(StageConstPos::PAUSE);

	// 山
	m_pMountain = CObject3D::Create(StageConstPos::MOUNTAIN,
		StageConstSize::MOUNTAIN,
		VECTOR2_NULL,
		StageConstTex::MAX,
		"data\\TEXTURE\\mountain.png");
	m_pMountain->SetRot(D3DXVECTOR3(-PI_HALF, 0.0f, PI_HALF));

	// ホワイトボード
	CWhiteboard::Create(StageConstPos::WHITEBOARD);

	// 注文机と袋詰め机
	m_pOrderDesk = COrderDesk::Create(StageConstPos::ORDERDESK);
	m_pBaggingDesk = CBaggingDesk::Create(StageConstPos::BAGGINGDESK);

	// アイテムボックスマネージャ
	m_pItemBoxManager = new CItemBoxManager;
	m_pItemBoxManager->Init();

	// プレイヤー
	m_pPlayer = CPlayer::Create(StageConstPos::PLAYER);

	// 客の初期化
	m_pCustomerManager = new CCustomerManager;

	m_pPasserbyManager = new CPasserbyManager;;

	CReview::Reset();

	// ステートマシンの生成と初期化
	m_Machine = new CStateMachine;
	CTitleState* pTitleState = new CTitleState;
	ChangeState(pTitleState);
	
	return S_OK;
}

//*************
// 終了処理
//*************
void CGame::Uninit(void)
{

	if (m_Machine != NULL)
	{
		m_Machine->Uninit();
		delete m_Machine;
		m_Machine = NULL;
	}
	if (m_pBlockManager != NULL)
	{
		m_pBlockManager->Uninit();
		delete m_pBlockManager;
		m_pBlockManager = NULL;
	}
	if (m_pCustomerManager != nullptr)
	{
		m_pCustomerManager->Uninit();
		delete m_pCustomerManager;
		m_pCustomerManager = nullptr;
	}
	if (m_pItemBoxManager != nullptr)
	{
		m_pItemBoxManager->Uninit();
		delete m_pItemBoxManager;
		m_pItemBoxManager = nullptr;
	}
	if (m_pPasserbyManager != nullptr)
	{
		m_pPasserbyManager->Uninit();
		delete m_pPasserbyManager;
		m_pPasserbyManager = nullptr;
	}

	CObject::Release();
}

//*************
// 更新処理
//*************
void CGame::Update(void)
{
	m_Machine->Update();
	m_pPasserbyManager->Update();
}

//**************
// 描画処理
//**************
void CGame::Draw(void)
{
	
}

//**************
// 状態を変える
//**************
void CGame::ChangeState(CGameStateBase* pState)
{
	pState->SetOwner(this);
	m_Machine->ChangeState(pState);
}

//***************************
// プレイヤーステートベース
//***************************
//***************************
// コンストラクタ
//***************************
CGameStateBase::CGameStateBase()
{
	m_pGame = NULL;
}

//***************************
// デストラクタ
//***************************
CGameStateBase::~CGameStateBase()
{

}

//********************************
// ゲームのポインタを設定する
//********************************
void CGameStateBase::SetOwner(CGame* pGame)
{
	m_pGame = pGame;
}