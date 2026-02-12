//**********************************
//
// ポーズ処理 [pause.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//*****************
// 静的メンバ変数
//*****************
bool CPause::m_bPause = false;

//=================
// コンストラクタ
//=================
CPause::CPause(int nPriority) :CObject2D(nPriority)
{
	m_nIdxTexture = 0;
	m_fAnim = 0;
}

//*******************
// デストラクタ
//*******************
CPause::~CPause()
{
}

//*******************
// インスタンス生成
//*******************
CPause* CPause::Create(D3DXVECTOR3 pos)
{
	CPause* pPause = NULL;
	CTexture* pTexture = CManager::GetCTexture();
	pPause = new CPause;

	// 初期化
	pPause->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\pause.png");
	pPause->Init();
	pPause->SetPos(pos);
	return pPause;
}

//*******************
// 初期化処理
//*******************
HRESULT CPause::Init(void)
{
	CObject2D::Init();
	CObject2D::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	CObject2D::SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	CTexture* pTexture = CManager::GetCTexture();
	CObject2D::BindTexture(pTexture->GetAddres(m_nIdxTexture));
	CObject::SetType(CObject::TYPE_PAUSE);

	return S_OK;

}

//*******************
// 終了処理
//*******************
void CPause::Uninit(void)
{
	CObject2D::Uninit();
}

//*******************
// 更新処理
//*******************
void CPause::Update(void)
{
	bool bInput = CManager::GetCInputKeyboard()->Trigger(DIK_TAB) 
		|| CManager::GetCInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_BACK);
	if (bInput)
	{
		SwitchPause();
	}

	// アニメーション
	Anim();
}

//*******************
// 描画処理
//*******************
void CPause::Draw(void)
{
	CObject2D::Draw();
}

//*******************
// アニメーション
//*******************
void CPause::Anim(void)
{
	if (m_bPause)
	{
		m_fAnim += fSpeed;
	}
	else
	{
		m_fAnim -= fSpeed;
	}

	// 最大値と最小値設定
	if (m_fAnim >= 1.0f)
	{
		m_fAnim = 1.0f;
	}
	else if (m_fAnim <= 0.0f)
	{
		m_fAnim = 0.0f;
	}
	// 大きさのアニメーション
	SetSize(D3DXVECTOR2(SCREEN_WIDTH* m_fAnim, SCREEN_HEIGHT* m_fAnim));
}

//*******************
// 位置設定
//*******************
void CPause::SetPos(D3DXVECTOR3 pos)
{
	CObject2D::SetPos(pos);
}

//*******************
// 位置情報取得
//*******************
D3DXVECTOR3 CPause::GetPos(void)
{
	return CObject2D::GetPos();
}

//*******************
// 大きさ取得
//*******************
D3DXVECTOR2 CPause::GetSize(void)
{
	return CObject2D::GetSize();
}