//******************************************
//
// シェーダファイル処理 [shaderManager.cpp]
// Author:Sasaki Soichiro
//
//******************************************

//*******************
// インクルード
//*******************
#include "shaderManager.h"
#include "manager.h"
#include "renderer.h"

// 静的メンバ変数初期化
int CShaderManager::m_nNumAll = 0;

//*****************
// コンストラクタ
//*****************
CShaderManager::CShaderManager()
{
	for (int nCnt = 0; nCnt < maxLoadNum; nCnt++)
	{
		m_pShader[nCnt] = nullptr;
	}
}

//*****************
// デストラクタ
//*****************
CShaderManager::~CShaderManager()
{
}

//*****************
// 読み込み
//*****************
HRESULT CShaderManager::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();

	return S_OK;
}

//*****************
// 破棄
//*****************
void CShaderManager::Unload(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < maxLoadNum; nCnt++)
	{
		if (m_pShader[nCnt] != nullptr)
		{
			m_pShader[nCnt]->Release();
			m_pShader[nCnt] = nullptr;
		}
	}
}

//*****************
// 登録
//*****************
int CShaderManager::Register(const char* pFilename)
{
	int nCntRegister = 0;
	for (nCntRegister = 0; nCntRegister < maxLoadNum; nCntRegister++)
	{
		if (SHADERFILE[nCntRegister] == NULL)
		{
			if (m_pShader[nCntRegister] == NULL)
			{
				// デバイスの取得
				LPDIRECT3DDEVICE9 pDevice;
				CRenderer* renderer = CManager::GetCRenderer();
				pDevice = renderer->GetDevice();
				// シェーダの読み込み
				D3DXCreateEffectFromFile(pDevice,
					TEXT(pFilename),
					0,
					0,
					D3DXSHADER_DEBUG,
					0,
					&m_pShader[nCntRegister],
					NULL);
				if (m_pShader[nCntRegister] == NULL)
				{
					MessageBox(NULL, "シェーダーを読み込めませんでした", "Error", MB_OK);
				}

				SHADERFILE[nCntRegister] = pFilename;
				return nCntRegister;
			}
			else
			{
				return -1;
			}
		}
		else if (strcmp(pFilename, SHADERFILE[nCntRegister]) == 0)
		{
			return nCntRegister;
		}
	}
	return -1;

}

//*****************
// 取得
//*****************
LPD3DXEFFECT CShaderManager::GetAddres(int nIdx)
{
	return m_pShader[nIdx];
}