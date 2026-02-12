//**********************************
//
// テクスチャ処理 [texture.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

// 静的メンバ変数初期化
int CTexture::m_nNumAll=0;

//*******************
// コンストラクタ
//*******************
CTexture::CTexture()
{
	for (int nCnt = 0; nCnt < nMaxTexture; nCnt++)
	{
		m_apTexture[nCnt] = NULL;
	}
	m_nNumAll++;
}

//*******************
// デストラクタ
//*******************
CTexture::~CTexture()
{
}

//*******************
// 読み込み
//*******************
HRESULT CTexture::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer* renderer = CManager::GetCRenderer();
	pDevice = renderer->GetDevice();
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE[nCnt],
			&m_apTexture[nCnt]);
	}

	return S_OK;
}

//*******************
// 破棄
//*******************
void CTexture::Unload(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < nMaxTexture; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//*******************
// 登録
//*******************
int CTexture::Register(const char* pFilename)
{
	int nCntRegister = 0;
	for (nCntRegister = 0; nCntRegister < nMaxTexture; nCntRegister++)
	{
		if (TEXTURE[nCntRegister] == NULL)
		{
			if (m_apTexture[nCntRegister] == NULL)
			{
				// デバイスの取得
				LPDIRECT3DDEVICE9 pDevice;
				CRenderer* renderer = CManager::GetCRenderer();
				pDevice = renderer->GetDevice();
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pFilename,
					&m_apTexture[nCntRegister]);
				TEXTURE[nCntRegister] = pFilename;
				return nCntRegister;
			}
			else
			{
				return -1;
			}
		}

		else if (strcmp(pFilename, TEXTURE[nCntRegister]) == 0)
		{
			return nCntRegister;
		}
	}
	return -1;
}

//*******************
// 情報取得
//*******************
LPDIRECT3DTEXTURE9 CTexture::GetAddres(int nIdx)
{
	if (nIdx == -1)
	{
		return nullptr;
	}
	return m_apTexture[nIdx];
}