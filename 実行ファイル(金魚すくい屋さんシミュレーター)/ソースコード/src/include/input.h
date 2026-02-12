//***************************************
//
// インプット処理 [input.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _INPUT_H_//このマクロ定義がされていなかったら
#define _INPUT_H_//２重インクルード防止のマクロ定義
//***************
// インクルード
//***************
#include "main.h"

//**************
// 入力クラス
//**************
class CInput
{
public:
	CInput();			// コンストラクタ
	virtual ~CInput();	// デストラクタ(純粋仮想関数)
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化(仮想関数)
	virtual void Uninit(void);								// 終了(仮想関数)
	virtual void Update(void) = 0;							// 更新(純粋仮想関数)
protected:
	static LPDIRECTINPUT8 m_pInput;	// 入力のポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;	// 入力デバイスへのポインタ
};

//*******************
// キーボードクラス
//*******************
class CInputKeyboard:public CInput
{
public:
	CInputKeyboard();// コンストラクタ
	~CInputKeyboard();// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	bool GetPress(int nKey);// 押している間
	bool Trigger(int nKey);	// 押した瞬間のみ
	bool Release(int nKey);	// 離したとき
	bool Repeat(int nKey);	// 繰り返し

private:
	BYTE m_aKeyState[256];				// キーボードの前回のプレス情報
	BYTE m_aOldState[256];				// キーボードのプレス情報
};

//**************
// マウスクラス
//**************
class CInputMouse :public CInput
{
public:
	typedef enum
	{
		MOUSE_L = 0, // マウス左
		MOUSE_R,     // マウス右
		MOUSE_H,     // マウスホイール
		MOUSE_MAX
	}MOUSE;
	CInputMouse();	// コンストラクタ
	~CInputMouse();	// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	bool GetDown(MOUSE button_type);// 押したとき
	bool GetUp(MOUSE button_type);	// 離したとき
	D3DXVECTOR2 GetVelocity();		// 向き情報
	D3DXVECTOR2 GetVelocityOld();	// 前フレームの向き情報
	D3DXVECTOR3 GetCursorRay();		// カーソルのレイ
private:
	BYTE m_aState[3];
	BYTE m_aOldState[3];
	DIMOUSESTATE m_CurrentState;	// 現在の入力情報
	DIMOUSESTATE m_PrevState;		// 前フレームの入力情報
	DIMOUSESTATE m_Oldpos;		    // 前フレームの入力情報
};

//********************
// ジョイパッドクラス
//********************
class CInputJoypad :public CInput
{
public:
	typedef enum
	{
		JOYKEY_UP = 0,		//十字キー(上)
		JOYKEY_DOWN,		//十字キー(下)
		JOYKEY_LEFT,		//十字キー(左)
		JOYKEY_RIGHT,		//十字キー(右)
		JOYKEY_START,		//スタートボタン
		JOYKEY_BACK,		//バックボタン
		JOYKEY_LEFTTHUMB,	//左スティックボタン押下
		JOYKEY_RIGHTTHUMB,	//右スティックボタン押下
		JOYKEY_LEFTS,		//LBボタン
		JOYKEY_RIGHTS,		//RBボタン
		JOYKEY_UNNOWN,		//使えない
		JOYKEY_NASHI,		//使えない
		JOYKEY_A,			//Aボタン
		JOYKEY_B,			//Bボタン
		JOYKEY_X,			//Xボタン
		JOYKEY_Y,			//Yボタン
		JOYKEY_N,
		JOYKEY_W,
		JOYKEY_MAX
	}JOYKEY;
	CInputJoypad();		// コンストラクタ
	~CInputJoypad();	// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	bool GetPress(JOYKEY key);
	bool GetTrigger(JOYKEY key);
	XINPUT_STATE*GetState() { return &m_State; }
private:
	XINPUT_STATE m_oldState;				//ジョイパッドの前回のプレス情報
	XINPUT_STATE m_State;					//ジョイパッドのプレス情報
	XINPUT_STATE m_StateTrigger;			//トリガー
};
#endif