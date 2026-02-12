//**********************************
//
// 入力処理 [input.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "rStick.h"

//****************
// 静的メンバ変数
//****************
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//****************
// コンストラクタ
//****************
CInput::CInput()
{
	m_pDevice = NULL;
}

//****************
// デストラクタ
//****************
CInput::~CInput()
{
}

//****************
// 初期化処理
//****************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//DirectInputのオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		return S_OK;
	}
	return E_FAIL;
}

//****************
// 終了処理
//****************
void CInput::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//****************
// コンストラクタ
//****************
CInputKeyboard::CInputKeyboard()
{
	for (int nCnt = 0; nCnt < 256;nCnt++)
	{
		m_aKeyState[nCnt]=NULL;				// キーボードの前回のプレス情報
		m_aOldState[nCnt]=NULL;				// キーボードのプレス情報
	}
}

//****************
// デストラクタ
//****************
CInputKeyboard::~CInputKeyboard()
{

}

//****************
// 初期化処理
//****************
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//****************
// 終了処理
//****************
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//****************
// 更新処理
//****************
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[256];//キーボードの入力情報

	for (int nCntkey = 0; nCntkey < 256; nCntkey++)
	{
		m_aOldState[nCntkey] = m_aKeyState[nCntkey];
	}
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntkey = 0; nCntkey < 256; nCntkey++)
		{
			m_aKeyState[nCntkey] = aKeyState[nCntkey];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//****************
// 押しているとき
//****************
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//********************************
// 押されているとき一回のみ反応
//********************************
bool CInputKeyboard::Trigger(int nKey)
{
	bool Trigger = false;
	if (m_aKeyState[nKey] & 0x80 && !(m_aOldState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//**********************
// はなされたとき反応
//**********************
bool CInputKeyboard::Release(int nKey)
{
	bool Trigger = false;
	if (m_aOldState[nKey] & 0x80 && !(m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

bool CInputKeyboard::Repeat(int nKey)
{
	bool Trigger = false;
	if (m_aOldState[nKey] & 0x80 && !(m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//****************
// コンストラクタ
//****************
CInputMouse::CInputMouse()
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_aState[nCnt] = NULL;		// マウスの前回のプレス情報
		m_aOldState[nCnt] = NULL;	// マウスのプレス情報
	}
	m_CurrentState = {};	// マウスの現在の入力情報
	m_PrevState = {};		// マウスの1フレーム前の入力情報
	m_Oldpos = {};		    // マウスの現在の入力情報
}

//****************
// デストラクタ
//****************
CInputMouse::~CInputMouse()
{
}

//****************
// 初期化処理
//****************
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モード
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}
//****************
// 終了処理
//****************
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}
//****************
// 更新処理
//****************
void CInputMouse::Update(void)
{
	m_Oldpos = m_CurrentState;

	// 更新前に最新マウス情報を保存する
	m_PrevState = m_CurrentState;

	// 最新のマウスの状態を更新
	HRESULT	hr1 = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentState);

	if (FAILED(hr1))
	{
		m_pDevice->Acquire();
		hr1 = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentState);
	}

	POINT p;
	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &p);

	m_CurrentState.lX = p.x;
	m_CurrentState.lY = p.y;
}
//******************
// 押された処理
//******************
bool CInputMouse::GetDown(MOUSE button_type)
{
	if (!(m_PrevState.rgbButtons[button_type] & (0x80)) &&
		m_CurrentState.rgbButtons[button_type] & (0x80))
	{
		return true;
	}

	return false;
}
//****************
// 離された処理
//****************
bool CInputMouse::GetUp(MOUSE button_type)
{
	if (m_PrevState.rgbButtons[button_type] & 0x80 &&
		!(m_CurrentState.rgbButtons[button_type] & 0x80))
	{
		return true;
	}

	return false;
}

//***************************
// カーソルの位置
//***************************
D3DXVECTOR2 CInputMouse::GetVelocity()
{
	return D3DXVECTOR2((float)m_CurrentState.lX, (float)m_CurrentState.lY);
}

//***************************
// 前フレームのカーソルの位置
//***************************
D3DXVECTOR2 CInputMouse::GetVelocityOld()
{
	return D3DXVECTOR2((float)m_Oldpos.lX, (float)m_Oldpos.lY);
}

//****************
// カーソルのレイ
//****************
D3DXVECTOR3 CInputMouse::GetCursorRay()
{
	D3DXVECTOR3 dir;
	CCamera* pCamera = CManager::GetCCamera();
	D3DXMATRIX projMtx = pCamera->GetProjMtx();
	D3DXMATRIX viewMtx = pCamera->GetViewMtx();

	D3DXVECTOR2 pos = CRStick::GetUIPos();

	// プロジェクションマトリックスから位置の計算する
	float px = ((2.0f * pos.x) / SCREEN_WIDTH - 1.0f) / projMtx._11;
	float py = ((-2.0f * pos.y) / SCREEN_HEIGHT + 1.0f) / projMtx._22;

	dir = D3DXVECTOR3(px, py, 1.0f);

	// 逆行列からレイの原点を求める
	D3DXMATRIX invViewMtx;
	D3DXMatrixInverse(&invViewMtx, NULL, &viewMtx);
	D3DXVECTOR3 rayOrigin = D3DXVECTOR3(invViewMtx._41, invViewMtx._42, invViewMtx._43);

	// レイの方向を作って正規化
	D3DXVec3TransformNormal(&dir, &dir, &invViewMtx);
	D3DXVec3Normalize(&dir, &dir);

	return dir;
}

//****************
// コンストラクタ
//****************
CInputJoypad::CInputJoypad()
{
	// メモリのクリア
	memset(&m_State, 0, sizeof(XINPUT_STATE));
	memset(&m_oldState, 0, sizeof(XINPUT_STATE));
	memset(&m_StateTrigger, 0, sizeof(XINPUT_STATE));
}

//****************
// デストラクタ
//****************
CInputJoypad::~CInputJoypad()
{

}

//****************
// 初期化処理
//****************
HRESULT  CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//XInputのステートを設定
	XInputEnable(true);
	return S_OK;
}

//****************
// 終了処理
//****************
void CInputJoypad::Uninit(void)
{
	//XInputのステートを設定
	XInputEnable(false);

	CInput::Uninit();
}

//****************
// 更新処理
//****************
void CInputJoypad::Update(void)
{
	XINPUT_STATE joykeyState;//ジョイパッドの入力処理
	m_oldState = m_State;
	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_State.Gamepad.wButtons;

		m_StateTrigger.Gamepad.wButtons = Button & ~OldButton;

		m_State = joykeyState;//ジョイパッドのプレス情報を保存
	}
}

//************************************
// ボタンが押されているときずっと反応
//************************************
bool CInputJoypad::GetPress(JOYKEY key)
{
	return(m_State.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//************************************
// ボタンが押されているとき1回のみ反応
//************************************
bool CInputJoypad::GetTrigger(JOYKEY key)
{
	return (m_StateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}