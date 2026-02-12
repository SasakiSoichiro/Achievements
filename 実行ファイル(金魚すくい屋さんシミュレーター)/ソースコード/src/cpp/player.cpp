//***************************************
//
// プレイヤー処理 [player.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//*******************
// インクルード
//*******************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "orderDesk.h"
#include "baggingDesk.h"
#include "customerManager.h"
#include "item.h"
#include "itemBoxManager.h"
#include "fishingLine.h"
#include "specBase.h"
#include "motion.h"
#include "sound.h"
#include "mesh.h"
#include "rStick.h"
#include <fstream>
#include <string>
#include <sstream>
#include "loadMotion.h"
#include "loadMotionManager.h"

// 定数
const D3DXVECTOR3 CPlayer::POINT[5] = { 
	{-35.0f,0.0f,150.0f},
	{-100.0f,0.0f,150.0f},
	{-100.0f,0.0f,0.0f},
	{-100.0f,0.0f,-150.0f},
	{-35.0f,0.0f,-150.0f} };

//*******************
// コンストラクタ
//*******************
CPlayer::CPlayer()
{
	for (int nCnt = 0; nCnt < maxModelParts; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}
	m_pMotion = nullptr;
	m_nNumModel = 0;
	m_mtxWorld = {};
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_rotDest = VECTOR3_NULL;
	m_bMove = false;
	m_bFishing = false;
	m_bFishingItem = false;
	m_nSelect = 0;
	m_nCurrentPoint = 0;
	m_nDestPoint = 0;
	m_state = STATE_NONE;
	m_Machine = nullptr;
	m_pItem = nullptr;
	m_pFishingLine = nullptr;
}

//*******************
// デストラクタ
//*******************
CPlayer::~CPlayer()
{
}

//*******************
// インスタンス生成
//*******************
CPlayer* CPlayer::Create(D3DXVECTOR3 pos)
{
	// 自身のポインタ
	CPlayer* pPlayer;

	// インスタンス生成
	pPlayer = new CPlayer;

	// 初期化
	pPlayer->SetPos(pos);
	pPlayer->Init();

	return pPlayer;
}

//*************
// 初期化処理
//*************
HRESULT CPlayer::Init(void)
{
	m_pFishingLine = CFishingLine::Create(GetPos());
	m_Machine = new CStateMachine;
	COrderDeskState* pOrderState = new COrderDeskState;
	ChangeState(pOrderState);

	// モーションの読み込み
	int nMotionIdx = -1;
	nMotionIdx = CManager::GetLoadMotionManager()->Register("data\\MOTION\\motion_staff.txt");
	m_pMotion = CManager::GetLoadMotionManager()->GetAddres(nMotionIdx, &m_apModel[0], m_nNumModel);

	return S_OK;
}

//*************
// 終了処理
//*************
void CPlayer::Uninit(void)
{
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}

	if (m_Machine != NULL)
	{
		m_Machine->Uninit();
		delete m_Machine;
		m_Machine = NULL;
	}
	CObject::Release();
}

//*************
// 更新処理
//*************
void CPlayer::Update(void)
{
	m_bFishingItem = false;
	Move();
	// アイテムの位置設定
	ItemPos();

	if(m_pFishingLine->GetIsHookAnim())
	{
		m_rot.y = D3DX_PI + m_pFishingLine->GetRot().y;
		m_rotDest = m_rot;
	}

	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= PI_DOUBLE;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{
		m_rotDest.y += PI_DOUBLE;
	}

	m_rot += (m_rotDest - m_rot) * fRotRatio;

	m_Machine->Update();
	m_pMotion->Update();
}

//*************
// 描画処理
//*************
void CPlayer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}

//*********************
// アイテムの位置設定
//*********************
void CPlayer::ItemPos(void)
{
	if (m_pItem == nullptr)return;

	D3DXVECTOR3 pos = m_pos;
	pos.x -= sinf(m_rot.y) * fItemDistanceXZ;
	pos.y += fItemDistanceY;
	pos.z -= cosf(m_rot.y) * fItemDistanceXZ;
	m_pItem->SetPos(pos);
}

void CPlayer::MoveControll(void)
{
	// 左に移動
	bool bLeft = CManager::GetCInputKeyboard()->Trigger(DIK_A)
		|| CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_LEFT);
	// 右に移動
	bool bRight = CManager::GetCInputKeyboard()->Trigger(DIK_D)
		|| CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_RIGHT);

	if (bLeft && m_nDestPoint > 0)
	{
		m_nDestPoint -= deltaPoint;
		StateControll();
		m_bMove = true;
	}
	else if (bRight && m_nDestPoint < maxPoint - 1)
	{
		m_nDestPoint += deltaPoint;
		StateControll();
		m_bMove = true;
	}
	else
	{
		m_bMove = false;
	}
}
//******************
// XZ平面の移動処理
//******************
void CPlayer::Move(void)
{
	// 操作
	MoveControll();

	float fDistance = (m_pos.x - POINT[m_nCurrentPoint].x) * (m_pos.x - POINT[m_nCurrentPoint].x) 
					+ (m_pos.z - POINT[m_nCurrentPoint].z) * (m_pos.z - POINT[m_nCurrentPoint].z);

	// 音
	CSound* pSound = CManager::GetCSound();

	if (fDistance <= fTurnTurnRadius)
	{//ターン地点まで来たら
		if (m_nCurrentPoint == m_nDestPoint)
		{// 目標と一致
			m_rotDest.y = -D3DX_PI*0.5f;
			pSound->Stop(CSound::LABEL_SE05);
			return;
		}
		else if (m_nCurrentPoint < m_nDestPoint)
		{// 目標より小さい
			m_nCurrentPoint++;
			m_pMotion->SetMotion(MOTION_WALK);
			pSound->Play(CSound::LABEL_SE05);
		}
		else if (m_nCurrentPoint > m_nDestPoint)
		{// 目標より大きい
			m_nCurrentPoint--;
			m_pMotion->SetMotion(MOTION_WALK);
			pSound->Play(CSound::LABEL_SE05);
		}
	}

	// 向く方向と進む方向
	m_rotDest.y = atan2f((POINT[m_nCurrentPoint].x - m_pos.x), (POINT[m_nCurrentPoint].z - m_pos.z)) + D3DX_PI;
	float fAngle = atan2f((POINT[m_nCurrentPoint].x - m_pos.x), (POINT[m_nCurrentPoint].z - m_pos.z));
	
	D3DXVECTOR3 move;
	// 位置反映
	move.x = sinf(fAngle) * fSpeed;
	move.y = 0.0f;
	move.z = cosf(fAngle) * fSpeed;
	m_pos += move;
	m_pFishingLine->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + fishingOffsetY, m_pos.z));
}

//******************
// 状態を変える
//******************
void CPlayer::ChangeState(CPlayerStateBase* pState)
{
	pState->SetOwner(this);
	m_Machine->ChangeState(pState);
}

//********************
// 条件による状態変更
//********************
void CPlayer::StateControll(void)
{
	if (m_nDestPoint == POINT_ORDER)
	{// オーダー
		COrderDeskState* pOrderDeskState = new COrderDeskState;
		ChangeState(pOrderDeskState);
	}
	else if (m_nDestPoint == POINT_PLAY)
	{// 遊ぶ場所
		CNormalState* pNormalState = new CNormalState;
		ChangeState(pNormalState);
	}
	else if (m_nDestPoint == POINT_BAGGING)
	{// 袋に入れるところ
		CBaggingState* pBaggingState = new CBaggingState;
		ChangeState(pBaggingState);
	}
}

bool CPlayer::GetIsFishingCancel(void)
{ 
	return m_pFishingLine->GetIsFishingCancel(); 
}

//***************************
// プレイヤーステートベース
//***************************
//***************************
// コンストラクタ
//***************************
CPlayerStateBase::CPlayerStateBase()
{
	m_pPlayer = NULL;
}

//***************************
// デストラクタ
//***************************
CPlayerStateBase::~CPlayerStateBase()
{

}

//********************************
// プレイヤーのポインタを設定する
//********************************
void CPlayerStateBase::SetOwner(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

//***************************
// 地面にいる状態
//***************************
//***************************
// コンストラクタ
//***************************
CNormalState::CNormalState()
{
	m_nCnt = 0;
}

//***************************
// デストラクタ
//***************************
CNormalState::~CNormalState()
{

}

//***************************
// 初期化処理
//***************************
void CNormalState::Init(void)
{
	m_pPlayer->SetState(CPlayer::STATE_NONE);
	CRStick::SetScreen(true);
}

//***************************
// 更新
//***************************
void CNormalState::Update(void)
{
	CCustomerManager* pCustomerManager = CGame::GetCCustomerManager();
	CFishingLine* pFishingLine = m_pPlayer->GetFishingLine();

	if (pFishingLine->GetHaveNum() > 0 && !pFishingLine->GetIsHookAnim())
	{
		for (int nCnt = 0; nCnt < nFishingHaveMax; nCnt++)
		{
			CItem* pItem = pFishingLine->GetItem(nCnt);
			if (pItem != nullptr)
			{
				pCustomerManager->Play(pItem);
				m_pPlayer->SetIsFishingItem(true);
			}
		}
		pFishingLine->Clear();
	}

	CMotion* pMotion = m_pPlayer->GetMotion();
	// シーンがチュートリアルかゲームの時だけ通す
	bool bScene = CGame::GetScene() == CGame::SCENE_TUTORIAL || CGame::GetScene() == CGame::SCENE_GAME;
	bool bInput = CManager::GetCInputKeyboard()->Trigger(DIK_RETURN) || CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_X);
	bool bFishing = bInput && !m_pPlayer->GetFishingLine()->GetIsHookAnim() && bScene;
	m_pPlayer->SetIsFishing(bFishing);
	if (bFishing)
	{// 音と釣りとモーション
		CSound* pSound = CManager::GetCSound();
		pSound->Play(CSound::LABEL_SE00);
		m_pPlayer->GetFishingLine()->MoveTrigger(CFishingLine::DIRTYPE_PLAY);
		pMotion->SetMotion(CPlayer::MOTION_FISHING);
	}
	else if (!m_pPlayer->GetFishingLine()->GetIsHookAnim())
	{
		pMotion->SetMotion(CPlayer::MOTION_NONE);
	}

	// 波を発生させる
	CMesh* pMesh = CGame::GetCMesh();
	bool bWave =	CManager::GetCInputMouse()->GetDown(CInputMouse::MOUSE_L) 
				||	CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A);
	pMesh->SetWave(bWave);
}

//***************************
// 終了処理
//***************************
void CNormalState::Uninit(void)
{

}

//***************************
// 注文机状態
//***************************
//***************************
// コンストラクタ
//***************************
COrderDeskState::COrderDeskState()
{
	m_nCnt = 0;
}

//***************************
// デストラクタ
//***************************
COrderDeskState::~COrderDeskState()
{

}

//***************************
// 初期化処理
//***************************
void COrderDeskState::Init(void)
{
	m_pPlayer->SetState(CPlayer::STATE_ORDER);
	CRStick::SetScreen(false);
}

//***************************
// 更新
//***************************
void COrderDeskState::Update(void)
{
	CCustomerManager* pCustomerManager = CGame::GetCCustomerManager();
	CFishingLine* pFishingLine = m_pPlayer->GetFishingLine();

	if (pFishingLine->GetHaveNum() > 0&& !pFishingLine->GetIsHookAnim())
	{
		for (int nCnt = 0; nCnt < nFishingHaveMax; nCnt++)
		{
			CItem* pItem = pFishingLine->GetItem(nCnt);
			if (pItem != nullptr)
			{
				pCustomerManager->Order(pItem);
				m_pPlayer->SetIsFishingItem(true);
			}
		}
		pFishingLine->Clear();
	}

	CMotion* pMotion = m_pPlayer->GetMotion();

	// シーンがチュートリアルかゲームの時だけ通す
	bool bScene = CGame::GetScene() == CGame::SCENE_TUTORIAL || CGame::GetScene() == CGame::SCENE_GAME;
	bool bInput = CManager::GetCInputKeyboard()->Trigger(DIK_RETURN) || CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_X);
	bool bFishing = bInput && !m_pPlayer->GetFishingLine()->GetIsHookAnim() && bScene;
	m_pPlayer->SetIsFishing(bFishing);
	if (bFishing)
	{
		// 音と釣りとモーション
		CSound* pSound = CManager::GetCSound();
		pSound->Play(CSound::LABEL_SE00);
		m_pPlayer->GetFishingLine()->MoveTrigger(CFishingLine::DIRTYPE_ORDER);
		pMotion->SetMotion(CPlayer::MOTION_FISHING);
	}
	else if (!m_pPlayer->GetFishingLine()->GetIsHookAnim())
	{
		pMotion->SetMotion(CPlayer::MOTION_NONE);
	}

}

//***************************
// 終了処理
//***************************
void COrderDeskState::Uninit(void)
{

}

//***************************
// 袋詰め状態
//***************************
//***************************
// コンストラクタ
//***************************
CBaggingState::CBaggingState()
{
}

//***************************
// デストラクタ
//***************************
CBaggingState::~CBaggingState()
{

}

//***************************
// 初期化処理
//***************************
void CBaggingState::Init(void)
{
	m_pPlayer->SetState(CPlayer::STATE_BAGGING);
	CRStick::SetScreen(false);
}

//***************************
// 更新
//***************************
void CBaggingState::Update(void)
{
	CCustomerManager* pCustomerManager = CGame::GetCCustomerManager();
	CFishingLine* pFishingLine = m_pPlayer->GetFishingLine();

	if (pFishingLine->GetHaveNum() > 0 && !pFishingLine->GetIsHookAnim())
	{
		for (int nCnt = 0; nCnt < nFishingHaveMax; nCnt++)
		{
			CItem* pItem = pFishingLine->GetItem(nCnt);
			if (pItem != nullptr)
			{
				pCustomerManager->Bagging(pItem);
				m_pPlayer->SetIsFishingItem(true);
			}

		}
		pFishingLine->Clear();
	}
	CMotion* pMotion = m_pPlayer->GetMotion();

	// シーンがチュートリアルかゲームの時だけ通す
	bool bScene = CGame::GetScene() == CGame::SCENE_TUTORIAL || CGame::GetScene() == CGame::SCENE_GAME;
	bool bInput = CManager::GetCInputKeyboard()->Trigger(DIK_RETURN) || CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_X);
	bool bFishing = bInput && !m_pPlayer->GetFishingLine()->GetIsHookAnim() && bScene;
	m_pPlayer->SetIsFishing(bFishing);
	if (bFishing)
	{
		// 音と釣りとモーション
		CSound* pSound = CManager::GetCSound();
		pSound->Play(CSound::LABEL_SE00);
		m_pPlayer->GetFishingLine()->MoveTrigger(CFishingLine::DIRTYPE_BAGGING);
		pMotion->SetMotion(CPlayer::MOTION_FISHING);
	}
	else if (!m_pPlayer->GetFishingLine()->GetIsHookAnim())
	{
		pMotion->SetMotion(CPlayer::MOTION_NONE);
	}

}

//***************************
// 終了処理
//***************************
void CBaggingState::Uninit(void)
{

}