//***************************************
//
// 客の状態処理 [customerState.cpp]
// Author:Sasaki Soichiro
//
//***************************************

//*******************
// インクルード
//*******************
#include "customerState.h"
#include "sound.h"
#include "manager.h"
#include "bowl.h"
#include "customer.h"

//***************************
// コンストラクタ
//***************************
CCustomerStateBase::CCustomerStateBase()
{
	m_pCustomer = NULL;
}

//***************************
// デストラクタ
//***************************
CCustomerStateBase::~CCustomerStateBase()
{

}

//********************************
// プレイヤーのポインタを設定する
//********************************
void CCustomerStateBase::SetOwner(CCustomer* pCustomer)
{
	m_pCustomer = pCustomer;
}

//******************************************************
// 注文机にいる状態
//******************************************************
//***************************
// コンストラクタ
//***************************
COrderWaitState::COrderWaitState()
{
}

//***************************
// デストラクタ
//***************************
COrderWaitState::~COrderWaitState()
{

}

//***************************
// 初期化処理
//***************************
void COrderWaitState::Init(void)
{

}

//***************************
// 更新
//***************************
void COrderWaitState::Update(void)
{
	// 持っているアイテムを調べる
	int nReceiveCnt = 0;
	for (int nCnt = 0; nCnt < nMaxItem; nCnt++)
	{
		if (m_pCustomer->GetOrder(nCnt).bHave)
		{
			nReceiveCnt++;
		}
	}

	// アイテムが二つそろった処理
	if (nReceiveCnt >= nPassCnt && !m_pCustomer->GetReceive())
	{
		m_pCustomer->InitPlay();

		CPlaySeachState* pPlaySeachState = new CPlaySeachState;
		m_pCustomer->ChangeState(pPlaySeachState);
		return;
	}

	m_pCustomer->PosDestMove();
}

//***************************
// 終了処理
//***************************
void COrderWaitState::Uninit(void)
{

}

//******************************************************
// 遊べる場所を探している状態
//******************************************************
//***************************
// コンストラクタ
//***************************
CPlaySeachState::CPlaySeachState()
{
}

//***************************
// デストラクタ
//***************************
CPlaySeachState::~CPlaySeachState()
{

}

//***************************
// 初期化処理
//***************************
void CPlaySeachState::Init(void)
{
}

//***************************
// 更新
//***************************
void CPlaySeachState::Update(void)
{
	D3DXVECTOR3 pos = m_pCustomer->GetPos();
	D3DXVECTOR3 posDest = m_pCustomer->GetPosDest();
	D3DXVECTOR3 rotDest = m_pCustomer->GetRotDest();

	if (!m_pCustomer->IsNextGridUpdate())
	{
		// 目標の位置に向かわせる
		float fAngle = atan2f((posDest.x - pos.x), (posDest.z - pos.z));
		D3DXVECTOR3 move;
		move.x = sinf(fAngle) * fSpeed;
		move.y = 0.0f;
		move.z = cosf(fAngle) * fSpeed;
		pos += move;
		rotDest.y = fAngle + D3DX_PI;
		m_pCustomer->SetPos(pos);
		m_pCustomer->SetRotDest(rotDest);
	}

	else if (m_pCustomer->GetSearch() == CCustomer::SEARCH_STAY)
	{
		// 遊ぶ場所についたら状態を変える
		CPlayState* pPlayState = new CPlayState;
		m_pCustomer->ChangeState(pPlayState);
		return;
	}

	m_pCustomer->PlayPosSearch();

}

//***************************
// 終了処理
//***************************
void CPlaySeachState::Uninit(void)
{

}

//******************************************************
// 遊んでいる状態
//******************************************************
//***************************
// コンストラクタ
//***************************
CPlayState::CPlayState()
{
	m_bScoopEnd = false;
	m_nScoopBrake = 0;
}

//***************************
// デストラクタ
//***************************
CPlayState::~CPlayState()
{

}

//***************************
// 初期化処理
//***************************
void CPlayState::Init(void)
{
}

//***************************
// 更新
//***************************
void CPlayState::Update(void)
{
	if (!m_bScoopEnd)
	{
		D3DXVECTOR3 pos = m_pCustomer->GetPos();
		D3DXVECTOR3 rotDest = m_pCustomer->GetRotDest();
		// 中央を向かせる
		float fAngle = atan2f((0.0f - pos.x), (0.0f - pos.z));
		rotDest.y = fAngle + D3DX_PI;
		m_pCustomer->SetRotDest(rotDest);

		// すくう処理
		Scoop();
	}
	else
	{// 満足したかポイが破れたら
		ScoopEnd();
	}
}

//***************************
// 終了処理
//***************************
void CPlayState::Uninit(void)
{

}

//***************************
// 金魚をすくう一連の処理
//***************************
void CPlayState::Scoop(void)
{
	if (m_nScoopBrake > nAnimStep2)
	{
		// すくい終わり
		m_pCustomer->SetScoop(false);
		m_nScoopBrake = 0;
	}
	else if (m_nScoopBrake > nAnimStep1 && !m_pCustomer->GetScoop())
	{
		// 金魚をすくう
		m_pCustomer->SetScoop(true);
		CSound* pSound = CManager::GetCSound();
		pSound->Play(CSound::LABEL_SE02);
	}
	else if (m_nScoopBrake > nAnimStep0)
	{
		// すくい始める
		m_pCustomer->SetMotion(CCustomer::MOTION_PLAY);
	}
	else
	{
		// タイミングを待っている
		m_pCustomer->SetMotion(CCustomer::MOTION_PLAYNUETORAL);
	}
	m_nScoopBrake++;

	if (m_pCustomer->GetBowl()->GetFishCnt() >= m_pCustomer->GetMaxFishCnt())
	{// 満足分すくった
		m_pCustomer->SetScoop(false);
		m_bScoopEnd = true;
		m_nScoopBrake = 0;
		m_pCustomer->SetMotion(CCustomer::MOTION_WIN);
	}
	else if (m_pCustomer->GetPoiOut())
	{
		m_pCustomer->SetScoop(false);
		m_bScoopEnd = true;
		m_nScoopBrake = 0;
		m_pCustomer->SetMotion(CCustomer::MOTION_LOSE);
		m_pCustomer->PoiOut();
	}
}

//***************************
// すくい終わりの処理
//***************************
void CPlayState::ScoopEnd(void)
{
	if (m_nScoopBrake > nScoopEndSwicth)
	{
		Switch();
		return;
	}
	m_nScoopBrake++;
}

//************************************
// 再度プレイするか袋詰めするかの処理
//************************************
void CPlayState::Switch(void)
{
	if (m_pCustomer->IsPlay())
	{// 再度遊ぶ
		m_pCustomer->InitPlayOrder();
		m_pCustomer->SetState(CCustomer::STATE_PLAYORDER);
		CPlayOrderState* pPlayOrderState = new CPlayOrderState;
		m_pCustomer->ChangeState(pPlayOrderState);
	}
	else
	{// 袋詰めに行く
		m_pCustomer->InitBagging();

		CBaggingWaitState* pBaggingState = new CBaggingWaitState;
		m_pCustomer->ChangeState(pBaggingState);
	}
}

//******************************************************
// 追加注文をしている状態
//******************************************************
//***************************
// コンストラクタ
//***************************
CPlayOrderState::CPlayOrderState()
{
}

//***************************
// デストラクタ
//***************************
CPlayOrderState::~CPlayOrderState()
{

}

//***************************
// 初期化処理
//***************************
void CPlayOrderState::Init(void)
{
}

//***************************
// 更新
//***************************
void CPlayOrderState::Update(void)
{
	// 持っているアイテムを調べる
	int nReceiveCnt = 0;
	for (int nCnt = 0; nCnt < nMaxItem; nCnt++)
	{
		if (m_pCustomer->GetOrder(nCnt).bHave)
		{
			nReceiveCnt++;
		}
	}

	// アイテムが二つそろった処理
	if (nReceiveCnt >= nPassCnt && !m_pCustomer->GetPlayReceive())
	{
		m_pCustomer->SetState(CCustomer::STATE_PLAY);
		m_pCustomer->InitPlay();
		CPlayState* pPlayState = new CPlayState;
		m_pCustomer->ChangeState(pPlayState);
		return;
	}
}

//***************************
// 終了処理
//***************************
void CPlayOrderState::Uninit(void)
{
}

//******************************************************
// 袋詰め状態
//******************************************************
//***************************
// コンストラクタ
//***************************
CBaggingWaitState::CBaggingWaitState()
{
}

//***************************
// デストラクタ
//***************************
CBaggingWaitState::~CBaggingWaitState()
{

}

//***************************
// 初期化処理
//***************************
void CBaggingWaitState::Init(void)
{
}

//***************************
// 更新
//***************************
void CBaggingWaitState::Update(void)
{
	m_pCustomer->GoBagging();

	// 持っているアイテムを調べる
	int nReceiveCnt = 0;
	for (int nCnt = 0; nCnt < nMaxItem; nCnt++)
	{
		if (m_pCustomer->GetOrder(nCnt).bHave)
		{
			nReceiveCnt++;
		}
	}

	// アイテムが二つそろった処理
	if (nReceiveCnt >= nPassCnt && !m_pCustomer->GetBagging())
	{
		m_pCustomer->InitPlay();
		CGoBackState* pGoBackState = new CGoBackState;
		m_pCustomer->ChangeState(pGoBackState);
		return;
	}
}

//***************************
// 終了処理
//***************************
void CBaggingWaitState::Uninit(void)
{
}

//******************************************************
// 帰る状態
//******************************************************
//***************************
// コンストラクタ
//***************************
CGoBackState::CGoBackState()
{
}

//***************************
// デストラクタ
//***************************
CGoBackState::~CGoBackState()
{

}

//***************************
// 初期化処理
//***************************
void CGoBackState::Init(void)
{
}

//***************************
// 更新
//***************************
void CGoBackState::Update(void)
{
	// 帰る
	D3DXVECTOR3 pos = m_pCustomer->GetPos();
	pos.x += moveX;
	pos.z += moveZ;
	m_pCustomer->SetPos(pos);
	// 向きを合わせる
	D3DXVECTOR3 rotDest = m_pCustomer->GetRotDest();
	float fAngle = atan2f((moveX), (moveZ));
	rotDest.y = fAngle + D3DX_PI;
	m_pCustomer->SetRotDest(rotDest);

	m_pCustomer->SetMotion(CCustomer::MOTION_MOVE);

	// 一定時間で消す貯めのカウント
	int nUninitCnt = m_pCustomer->GetUninitCnt();
	nUninitCnt++;
	m_pCustomer->SetUninitCnt(nUninitCnt);
}

//***************************
// 終了処理
//***************************
void CGoBackState::Uninit(void)
{
}