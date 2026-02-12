//**************************************
//
// オブジェクト処理 [object.cpp]
// Author Sasaki Soichiro
//
//**************************************

//*******************
// インクルード
//*******************
#include "object.h"
#include "renderer.h"
#include "debugproc.h"
#include "camera.h"
#include "manager.h"
#include "pause.h"

//**********************
// 静的メンバ変数初期化
//**********************
int CObject::m_nNumAll = 0;
int CObject::m_nNumPriority[MAX_PRI] = {};
CObject* CObject::m_apTop[MAX_PRI] = {};
CObject* CObject::m_apCur[MAX_PRI] = {};

//*****************
// コンストラクタ
//*****************
CObject::CObject(int nPriority)
{

	if (m_apTop[nPriority]== NULL)
	{
		m_apTop[nPriority] = this;		// 自分自身を代入
		m_apCur[nPriority] = this;
		m_pPrev = NULL;
		m_pNext = NULL;
	}
	else
	{
		m_pPrev = m_apCur[nPriority];
		m_apCur[nPriority]->m_pNext = this;
		m_pNext = NULL;
		m_apCur[nPriority] = this;
	}
	m_nPriority = nPriority;
	m_type = TYPE_NONE;
	m_nNumAll++;					// 総数をカウントアップ
	m_nNumPriority[nPriority]++;
	m_bDeath = false;
}
//*****************
// デストラクタ
//*****************
CObject::~CObject()
{

}

//***************
// 全ての更新
//***************
void CObject::UpdateAll(void)
{
	bool bPause = CPause::GetPause();

	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];
		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;
			if (!bPause)
			{
				// 更新処理
				pObject->Update();
			}
			else if (bPause && pObject->GetType() == TYPE_PAUSE || pObject->GetType() == TYPE_SCENE)
			{// ポーズ中にポーズの更新のみする
				// 更新処理
				pObject->Update();
			}
			pObject = pObjectNext;// 次のオブジェクトを代入
		}

		pObject = m_apTop[nCntPriority];
		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;
			if (pObject != NULL && pObject->m_bDeath == true)
			{
				Destroy(pObject);
			}
			pObject = pObjectNext;// 次のオブジェクトを代入
		}
	}
}

//****************
// 全ての描画
//****************
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCCamera();
	pCamera->SetCamera();
	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];
		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;

			// 更新処理
			pObject->Draw();
			pObject = pObjectNext;// 次のオブジェクトを代入
		}

	}
}

//***************
// 全ての終了
//***************
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];
		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;

			// 更新処理
			pObject->Uninit();
			pObject = pObjectNext;// 次のオブジェクトを代入
		}
		pObject = m_apTop[nCntPriority];
		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;
			if (pObject != NULL && pObject->m_bDeath == true)
			{
				int nPriority = pObject->m_nPriority;
				if (pObject->m_pNext != NULL && pObject->m_pPrev != NULL)
				{
					pObject->m_pPrev->m_pNext = pObject->m_pNext;
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;
				}
				else if (m_apTop[nPriority]->m_pPrev == NULL && pObject->m_pNext != NULL)
				{
					m_apTop[nPriority] = pObject->m_pNext;
					pObject->m_pNext->m_pPrev = NULL;
				}
				else if (m_apCur[nPriority]->m_pNext == NULL && pObject->m_pPrev != NULL)
				{
					m_apCur[nPriority] = pObject->m_pPrev;
					pObject->m_pPrev->m_pNext = NULL;
				}
				if (m_apTop[nPriority] == pObject)
				{
					m_apTop[nPriority] = NULL;
				}
				pObject->m_pNext = NULL;
				pObject->m_pPrev = NULL;
				delete pObject;
				pObject = nullptr;
				m_nNumAll--;		// 総数をカウントダウン
				m_nNumPriority[nPriority]--;
			}
			pObject = pObjectNext;// 次のオブジェクトを代入
		}

	}
}

//**************
// 終了
//**************
void CObject::Release(void)
{
	m_bDeath = true;
}

//**************
// 種類を設定
//**************
void CObject::SetType(CObject::TYPE type)
{
	m_type = type;
}

//*******************
// オブジェクト消す
//*******************
void CObject::Destroy(CObject* pObject)
{
	int nPriority = pObject->m_nPriority;
	if (pObject->m_pNext != nullptr && pObject->m_pPrev != nullptr)
	{
		pObject->m_pPrev->m_pNext = pObject->m_pNext;
		pObject->m_pNext->m_pPrev = pObject->m_pPrev;
	}
	else if (pObject->m_pPrev == nullptr && pObject->m_pNext != nullptr)
	{
		m_apTop[nPriority] = pObject->m_pNext;
		pObject->m_pNext->m_pPrev = NULL;
	}
	else if (pObject->m_pNext == nullptr && pObject->m_pPrev != nullptr)
	{
		m_apCur[nPriority] = pObject->m_pPrev;
		pObject->m_pPrev->m_pNext = NULL;
	}
	else
	{
		m_apCur[nPriority] = nullptr;
		m_apTop[nPriority] = nullptr;
	}

	m_nNumAll--;		// 総数をカウントダウン
	pObject->m_pNext = NULL;
	pObject->m_pPrev = NULL;
	delete pObject;
	m_nNumPriority[nPriority]--;

}