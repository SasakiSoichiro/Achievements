//*****************************************
//
// ボウル処理 [bowl.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "bowl.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "player.h"
#include "mesh.h"
#include <time.h>
#include "block.h"

//*****************
// コンストラクタ
//*****************
CBowl::CBowl(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	m_nFishCnt = 0;
	m_bDeath = false;
	m_posOld = VECTOR3_NULL;
}

//*****************
// デストラクタ
//*****************
CBowl::~CBowl()
{
}

//******************
// インスタンス生成
//*******************
CBowl* CBowl::Create(D3DXVECTOR3 pos)
{
	CBowl* pBowl;

	// インスタンス生成
	pBowl = new CBowl;
	// モデルの設定
	pBowl->SetXModel("data\\MODEL\\customer_bowl.x");
	// 初期化
	pBowl->SetPos(pos);
	pBowl->Init();

	return pBowl;
}

//*****************
// 初期化処理
//*****************
HRESULT CBowl::Init(void)
{
	CObjectX::Init();
	SetShadowScreen(false);

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CBowl::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CBowl::Update(void)
{
	if (m_bDeath)
	{
		Uninit();
	}
}

//*****************
// 描画処理
//*****************
void CBowl::Draw(void)
{
	CObjectX::Draw();
}

//****************************************
// 範囲に入ったらプレイヤーの状態を変える
//****************************************
bool CBowl::Area(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 bowlPos = GetPos();
	// 距離を求める
	float fDistX = pos.x - bowlPos.x;
	float fDistZ = pos.z - bowlPos.z;
	float fDistance = sqrtf((fDistX * fDistX) + (fDistZ * fDistZ));

	if (fDistance < fAreaRadius)
	{// 範囲
		return false;
	}

	return true;
}

//******************
// 位置の設定
//******************
void CBowl::SetPos(D3DXVECTOR3 pos)
{
	m_posOld = GetPos();
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CBowl::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CBowl::SetXModel(const char* fileName)
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
D3DXVECTOR3 CBowl::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CBowl::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CBowl::GetXModel(void)
{
	return CObjectX::GetXModel();
}

D3DXVECTOR3 CBowl::GetMove(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = pos - m_posOld;
	return move;
}