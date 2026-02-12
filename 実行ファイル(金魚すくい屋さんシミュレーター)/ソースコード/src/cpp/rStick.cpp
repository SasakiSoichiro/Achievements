//**********************************
//
// Rスティック処理 [rStick.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "rStick.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"


D3DXVECTOR2 CRStick::m_TouchPos =VECTOR2_NULL;
bool CRStick::m_bScreen = false;

//*****************
// コンストラクタ
//*****************
CRStick::CRStick(int nPriority) :CObject2D(nPriority)
{
	m_nIdxTexture = 0;	// インデックス
	m_bDeath = false;
	m_move = VECTOR2_NULL;
	m_RstickPos = VECTOR2_NULL;
	m_RstickOldPos = VECTOR2_NULL;
	m_MouseOldPos = VECTOR2_NULL;
	m_fSpeed = 0.0f;
	m_bScreen = false;
}

//*****************
// デストラクタ
//*****************
CRStick::~CRStick()
{
}

//******************
// インスタンス生成
//******************
CRStick* CRStick::Create(D3DXVECTOR3 pos)
{
	CRStick* pRStick;								// ライフのポインタ
	CTexture* pTexture = CManager::GetCTexture();	// テクスチャのポインタ

	pRStick = new CRStick;// インスタンス生成
	// テクスチャ
	pRStick->m_nIdxTexture = pTexture->Register("data\\TEXTURE\\cursor.png");

	// 初期化
	pRStick->m_TouchPos.x = pos.x;
	pRStick->m_TouchPos.y = pos.y;
	pRStick->Init();

	// 情報を渡す
	return pRStick;
}

//*****************
// 初期化処理
//*****************
HRESULT CRStick::Init(void)
{
	CTexture* pTexture = CManager::GetCTexture();
	CObject2D::BindTexture(pTexture->GetAddres(m_nIdxTexture));

	// 2Dの初期化
	CObject2D::Init();

	// UVの設定
	CObject2D::SetTex(D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f));
	// 大きさの設定
	CObject2D::SetSize(D3DXVECTOR2(fSizeX, fSizeY));
	CObject2D::SetRot(VECTOR3_NULL);
	CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CInputMouse* pMouse = CManager::GetCInputMouse();
	m_TouchPos = pMouse->GetVelocity();
	ShowCursor(FALSE);
	return S_OK;
}

//*****************
// 終了処理
//*****************
void CRStick::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//*****************
// 更新処理
//*****************
void CRStick::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pInputKeyboard;
	CInputJoypad* pInputJoypad;
	pInputKeyboard = CManager::GetCInputKeyboard();
	pInputJoypad = CManager::GetCInputJoypad();
	XINPUT_STATE* pInputJoyStick = pInputJoypad->GetState();
	CInputMouse* pMouse = CManager::GetCInputMouse();

	m_RstickOldPos = m_RstickPos;
	bool bMove = pInputJoyStick->Gamepad.sThumbLX >= nDeadZone
		|| pInputJoyStick->Gamepad.sThumbLX <= -nDeadZone
		|| pInputJoyStick->Gamepad.sThumbLY >= nDeadZone
		|| pInputJoyStick->Gamepad.sThumbLY <= -nDeadZone;
	if (bMove)
	{
		m_RstickPos.x = pInputJoyStick->Gamepad.sThumbLX;
		m_RstickPos.y = pInputJoyStick->Gamepad.sThumbLY;
		m_fSpeed = fSpeed;
	}
	else
	{
		m_RstickPos = VECTOR2_NULL;
		m_fSpeed = 0.0f;
	}

	D3DXVECTOR2 vec= m_RstickPos - VECTOR2_NULL;
	D3DXVec2Normalize(&vec, &vec);
	float fAngle = atan2f(vec.x, vec.y);

	if (bMove)
	{
		m_move.x = sinf(fAngle) * m_fSpeed;
		m_move.y = -cosf(fAngle) * m_fSpeed;
	}
	else
	{
		m_move.x = pMouse->GetVelocity().x - m_MouseOldPos.x;
		m_move.y = pMouse->GetVelocity().y - m_MouseOldPos.y;
	}

	m_TouchPos += m_move;

	m_MouseOldPos = pMouse->GetVelocity();

	SetPos(D3DXVECTOR3(m_TouchPos.x, m_TouchPos.y,0.0f));

	if (m_bScreen)
	{
		SetSize(D3DXVECTOR2(fSizeX, fSizeY));
	}
	else
	{
		SetSize(VECTOR2_NULL);
	}

	if (m_bDeath)
	{
		Uninit();
		return;
	}
}

//*****************
// 描画処理
//*****************
void CRStick::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//*****************
// 位置設定
//*****************
void CRStick::SetPos(D3DXVECTOR3 pos)
{
	// 位置設定
	CObject2D::SetPos(pos);
}

//*****************
// 向き設定
//*****************
void CRStick::SetRot(D3DXVECTOR3 rot)
{
	// 位置設定
	CObject2D::SetRot(rot);
}

//*****************
// 位置情報取得
//*****************
D3DXVECTOR3 CRStick::GetPos(void)
{
	// 位置情報取得
	return CObject2D::GetPos();
}