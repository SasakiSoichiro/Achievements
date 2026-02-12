//************************************************************
//
// モーション読み込みマネージャ処理 [loadMotionManager.cpp]
// Author:Sasaki Soichiro
//
//************************************************************

//*******************
// インクルード
//*******************
#include "loadMotionManager.h"
#include "manager.h"
#include "renderer.h"
#include "motion.h"

// 静的メンバ変数初期化
int CLoadMotionManager::m_nNumAll = 0;

//****************
// コンストラクタ
//****************
CLoadMotionManager::CLoadMotionManager()
{
	for (int nCnt = 0; nCnt < nMaxLoad; nCnt++)
	{
		m_LoadInfo[nCnt].apMotion.ppModel = nullptr;
		m_LoadInfo[nCnt].apMotion.pMotion = nullptr;
		m_LoadInfo[nCnt].apMotion.nNumModel = 0;
		for (int nCntModel = 0; nCntModel < nMaxModel; nCntModel++)
		{
			m_LoadInfo[nCnt].apModel[nCntModel] = nullptr;
		}
	}
	m_nNumAll++;
}

//****************
// デストラクタ
//****************
CLoadMotionManager::~CLoadMotionManager()
{
}

//****************
// 読み込み
//****************
HRESULT CLoadMotionManager::Load(void)
{

	return S_OK;
}

//****************
// 破棄
//****************
void CLoadMotionManager::Unload(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < nMaxLoad; nCnt++)
	{
		if (m_LoadInfo[nCnt].apMotion.pMotion != nullptr)
		{
			m_LoadInfo[nCnt].apMotion.pMotion->Uninit();
			delete m_LoadInfo[nCnt].apMotion.pMotion;
			m_LoadInfo[nCnt].apMotion.pMotion = nullptr;
		}

		for (int nCntModel = 0; nCntModel < m_LoadInfo[nCnt].apMotion.nNumModel; nCntModel++)
		{
			if (m_LoadInfo[nCnt].apModel[nCntModel] != nullptr)
			{
				m_LoadInfo[nCnt].apModel[nCntModel]->Uninit();
				delete m_LoadInfo[nCnt].apModel[nCntModel];
				m_LoadInfo[nCnt].apModel[nCntModel] = nullptr;
			}
		}
	}
}

//****************
// 登録
//****************
int CLoadMotionManager::Register(const char* pFilename)
{
	int nCntRegister = 0;
	for (nCntRegister = 0; nCntRegister < nMaxLoad; nCntRegister++)
	{
		if (MOTION[nCntRegister] == NULL)
		{
			if (m_LoadInfo[nCntRegister].apMotion.pMotion == nullptr)
			{
				m_LoadInfo[nCntRegister].apMotion.ppModel = &m_LoadInfo[nCntRegister].apModel[0];
				CLoadMotion::LoadMotion(pFilename, m_LoadInfo[nCntRegister].apMotion);
				m_LoadInfo[nCntRegister].apModel[0] = *m_LoadInfo[nCntRegister].apMotion.ppModel;
				m_LoadInfo[nCntRegister].nNumModel = m_LoadInfo[nCntRegister].apMotion.nNumModel;
				MOTION[nCntRegister] = pFilename;
				return nCntRegister;
			}
			else
			{
				return -1;
			}
		}

		else if (strcmp(pFilename, MOTION[nCntRegister]) == 0)
		{
			return nCntRegister;
		}
	}
	return -1;
}

//****************
// 情報取得
//****************
CLoadMotion::Load CLoadMotionManager::GetAddres(int nIdx)
{
	CLoadMotion::Load load;
	load.ppModel = nullptr;
	load.pMotion = nullptr;
	load.nNumModel = 0;
	if (nIdx == -1)
	{
		return load;
	}
	load.ppModel = &m_LoadInfo[nIdx].apModel[0];
	load.nNumModel = m_LoadInfo[nIdx].nNumModel;
	load.pMotion = CMotion::Create(load.ppModel, load.nNumModel, false);

	for (int nCnt = 0; nCnt < 15; nCnt++)
	{
		load.pMotion->SetInfo(m_LoadInfo[nIdx].apMotion.pMotion->GetMotionInfo(nCnt));
	}

	return load;
}

// 静的メンバ変数初期化
int CLoadMotionManagerS::m_nNumAll = 0;

//=================
// コンストラクタ
//=================
CLoadMotionManagerS::CLoadMotionManagerS()
{
	for (int nCnt = 0; nCnt < nMaxLoad; nCnt++)
	{
		for (int nCntModelInfo = 0; nCntModelInfo < nMaxModel; nCntModelInfo++)
		{
			m_LoadInfo[nCnt].load.ModelInfo[nCntModelInfo].nParent=-1;
			m_LoadInfo[nCnt].load.ModelInfo[nCntModelInfo].pos = VECTOR3_NULL;
			m_LoadInfo[nCnt].load.ModelInfo[nCntModelInfo].offsetPos = VECTOR3_NULL;
			m_LoadInfo[nCnt].load.ModelInfo[nCntModelInfo].rot = VECTOR3_NULL;
		}
		m_LoadInfo[nCnt].load.nNumModel = 0;
	}
}

//=================
// デストラクタ
//=================
CLoadMotionManagerS::~CLoadMotionManagerS()
{
}

int CLoadMotionManagerS::Register(const char* pFilename)
{
	int nCntRegister = 0;
	for (nCntRegister = 0; nCntRegister < nMaxLoad; nCntRegister++)
	{
		if (MOTION[nCntRegister] == NULL)
		{
			CLoadMotionS::LoadMotion(pFilename, m_LoadInfo[nCntRegister].load);
			m_LoadInfo[nCntRegister].nNumModel = m_LoadInfo[nCntRegister].load.nNumModel;
			MOTION[nCntRegister] = pFilename;
			return nCntRegister;
		}

		else if (strcmp(pFilename, MOTION[nCntRegister]) == 0)
		{
			return nCntRegister;
		}
	}
	return -1;
}

CMotion* CLoadMotionManagerS::GetAddres(int nIdx,CModel** ppModel, int &nModelNum)
{
	if (nIdx == -1)return nullptr;

	nModelNum = m_LoadInfo[nIdx].nNumModel;

	// モデルのインスタンス生成
	for (int nCnt = 0; nCnt < m_LoadInfo[nIdx].nNumModel; nCnt++)
	{
		ppModel[nCnt] = CModel::Create(VECTOR3_NULL, m_LoadInfo[nIdx].load.ModelInfo[nCnt].modelName.c_str());
	}
	// モデルの初期設定
	for (int nCnt = 0; nCnt < m_LoadInfo[nIdx].nNumModel; nCnt++)
	{
		if (m_LoadInfo[nIdx].load.ModelInfo[nCnt].nParent != -1)
		{
			ppModel[nCnt]->SetParent(ppModel[m_LoadInfo[nIdx].load.ModelInfo[nCnt].nParent]);
		}
		ppModel[nCnt]->SetPos(m_LoadInfo[nIdx].load.ModelInfo[nCnt].pos);
		ppModel[nCnt]->SetOffSetPos(m_LoadInfo[nIdx].load.ModelInfo[nCnt].offsetPos);
		ppModel[nCnt]->SetRot(m_LoadInfo[nIdx].load.ModelInfo[nCnt].rot);
	}
	// モーションのインスタンス生成
	CMotion* pMotion = CMotion::Create(ppModel, m_LoadInfo[nIdx].nNumModel, false);
	// モーションの初期設定
	for (int nCnt = 0; nCnt < nMaxMotion; nCnt++)
	{
		pMotion->SetInfo(m_LoadInfo[nIdx].load.MotionInfo[nCnt]);
	}

	return pMotion;
}