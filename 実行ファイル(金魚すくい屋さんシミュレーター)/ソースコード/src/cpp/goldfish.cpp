//*****************************************
//
// 金魚処理 [goldfish.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "goldfish.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "mesh.h"
#include <time.h>
#include "bowl.h"
#include "customerManager.h"
#include "customer.h"
#include "mathUtil.h"

// 定数
namespace Diffuse
{
	D3DXCOLOR BLACK = { 0.0f,0.0f,0.0f,1.0f };
	D3DXCOLOR RED = { 1.0f,0.2f,0.2f,1.0f };
}

//*****************
// コンストラクタ
//*****************
CGoldFish::CGoldFish(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	m_State = STATE_AQUARIUM;
	m_fAngle = 0.0f;
	m_fAngleDest = 0.0f;
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpeed = 0.0f;
	m_nFeelingCnt = 0;
	m_nChangeFeelingCnt = 0;
	m_feeling = 0;
	m_nColorType = 0;
	m_escape = 0;
	m_bEscape = false;
	m_nEscapeCnt = 0;
	m_pBowl = nullptr;
	m_bDeath = false;
	m_fOffsetBowl = 0.0f;
}

//*****************
// デストラクタ
//*****************
CGoldFish::~CGoldFish()
{
}

//******************
// インスタンス生成
//*******************
CGoldFish* CGoldFish::Create(D3DXVECTOR3 pos,int nColorType)
{
	CGoldFish* pFish;

	// インスタンス生成
	pFish = new CGoldFish;
	// モデルの設定
	pFish->SetXModel("data\\MODEL\\GoldFish.x");
	pFish->m_nColorType = nColorType;
	// 初期化
	pFish->SetPos(pos);
	pFish->Init();

	return pFish;
}

//*****************
// 初期化処理
//*****************
HRESULT CGoldFish::Init(void)
{
	CObjectX::Init();
	m_nChangeFeelingCnt = nFeelCool;
	m_feeling = rand() % FEELING_MAX;
	m_fSpeed = fSpeed;
	m_fOffsetBowl = (float)(rand() % nRandBowlOffset) - MathUtil::Half<float>(nRandBowlOffset);
	CObjectX::SetShadowScreen(false);
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CGoldFish::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CGoldFish::Update(void)
{
	// 取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	m_posOld = pos;

	// 位置と向きの設定
	pos.x += sinf(m_fAngle) * m_fSpeed;
	pos.z += cosf(m_fAngle) * m_fSpeed;

	// 向き
	if (m_fAngle >PI_DOUBLE)
	{
		m_fAngle = 0.0f;
	}
	else if (m_fAngle < 0.0f)
	{
		m_fAngle = PI_DOUBLE;
	}
	rot.y = m_fAngle;
	// 反映
	SetPos(pos);
	SetRot(rot);

	Feeling();
	if(m_bEscape)
	{
		Escape();
	}

	if (m_bDeath)
	{
		Uninit();
	}
}

//*****************
// 描画処理
//*****************
void CGoldFish::Draw(void)
{
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ
	CXModel::XMODEL XModel = GetXModel();

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)XModel.pBuffMat->GetBufferPointer();

	if (m_nColorType == 1)
	{
		pMat->MatD3D.Diffuse = Diffuse::BLACK;
	}
	else
	{
		pMat->MatD3D.Diffuse = Diffuse::RED;
	}

	CObjectX::Draw();
}

//**********************
// 水槽との当たり判定
//**********************
bool CGoldFish::AquariumCollision(D3DXVECTOR3 pos, D3DXVECTOR3 posOld)
{
	D3DXVECTOR3 AquariumPos = CGame::GetCMesh()->GetPos();
	D3DXVECTOR2 AquariumSize = CGame::GetCMesh()->GetSize();

	// XZ平面の当たり判定
	if (AquariumPos.x + AquariumSize.x <= pos.x + fFishSize && AquariumPos.x + AquariumSize.x > posOld.x + fFishSize
		|| AquariumPos.x - AquariumSize.x >= pos.x - fFishSize && AquariumPos.x - AquariumSize.x < posOld.x - fFishSize
		|| AquariumPos.z + AquariumSize.y <= pos.z + fFishSize && AquariumPos.z + AquariumSize.y > posOld.z + fFishSize
		|| AquariumPos.z - AquariumSize.y >= pos.z - fFishSize && AquariumPos.z - AquariumSize.y < posOld.z - fFishSize)
	{
		return true;
	}

	return false;
}

//******************
// 気分を決める
//******************
void CGoldFish::Feeling(void)
{
	D3DXVECTOR3 pos = GetPos();
	IsEscape();

	bool bHit=false;
	switch (m_State)
	{
	case STATE_AQUARIUM:
		// 水槽
		bHit = AquariumCollision(GetPos(), m_posOld);
		Scoop();
		break;
	case STATE_BOWL:
		// ボウル
		if (m_pBowl != nullptr)
		{
			m_feeling = FEELING_STOP;
			D3DXVECTOR3 pos = GetPos();
			D3DXVECTOR3 bowlPos = m_pBowl->GetPos();
			pos = D3DXVECTOR3(bowlPos.x + m_fOffsetBowl, bowlPos.y + fBowlHeightOffset, bowlPos.z + m_fOffsetBowl);
			SetPos(pos);
			if (m_pBowl->GetDeath())
			{
				m_State = STATE_AQUARIUM;
				SetPos(D3DXVECTOR3(0.0f, fHeight, 0.0f));
				m_posOld = D3DXVECTOR3(0.0f, fHeight, 0.0f);
				m_pBowl = nullptr;
			}
		}
		break;
	}

	if (bHit)
	{
		srand((unsigned)time(NULL));
		pos = m_posOld;
		SetPos(pos);
		m_feeling = rand() % nRandFeeling + nMinFeeling;
		m_nFeelingCnt = 0;
		m_escape = false;
	}


	if (m_bEscape)return;

	switch (m_feeling)
	{
	case FEELING_STRAIGHT:
		m_fSpeed = fSpeed;
		break;
	case FEELING_FULLSPEED:
		m_fSpeed = fMaxSpeed;
		break;
	case FEELING_RIGHT:
		m_fAngle += fRotDelta;
		break;
	case FEELING_LEFT:
		m_fAngle -= fRotDelta;
		break;
	case FEELING_STOP:
		m_fSpeed -= fStopSpped;
		if (m_fSpeed < 0.0f)m_fSpeed = 0.0f;
		break;
	case FEELING_TURN:
		m_fAngle += D3DX_PI;
		m_feeling = FEELING_STRAIGHT;
		m_nFeelingCnt = 0;
		break;
	}

	m_nFeelingCnt++;

	if (m_nFeelingCnt > m_nChangeFeelingCnt)
	{
		m_nChangeFeelingCnt = rand() % nMaxChangeFeelRand + nChangeFeelRand;
		m_feeling = rand() % FEELING_MAX;
		m_nFeelingCnt = 0;
	}
}

//******************
// 逃げる判定
//******************
void CGoldFish::IsEscape(void)
{
	CMesh* pMesh = CGame::GetCMesh();
	bool bWave = pMesh->GetWaveTrigger();
	if (bWave)
	{
		D3DXVECTOR3 wavePos = pMesh->CursorCollision();
		D3DXVECTOR3 meshPos = pMesh->GetPos();
		D3DXVECTOR2 meshSize = pMesh->GetSize();
		D3DXVECTOR3 pos = GetPos();

		// メッシュの上じゃ無かったら返す
		if (pos.x > meshPos.x + meshSize.x || pos.x < meshPos.x - meshSize.x
			|| pos.z > meshPos.z + meshSize.y || pos.z < meshPos.z - meshSize.y)
		{
			return;
		}

		// 現在の距離計算
		float DistX, DistZ;
		DistX = wavePos.x - pos.x;
		DistZ = wavePos.z - pos.z;
		float Dist = sqrtf((DistX * DistX)  + (DistZ * DistZ));
		const float range = fIsEscapeRange;
		if (Dist < range)
		{
			m_fAngleDest = (atan2f(DistX, DistZ) + D3DX_PI);
			m_bEscape = true;
			m_nEscapeCnt = 0;
			m_escape = rand() % ESCAPE_MAX;
		}
	}
}

//******************
// 逃げる
//******************
void CGoldFish::Escape(void)
{
	switch (m_escape)
	{
	case ESCAPE_RIGHT:
		m_fAngle += (m_fAngleDest - m_fAngle) * fAngleRatio;
		m_fSpeed = fMaxSpeed;
		break;
	case ESCAPE_LEFT:
		m_fAngle -= (m_fAngle -m_fAngleDest) * fAngleRatio;
		m_fSpeed = fMaxSpeed;
		break;
	}

	m_nEscapeCnt++;
	if (m_nEscapeCnt > nEscapeCnt)
	{
		m_nEscapeCnt = 0;
		m_bEscape = false;
	}
}

//******************
// すくわれる処理
//******************
void CGoldFish::Scoop(void)
{
	if (!ScoopArea()) return;

	D3DXVECTOR3 pos = GetPos();
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();

	std::vector<CCustomer*> playCustomer = pCustomerM->GetPlayCustomer();

	for (int nCnt = 0; nCnt <(int)playCustomer.size(); nCnt++)
	{
		if (playCustomer[nCnt]->ScoopFish(pos))
		{
			ScoopRand(nCnt);
			return;
		}
	}
}

//******************
// すくわれるエリア
//******************
bool CGoldFish::ScoopArea(void)
{
	CMesh* pMesh = CGame::GetCMesh();
	D3DXVECTOR3 meshPos = pMesh->GetPos();
	D3DXVECTOR2 meshSize = pMesh->GetSize();
	D3DXVECTOR3 pos = GetPos();

	// メッシュの上じゃ無かったら返す
	if (pos.x > meshPos.x + meshSize.x- fScoopAreaOffset || pos.x < meshPos.x - meshSize.x+ fScoopAreaOffset
		|| pos.z > meshPos.z + meshSize.y- fScoopAreaOffset || pos.z < meshPos.z - meshSize.y + fScoopAreaOffset)
	{
		return true;
	}

	return false;
}

//************************
// 一定の確率でポイを破る
//************************
void CGoldFish::ScoopRand(int nCnt)
{
	int nRand = rand() % nPoiRand;
	CCustomerManager* pCustomerM = CGame::GetCCustomerManager();
	std::vector<CCustomer*> playCustomer = pCustomerM->GetPlayCustomer();

	if (nRand < nPoiOutRand)
	{// すくうことに成功
		m_pBowl = playCustomer[nCnt]->GetBowl();
		m_State = STATE_BOWL;
		m_pBowl->AddFish();
		D3DXVECTOR3 bowlPos = m_pBowl->GetPos();
		bowlPos.x += (float)(rand() % (int)fBowlDistance) - MathUtil::Half(fBowlDistance);
		bowlPos.y += fBowlHeight;
		bowlPos.z += (float)(rand() % (int)fBowlDistance) - MathUtil::Half(fBowlDistance);
		SetPos(bowlPos);
	}
	else
	{// ポイが破れた
		playCustomer[nCnt]->SetPoiOut(true);
	}
}

//******************
// 位置の設定
//******************
void CGoldFish::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CGoldFish::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CGoldFish::SetXModel(const char* fileName)
{
	// CXModelのポインタに情報を入れる
	CXModel* pXModel = CManager::GetCXModel();
	// 使うモデルのインデックスを入れる
	m_nIdxXFile = pXModel->Register(fileName);
	// Xファイルを割り当てる
	BindXFile(pXModel->GetAddres(m_nIdxXFile));
}

//******************
// 位置情報の取得
//******************
D3DXVECTOR3 CGoldFish::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CGoldFish::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CGoldFish::GetXModel(void)
{
	return CObjectX::GetXModel();
}