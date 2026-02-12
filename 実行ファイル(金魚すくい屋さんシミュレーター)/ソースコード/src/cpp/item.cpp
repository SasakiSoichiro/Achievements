//*****************************************
//
// アイテム処理 [item.cpp]
// Author:Sasaki Soichiro
//
//*****************************************

//*******************
// インクルード
//*******************
#include "item.h"
#include "manager.h"

//*****************
// コンストラクタ
//*****************
CItem::CItem(int nPriority) :CObjectX(nPriority)
{
	m_nIdxXFile = -1;
	m_type = ITEM_MAX;
}

//*****************
// デストラクタ
//*****************
CItem::~CItem()
{
}

//*******************
// インスタンス生成
//*******************
CItem* CItem::Create(D3DXVECTOR3 pos, ITEM type)
{
	CItem* pItem;
	CTexture* pTexture = CManager::GetCTexture();

	int nNum = CObject::GetNumAll();
	if (nNum < MAX_OBJ - 1)
	{
		pItem = new CItem;
		pItem->m_type = type;
		switch (type)
		{
		case ITEM_SOFTPOI:
			pItem->SetXModel("data\\MODEL\\poi_red.x");
			break;
		case ITEM_MIDIUMPOI:
			pItem->SetXModel("data\\MODEL\\poi_yellow.x");
			break;
		case ITEM_HARDPOI:
			pItem->SetXModel("data\\MODEL\\poi_white.x");
			break;
		case ITEM_BOWL:
			pItem->SetXModel("data\\MODEL\\bowl.x");
			break;
		case ITEM_BAG_S:
			pItem->SetXModel("data\\MODEL\\baggingS.x");
			break;
		case ITEM_BAG_L:
			pItem->SetXModel("data\\MODEL\\baggingL.x");
			break;
		}
		pItem->SetPos(pos);
		pItem->Init();

		return pItem;
	}
	return NULL;
}
//*****************
// 初期化処理
//*****************
HRESULT CItem::Init(void)
{
	CObjectX::Init();
	CObjectX::SetShadowScreen(false);
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CItem::Uninit(void)
{
	CObjectX::Uninit();
}

//*****************
// 更新処理
//*****************
void CItem::Update(void)
{

}

//*****************
// 描画処理
//*****************
void CItem::Draw(void)
{
	CObjectX::Draw();
}

//******************
// 位置の設定
//******************
void CItem::SetPos(D3DXVECTOR3 pos)
{
	CObjectX::SetPos(pos);
}

//******************
// 向きの設定
//******************
void CItem::SetRot(D3DXVECTOR3 rot)
{
	CObjectX::SetRot(rot);
}

//******************
// 使うモデルの設定
//******************
void CItem::SetXModel(const char* fileName)
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
D3DXVECTOR3 CItem::GetPos(void)
{
	return CObjectX::GetPos();
}

//******************
// 向き情報取得
//******************
D3DXVECTOR3 CItem::GetRot(void)
{
	return CObjectX::GetRot();
}

//******************
// モデル情報取得
//******************
CXModel::XMODEL CItem::GetXModel(void)
{
	return CObjectX::GetXModel();
}