//***************************************
//
// 客処理 [customer.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//*******************
// インクルード
//*******************
#include "customer.h"
#include "customerManager.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "order.h"
#include "debugproc.h"
#include "bowl.h"
#include "review.h"
#include "sound.h"
#include "angryEffect.h"
#include "loadMotion.h"
#include "loadMotionManager.h"

//**********************
// 静的メンバ変数初期化
//**********************
int CCustomer::m_nNumAll = 0;

// 定数
namespace OffsetPos
{
	const D3DXVECTOR3 ANGRYEFFECT = { -10.0f,15.0f,-10.0f };
	const D3DXVECTOR3 ORDER = { 0.0f,35.0f,15.0f };
}

//*******************
// コンストラクタ
//*******************
CCustomer::CCustomer(int nPriority) :CObject(nPriority)
{
	for (int nCnt = 0; nCnt < maxModelParts; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}
	m_pMotion = nullptr;
	m_pOrder = nullptr;
	m_pBowl = nullptr;
	m_Machine = nullptr;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_rotDest = VECTOR3_NULL;
	m_mtxWorld = {};
	for (int nCnt = 0; nCnt < nMaxOrderNum; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
		m_Order[nCnt].type = CItem::ITEM_SOFTPOI;
		m_Order[nCnt].bHave = false;
	}
	m_bCorrect = false;
	m_bOrder = false;
	m_bReceive = false;
	m_bBagging = false;
	m_bPlayReceive = false;
	m_bScoop = false;
	m_bScoopEnd = false;
	m_bQueue = false;
	m_bDeath = false;
	m_bPoiOut = false;
	m_grid.nX = 0;
	m_grid.nZ = 0;
	m_nScoopBrake = 0;
	m_nMaxFishCnt = 0;
	m_nAngryCnt = 0;
	m_UninitCnt = 0;
	m_nAngryEffectCnt = 0;
	m_nNumModel = 0;
	m_fReview = 0.0f;
	m_BaggingPos = VECTOR3_NULL;
	m_state = STATE_ORDER;
	m_Search = SEARCH_NULL0;
	m_nIdx = m_nNumAll;
	m_nNumAll++;
}

//*******************
// デストラクタ
//*******************
CCustomer::~CCustomer()
{
	m_nNumAll--;
}

//*******************
// インスタンス生成
//*******************
CCustomer* CCustomer::Create(D3DXVECTOR3 pos)
{
	// 自身のポインタ
	CCustomer* pCustomer=nullptr;

	// インスタンス生成
	pCustomer = new CCustomer;

	// 初期化
	pCustomer->SetPos(pos);
	pCustomer->Init();

	return pCustomer;

}

//*************
// 初期化処理
//*************
HRESULT CCustomer::Init(void)
{
	D3DXVECTOR3 pos = GetPos();
	m_pOrder = COrder::Create(D3DXVECTOR3(pos.x, pos.y + fOrderItemDistance, pos.z));

	m_Machine = new CStateMachine;
	COrderWaitState* pOrderState = new COrderWaitState;
	ChangeState(pOrderState);

	RandPoiType();
	m_Order[1].type = CItem::ITEM_BOWL;
	m_pOrder->SetItem(m_Order[0].type, m_Order[1].type);

	// モーションの読み込み
	int nMotionIdx = -1;
	nMotionIdx = CManager::GetLoadMotionManager()->Register("data\\MOTION\\motion_customer.txt");
	m_pMotion = CManager::GetLoadMotionManager()->GetAddres(nMotionIdx, &m_apModel[0], m_nNumModel);

	// 取ったら満足する魚の数
	m_nMaxFishCnt = rand() % nRandFishCnt + nMinFishCnt;
	m_fReview = fMaxReviewScore;

	return S_OK;
}

//*************
// 終了処理
//*************
void CCustomer::Uninit(void)
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
void CCustomer::Update(void)
{
	AngryCount();
	AngryEffect();

	m_Machine->Update();

	// ボウルの位置設定
	D3DXVECTOR3 bowlPos = m_pos;
	bowlPos.x -= sinf(m_rot.y + PI_HALF) * BowlOffsetXZ;
	bowlPos.z -= cosf(m_rot.y + PI_HALF) * BowlOffsetXZ;
	if (m_pBowl != nullptr)
	{
		m_pBowl->SetPos(bowlPos);
	}

	// 向きの更新
	UpdateRotDest();

	m_pOrder->SetPos(m_pos+OffsetPos::ORDER);
	ItemPos();
	m_pMotion->Update();

	// 終了処理
	if (m_UninitCnt > maxUninitCnt|| m_bDeath)
	{
		m_UninitCnt = 0;
		// ボウルと自身の終了
		if (m_pBowl != nullptr)
		{
			m_pBowl->Death();
			m_pBowl = nullptr;
		}
		m_pOrder->Death();
		Uninit();
	}
}

//*************
// 描画処理
//*************
void CCustomer::Draw(void)
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

//******************
// 状態を変える
//******************
void CCustomer::ChangeState(CCustomerStateBase* pState)
{
	pState->SetOwner(this);
	m_Machine->ChangeState(pState);
}

//*********************
// 向きの更新
//*********************
void CCustomer::UpdateRotDest(void)
{
	// 正規化処理
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= PI_DOUBLE;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{
		m_rotDest.y += PI_DOUBLE;
	}

	m_rot.y += (m_rotDest.y - m_rot.y) * fRotDestRatio;
}

//*********************
// アイテムの位置設定
//*********************
void CCustomer::ItemPos(void)
{
	for (int nCnt = 0; nCnt < nMaxItemNum; nCnt++)
	{
		if (m_pItem[nCnt] != nullptr)
		{
			D3DXVECTOR3 pos = m_pos;
			pos.x -= sinf(m_rot.y) * ItemOffsetXZ;
			pos.y += ItemOffsetY;
			pos.z -= cosf(m_rot.y) * ItemOffsetXZ;
			m_pItem[nCnt]->SetPos(pos);
		}
	}
}

//******************************
// オーダーを完了した瞬間の処理
//******************************
void CCustomer::InitPlay(void)
{
	switch (m_state)
	{
	case STATE_ORDER:
		ItemUninit();
		// 右手にポイをつける
		m_apModel[nItemModelIdx] = CModel::Create(VECTOR3_NULL, "data\\MODEL\\customer_poi.x");
		m_apModel[nItemModelIdx]->SetParent(m_apModel[nHandRIdx]);
		m_nNumModel++;
		// 受け取り完了
		m_bReceive = true;
		break;
	case STATE_PLAY:
		ItemUninit();
		// 右手にポイをつける
		m_apModel[nItemModelIdx] = CModel::Create(VECTOR3_NULL, "data\\MODEL\\customer_poi.x");
		m_apModel[nItemModelIdx]->SetParent(m_apModel[nHandRIdx]);
		m_nNumModel++;
		// 受け取り完了
		m_bPlayReceive = true;
		m_bPoiOut = false;
		break;
	case STATE_BAGGING:
		ItemUninit();
		// ボウルと自身の終了
		if (m_pBowl != nullptr)
		{
			m_pBowl->Death();
			m_pBowl = nullptr;
		}
		// 右手に袋をつける
		m_apModel[nItemModelIdx] = CModel::Create(VECTOR3_NULL, "data\\MODEL\\baggingS.x");
		m_apModel[nItemModelIdx]->SetParent(m_apModel[nHandRIdx]);
		m_nNumModel++;
		// 受け取り完了
		m_bBagging = true;

		Review();
		break;
	}

	for (int nCnt = 0; nCnt < nMaxItemNum; nCnt++)
	{// 受付でもらったアイテムを消す(大きさを変えるため)
		if (m_pItem[nCnt] != nullptr)
		{
			m_pItem[nCnt]->Uninit();
			m_pItem[nCnt] = nullptr;
		}
		m_Order[nCnt].bHave = false;
	}
}

//*************
// 注文の表示
//*************
void CCustomer::OrderScreen(bool bScreen)
{
	m_pOrder->SetScreen(bScreen);
}

//*************
// 受け取る
//*************
void CCustomer::Receive(CItem* pItem)
{
	bool fReceive = false;

	for (int nCnt = 0; nCnt < nMaxItemNum; nCnt++)
	{// アイテム枠二つを調べる
		if (m_Order[nCnt].type == pItem->GetType() && !m_Order[nCnt].bHave)
		{// 貰ったアイテムが欲しいやつだったら
			m_pItem[nCnt] = pItem;
			m_Order[nCnt].bHave = true;
			fReceive = true;
			return;
		}
	}

	if (!fReceive)
	{// アイテムの一致がなかったら
		D3DXVECTOR3 pos = GetPos();
		pos.x += fAngryEffectWidthOffset;
		pos.y += fAngryEffectHeightOffset;
		CAngryEffect::Create(pos, D3DXVECTOR2(fAngryEffectSize, fAngryEffectSize));
		m_nAngryCnt += nMistakeAngryCnt;
	}

	pItem->Uninit();
	pItem = nullptr;
}

//*****************
// 遊ぶ場所の探索
//*****************
void CCustomer::PlayPosSearch(void)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	CCustomerManager::PLAYAREA playArea = pCustomerM->GetPlayAreaGrid(m_grid.nX, m_grid.nZ);

	if (m_Search == SEARCH_STAY|| !IsNextGridUpdate()|| pCustomerM->GetIsFill())
	{// 条件でその場にとどまる
		m_posDest = playArea.pos;
		return;
	}

	// 遊ぶ場所の探索処理
	switch (m_Search)
	{
	case SEARCH_NULL0:
		Null0Update();
		m_pMotion->SetMotion(MOTION_MOVE);
		break;
	case SEARCH_NULL1:
		Null1Update();
		m_pMotion->SetMotion(MOTION_MOVE);
		break;
	case SEARCH_NONE:
		NoneUpdate();
		break;
	}
}

//*************************
// 注文受け取って最初のマス
//*************************
void CCustomer::Null0Update(void)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	CCustomerManager::PLAYAREA playArea = pCustomerM->GetPlayAreaGrid(m_grid.nX, m_grid.nZ + 1);

	if (playArea.type == CCustomerManager::PLAYAREA_TYPE_NONE)
	{// 空いてたらそこに座る
		m_Search = SEARCH_STAY;
		m_grid.nZ++;
		pCustomerM->SetPlayAreaGrid(m_grid.nX, m_grid.nZ, CCustomerManager::PLAYAREA_TYPE_FILL);
		m_pBowl = CBowl::Create(m_pos);
	}
	else if (playArea.type == CCustomerManager::PLAYAREA_TYPE_FILL)
	{// 埋まってたら次のマスを探す
		m_Search = SEARCH_NULL1;
		m_grid.nX++;
		m_grid.nZ++;
	}
}

//*************************
// 探索エリアのマス
//*************************
void CCustomer::Null1Update(void)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	CCustomerManager::PLAYAREA playArea = pCustomerM->GetPlayAreaGrid(m_grid.nX - 1, m_grid.nZ);

	if (playArea.type == CCustomerManager::PLAYAREA_TYPE_NONE)
	{// 空いてたら探す状態を変える
		m_Search = SEARCH_NONE;
	}
	else if (playArea.type == CCustomerManager::PLAYAREA_TYPE_FILL)
	{// 埋まってたら次の場所を探す
		m_Search = SEARCH_NULL1;
		m_grid.nZ++;
	}
}

//*****************************
// 空いてるところを見つけた時
//*****************************
void CCustomer::NoneUpdate(void)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	int nZ = m_grid.nZ + 1;
	if (m_grid.nZ + 1 > nMaxSeachGridZ)
	{
		nZ = m_grid.nZ;
	}
	CCustomerManager::PLAYAREA playArea = pCustomerM->GetPlayAreaGrid(m_grid.nX - 1, nZ);

	if (playArea.type == CCustomerManager::PLAYAREA_TYPE_NONE)
	{// 隣が空いてたらそこに座る
		m_Search = SEARCH_STAY;
		m_grid.nX--;
		if (m_grid.nZ + 1 <= nMaxSeachGridZ)
		{
			m_grid.nZ++;
		}
		pCustomerM->SetPlayAreaGrid(m_grid.nX, m_grid.nZ, CCustomerManager::PLAYAREA_TYPE_FILL);
		m_pBowl = CBowl::Create(m_pos);
	}
	else if (playArea.type == CCustomerManager::PLAYAREA_TYPE_FILL)
	{// 隣が埋まっていたら前回見ていたところに座る
		m_Search = SEARCH_STAY;
		m_grid.nX--;
		pCustomerM->SetPlayAreaGrid(m_grid.nX, m_grid.nZ, CCustomerManager::PLAYAREA_TYPE_FILL);
		m_pBowl = CBowl::Create(m_pos);
	}
}

//*****************************
// 1マスずつ進む処理
//*****************************
bool CCustomer::IsNextGridUpdate(void)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	CCustomerManager::PLAYAREA playArea = pCustomerM->GetPlayAreaGrid(m_grid.nX, m_grid.nZ);

	// 距離を求める
	float fDistX = m_pos.x - playArea.pos.x;
	float fDistZ = m_pos.z - playArea.pos.z;
	float fDistance = sqrtf((fDistX * fDistX) + (fDistZ * fDistZ));
	if (fDistance < fIsNextRadius)
	{// 範囲内だったら
		return true;
	}

	return false;
}

//********************
// 金魚をすくう処理
//********************
bool CCustomer::ScoopFish(D3DXVECTOR3 pos)
{
	if (!m_bScoop)return false;

	D3DXVECTOR3 playerPos = m_pos;
	playerPos.x -= sinf(m_rot.y) * fScoopDist;
	playerPos.z -= cosf(m_rot.y) * fScoopDist;

	// 距離を求める
	float fDistX = pos.x - playerPos.x;
	float fDistZ = pos.z - playerPos.z;
	float fDistance = sqrtf((fDistX * fDistX) + (fDistZ * fDistZ));

	const float fRadius = fScoopRadius;

	if (fDistance < fRadius)
	{// 範囲内だったら
		return true;
	}

	return false;
}

//****************************
// 袋に入れるところに向かう
//****************************
void CCustomer::GoBagging(void)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	CCustomerManager::PLAYAREA playArea = pCustomerM->GetPlayAreaGrid(m_grid.nX, m_grid.nZ);

	m_posDest = playArea.pos;

	if (!IsNextGridUpdate()&& !m_bQueue)
	{
		// 列の場所まで歩く
		m_pMotion->SetMotion(MOTION_MOVE);
		float fAngle = atan2f((m_posDest.x - m_pos.x), (m_posDest.z - m_pos.z));

		D3DXVECTOR3 move;
		move.x = sinf(fAngle) * fSpeed;
		move.y = 0.0f;
		move.z = cosf(fAngle) * fSpeed;
		m_pos += move;
		m_rotDest.y = fAngle + D3DX_PI;
	}
	else if (m_bQueue)
	{
		// 列に並ぶ
		m_pMotion->SetMotion(MOTION_NONE);
		m_pos += (m_BaggingPos - m_pos) * fBaggingPosRatio;
		m_rotDest.y = PI_HALF;
	}

	// 決められたグリッドまで行ったら並ばせる
	if (IsNextGridUpdate())
	{
		m_grid.nZ++;
		if (m_grid.nZ >= nMaxGridZ)
		{
			m_bQueue = true;
		}
	}
}

//************************************
// 怒るまでのカウントをするタイミング
//************************************
void CCustomer::AngryCount(void)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();

	// 状態ごとの怒る条件
	switch (m_state)
	{
	case STATE_ORDER:
		// 埋まっていないのに待たされたら
		if (!pCustomerM->GetIsFill())
		{
			m_nAngryCnt++;
		}
		break;
	case STATE_PLAY:
		// 埋まってるのに通されたら
		if (pCustomerM->GetIsFill()&&m_Search==SEARCH_NULL0)
		{
			m_nAngryCnt++;
		}
		break;
	case STATE_PLAYORDER:
		// 追加注文で待たされたら
		m_nAngryCnt++;
		break;

	case STATE_BAGGING:
		// 袋詰めで待たされたら
		m_nAngryCnt += nBaggingAngryCnt;
		break;
	}
}

//******************
// 評価
//******************
void CCustomer::Review(void)
{
	if (m_nAngryCnt > angryLine0)
	{// 最低評価(固定点式)
		m_fReview = fReview0;
	}
	else if (m_nAngryCnt > angryLine1)
	{// 低評価(固定式)
		m_fReview = fReview1;
	}
	else if (m_nAngryCnt > angryLine2)
	{// 微高評価(減点式)
		m_fReview -= fReview2;
	}
	else if (m_nAngryCnt > angryLine3)
	{// 高評価(減点式)
		m_fReview -= fReview3;
	}

	// 評価を書く
	CReview::Write(m_fReview);
}

//******************
// 怒りマークを出す
//******************
void CCustomer::AngryEffect(void)
{
	// 一定時間ずつ出す
	m_nAngryEffectCnt++;
	if (m_nAngryEffectCnt < AngryEffectCool)
	{
		return;
	}

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 effectPos = VECTOR3_NULL;
	const D3DXVECTOR2 size = D3DXVECTOR2(fAngryEffecrSize, fAngryEffecrSize);
	if (m_nAngryCnt > angryLine0)
	{// かなり怒り
		for (int nCnt = 0; nCnt < nAngryEffectNum; nCnt++)
		{
			effectPos.x = pos.x + (float)(rand() % AngryEffectRandXZ) + OffsetPos::ANGRYEFFECT.x;
			effectPos.y = pos.y + (float)(rand() % AngryEffectRandY) + OffsetPos::ANGRYEFFECT.y;
			effectPos.z = pos.z + (float)(rand() % AngryEffectRandXZ) + OffsetPos::ANGRYEFFECT.z;

			CAngryEffect::Create(effectPos, size);
		}
	}
	else if (m_nAngryCnt > angryLine1)
	{// ちょい怒り
		effectPos.x = pos.x + (float)(rand() % AngryEffectRandXZ) + OffsetPos::ANGRYEFFECT.x;
		effectPos.y = pos.y + (float)(rand() % AngryEffectRandY) + OffsetPos::ANGRYEFFECT.y;
		effectPos.z = pos.z + (float)(rand() % AngryEffectRandXZ) + OffsetPos::ANGRYEFFECT.z;

		CAngryEffect::Create(effectPos, size);
	}

	m_nAngryEffectCnt = 0;
}

//************************
// ポイが破れたときの処理
//************************
void CCustomer::PoiOut(void)
{
	ItemUninit();

	// 破れたポイに切り替える
	m_apModel[nItemModelIdx] = CModel::Create(VECTOR3_NULL, "data\\MODEL\\customer_poiOut.x");
	m_apModel[nItemModelIdx]->SetParent(m_apModel[nHandRIdx]);
	m_nNumModel++;
}

//************************
// ポイの硬さを決める処理
//************************
void CCustomer::RandPoiType(void)
{
	// ランダムでほしいポイを決める
	int nPoiType = rand() % (CItem::ITEM_HARDPOI + 1);
	switch (nPoiType)
	{
	case CItem::ITEM_SOFTPOI:
		m_Order[0].type = CItem::ITEM_SOFTPOI;
		break;
	case CItem::ITEM_MIDIUMPOI:
		m_Order[0].type = CItem::ITEM_MIDIUMPOI;
		break;
	case CItem::ITEM_HARDPOI:
		m_Order[0].type = CItem::ITEM_HARDPOI;
		break;
	}
}

//***************************
// 袋詰めのための初期化処理
//***************************
void CCustomer::InitBagging(void)
{
	// 自分のグリッドを空ける
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	pCustomerM->SetPlayAreaGrid(m_grid.nX, m_grid.nZ, CCustomerManager::PLAYAREA_TYPE_NONE);

	// 状態変える
	m_state = STATE_BAGGING;

	if (m_grid.nZ < nBaggingDestGridZ0)
	{
		m_grid.nX = nBaggingDestX0;
		m_grid.nZ = nBaggingDestZ0;
	}
	else if (m_grid.nZ < nBaggingDestGridZ1)
	{
		m_grid.nX = nBaggingDestX1;
		m_grid.nZ = nBaggingDestZ1;
	}
	else
	{
		m_bQueue = true;
	}

	// 魚の取れた数で袋の大きさを決める
	if (m_pBowl->GetFishCnt() > baggingLOrderNum)
	{
		m_Order[0].type = CItem::ITEM_BAG_L;
	}
	else
	{
		m_Order[0].type = CItem::ITEM_BAG_S;
	}
	m_Order[1].type = CItem::ITEM_MAX;
	m_pOrder->SetItem(m_Order[0].type, m_Order[1].type);
}

//************************
// 帰るか新しく注文するか
//************************
void CCustomer::InitPlayOrder(void)
{
	m_state = STATE_PLAYORDER;

	RandPoiType();
	m_Order[1].type = CItem::ITEM_MAX;
	m_pOrder->SetItem(m_Order[0].type, m_Order[1].type);
}

//************************
// 帰るか新しく注文するか
//************************
bool CCustomer::IsPlay(void)
{
	// 金魚が0匹または満足分に達してないかつ確率
	bool bFish = m_pBowl->GetFishCnt() <= 0&& !m_bPlayReceive;
	bool bMaxFish = m_pBowl->GetFishCnt()< m_nMaxFishCnt;
	bool bRand = rand() % nMaxReplayRand <= nReplayRand;
	bRand = bRand && bMaxFish&& !m_bPlayReceive;

	return bFish || bRand;
}

//*****************************
// そのグリッドの位置にいるか
//*****************************
bool CCustomer::GetIsPlayArea(int nX, int nZ)
{
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	CCustomerManager::PLAYAREA playArea = pCustomerM->GetPlayAreaGrid(nX, nZ);

	// 距離を求める
	float fDistX = m_pos.x - playArea.pos.x;
	float fDistZ = m_pos.z - playArea.pos.z;
	float fDistance = sqrtf((fDistX * fDistX) + (fDistZ * fDistZ));

	const float fRadius = fIsPlayRadius;

	if (fDistance < fRadius)
	{// 範囲
		return true;
	}

	return false;

}

//************************
// 目標の位置に向かう
//************************
void CCustomer::PosDestMove(void)
{
	m_pos += (m_posDest - m_pos) * fDestPosRatio;
}

//*******************************
// 手に持っているアイテムを消す
//*******************************
void CCustomer::ItemUninit(void)
{
	if (m_apModel[nItemModelIdx] != nullptr)
	{
		m_apModel[nItemModelIdx]->Uninit();
		delete m_apModel[nItemModelIdx];
		m_apModel[nItemModelIdx] = nullptr;
		m_nNumModel--;
	}
}