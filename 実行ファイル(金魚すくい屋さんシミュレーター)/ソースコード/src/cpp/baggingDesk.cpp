//*****************************************
//
// 袋詰め机処理 [baggingDesk.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "baggingDesk.h"
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
CBaggingDesk::CBaggingDesk(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	m_baggingPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pArea = nullptr;
}

//*****************
// デストラクタ
//*****************
CBaggingDesk::~CBaggingDesk()
{
}

//******************
// インスタンス生成
//*******************
CBaggingDesk* CBaggingDesk::Create(D3DXVECTOR3 pos)
{
	CBaggingDesk* pOrderDesk;

	// インスタンス生成
	pOrderDesk = new CBaggingDesk;
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
HRESULT CBaggingDesk::Init(void)
{
	CObjectX::Init();

	D3DXVECTOR3 orderPos;
	D3DXVECTOR3 rot = GetRot();
	rot.y -= PI_HALF;
	SetRot(rot);
	orderPos.x = GetPos().x + sinf(rot.y) * fOffsetDistance;
	orderPos.y = GetPos().y;
	orderPos.z = GetPos().z + cosf(rot.y) * fOffsetDistance;
	m_baggingPos = orderPos;
	m_pArea = CBlock::Create(orderPos,0.0f, "data\\MODEL\\area.x", 0);

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CBaggingDesk::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CBaggingDesk::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CBaggingDesk::Draw(void)
{
	CObjectX::Draw();
}

//****************************************
// 範囲に入ったらプレイヤーの状態を変える
//****************************************
bool CBaggingDesk::Area(D3DXVECTOR3 pos)
{
	// 距離を求める
	float fDistX = pos.x - m_baggingPos.x;
	float fDistZ = pos.z - m_baggingPos.z;
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
void CBaggingDesk::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CBaggingDesk::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CBaggingDesk::SetXModel(const char* fileName)
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
D3DXVECTOR3 CBaggingDesk::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CBaggingDesk::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CBaggingDesk::GetXModel(void)
{
	return CObjectX::GetXModel();
}