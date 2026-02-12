//*****************************************
//
// 注文机処理 [orderDesk.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "orderDesk.h"
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
COrderDesk::COrderDesk(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	m_orderPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pOrderArea = nullptr;
}

//*****************
// デストラクタ
//*****************
COrderDesk::~COrderDesk()
{
}

//******************
// インスタンス生成
//*******************
COrderDesk* COrderDesk::Create(D3DXVECTOR3 pos)
{
	COrderDesk* pOrderDesk;

	// インスタンス生成
	pOrderDesk = new COrderDesk;
	// モデルの設定
	pOrderDesk->SetXModel("data\\MODEL\\orderDesk.x");
	// 初期化
	pOrderDesk->SetPos(pos);
	pOrderDesk->Init();

	return pOrderDesk;
}

//*****************
// 初期化処理
//*****************
HRESULT COrderDesk::Init(void)
{
	CObjectX::Init();

	D3DXVECTOR3 rot = GetRot();
	rot.y -=PI_HALF;
	SetRot(rot);
	D3DXVECTOR3 pos = GetPos();
	m_orderPos.x = pos.x + sinf(rot.y) * fAreaDistance;
	m_orderPos.y = pos.y;
	m_orderPos.z = pos.z + cosf(rot.y) * fAreaDistance;
	m_pOrderArea = CBlock::Create(m_orderPos,0.0f, "data\\MODEL\\area.x",0);

	return S_OK;
}

//*****************
// 終了処理
//*****************
void COrderDesk::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void COrderDesk::Update(void)
{

}

//*****************
// 描画処理
//*****************
void COrderDesk::Draw(void)
{
	CObjectX::Draw();
}

//****************************************
// 範囲に入ったらプレイヤーの状態を変える
//****************************************
bool COrderDesk::Area(D3DXVECTOR3 pos)
{
	// 距離を求める
	float fDistX = pos.x - m_orderPos.x;
	float fDistZ = pos.z - m_orderPos.z;
	float fDistance = sqrtf((fDistX * fDistX) + (fDistZ * fDistZ));

	if (fDistance < fAreaRadius)
	{// 範囲
		return true;
	}

	return false;
}
//******************
// 位置の設定
//******************
void COrderDesk::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void COrderDesk::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void COrderDesk::SetXModel(const char* fileName)
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
D3DXVECTOR3 COrderDesk::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 COrderDesk::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL COrderDesk::GetXModel(void)
{
	return CObjectX::GetXModel();
}