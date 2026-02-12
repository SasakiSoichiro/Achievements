//*****************************************
//
// アイテム箱処理 [itemBox.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "itembox.h"
#include "manager.h"

//*****************
// コンストラクタ
//*****************
CItemBox::CItemBox(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	m_areaPos = VECTOR3_NULL;
	m_type = CItem::ITEM_MAX;
}

//*****************
// デストラクタ
//*****************
CItemBox::~CItemBox()
{
}

//******************
// インスタンス生成
//*******************
CItemBox* CItemBox::Create(D3DXVECTOR3 pos,CItem::ITEM type)
{
	CItemBox* pItemBox;

	// インスタンス生成
	pItemBox = new CItemBox;
	// モデルの設定
	switch (type)
	{
	case CItem::ITEM_SOFTPOI:
		pItemBox->SetXModel("data\\MODEL\\pois_red.x");
		break;
	case CItem::ITEM_MIDIUMPOI:
		pItemBox->SetXModel("data\\MODEL\\pois_yellow.x");
		break;
	case CItem::ITEM_HARDPOI:
		pItemBox->SetXModel("data\\MODEL\\pois_white.x");
		break;
	case CItem::ITEM_BOWL:
		pItemBox->SetXModel("data\\MODEL\\bowls.x");
		break;
	case CItem::ITEM_BAG_S:
		pItemBox->SetXModel("data\\MODEL\\bags_S.x");
		break;
	case CItem::ITEM_BAG_L:
		pItemBox->SetXModel("data\\MODEL\\bags_L.x");
		break;

	}
	// 初期化
	pItemBox->SetPos(pos);
	pItemBox->Init();
	pItemBox->m_type = type;

	return pItemBox;
}

//*****************
// 初期化処理
//*****************
HRESULT CItemBox::Init(void)
{
	CObjectX::Init();
	D3DXVECTOR3 rot = GetRot();
	rot.y -= PI_HALF;
	SetRot(rot);
	D3DXVECTOR3 pos = GetPos();
	m_areaPos.x = pos.x - sinf(rot.y) * distance;
	m_areaPos.y = pos.y;
	m_areaPos.z = pos.z - cosf(rot.y) * distance;

	return S_OK;
}

//*****************
// 終了処理
//*****************
void CItemBox::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CItemBox::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CItemBox::Draw(void)
{
	CObjectX::Draw();
}

//*****************
// 範囲処理
//*****************
bool CItemBox::Area(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 itemPos = GetPos();

	// 距離を求める
	float fDistX = pos.x - m_areaPos.x;
	float fDistZ = pos.z - m_areaPos.z;
	float fDistance = sqrtf((fDistX * fDistX) + (fDistZ * fDistZ));

	const float fRadius = radius;

	if (fDistance < fRadius)
	{// 範囲
		SetPos(D3DXVECTOR3(itemPos.x, minHeight, itemPos.z));
		return true;
	}
	SetPos(D3DXVECTOR3(itemPos.x, maxHeight, itemPos.z));

	return false;
}

//*****************
// 拾う
//*****************
CItem* CItemBox::Pick(void)
{
	return CItem::Create(m_areaPos,m_type);
}

//*****************
// 置く
//*****************
CItem* CItemBox::Put(CItem* pItem)
{
	if (pItem->GetType() == m_type)
	{
		pItem->Uninit();
		pItem = nullptr;
		return pItem;
	}
	else
	{
		return pItem;
	}
}

//******************
// 位置の設定
//******************
void CItemBox::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CItemBox::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CItemBox::SetXModel(const char* fileName)
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
D3DXVECTOR3 CItemBox::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CItemBox::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CItemBox::GetXModel(void)
{
	return CObjectX::GetXModel();
}
