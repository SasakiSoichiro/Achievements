//************************************
//
// マネージャ処理 [manager.h]
// Author:Sasaki Soichiro
//
//************************************
#ifndef _MANAGER_H_ // このマクロ定義がされていなかったら
#define _MANAGER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "scene.h"

class CRenderer;		
class CInputKeyboard;	
class CInputMouse;		
class CInputJoypad;		
class CSound;			
class CDebugProc;		
class CTexture;			
class CCamera;			
class CLight;			
class CXModel;			
class CScene;			
class CBlurManager;		
class CPauseManager;
class CFade;
class CLoadMotionManagerS;
class CShaderManager;

//********************
// マネージャクラス
//********************
class CManager
{
public:
	CManager();		// コンストラクタ
	~CManager();	// デストラクタ
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);	// 初期化
	void Uninit(void);											// 終了
	void Update(void);											// 更新
	void Draw(void);											// 描画
	static CRenderer* GetCRenderer(void) { return m_pRenderer; }				// レンダラー情報取得
	static CInputKeyboard* GetCInputKeyboard(void) { return m_pInputKeyboard; }	// キーボード情報取得
	static CInputMouse* GetCInputMouse(void) { return m_pInputMouse; }			// マウス情報取得
	static CInputJoypad* GetCInputJoypad(void) { return m_pInputJoypad; }		// ジョイパッド情報取得
	static CSound* GetCSound(void) { return m_pSound; }							// サウンド情報取得
	static CDebugProc* GetCDebugProc(void) { return m_pDebugProc; }				// デバッグ表示情報取得
	static CTexture* GetCTexture(void) { return m_pTexture; }					// テクスチャ情報取得
	static CCamera* GetCCamera(void) { return m_pCamera; }						// カメラ情報取得
	static CLight* GetCLight(void) { return m_pLight; }							// ライト情報取得
	static CXModel* GetCXModel(void) { return m_pXModel; }						// モデル情報取得
	static CBlurManager* GetCBlurManager(void) { return m_pBlur; }				// ブラー情報取得
	static CPauseManager* GetCPauseManager(void) { return m_pPauseManager; }	// ポーズマネージャー情報取得
	static CLoadMotionManagerS* GetLoadMotionManager(void) { return m_pLoadMotionManager; }// モーション読み込みマネージャ情報取得
	static CShaderManager* GetShaderManager(void) { return m_pShaderManager; }	// シェーダ読み込みマネージャ情報取得
	static CFade* GetFade(void) { return m_pFade; }								// フェード情報取得
	static void SetMode(CScene::MODE mode);										// シーンの設定
	static CScene::MODE GetMode(void);											// モード情報取得
private:
	static CRenderer* m_pRenderer;				// レンダラーへのポインタ
	static CInputKeyboard* m_pInputKeyboard;	// キーボードへのポインタ
	static CInputMouse* m_pInputMouse;			// マウスへのポインタ
	static CInputJoypad* m_pInputJoypad;		// ジョイパッドへのポインタ
	static CSound* m_pSound;					// サウンドへのポインタ
	static CDebugProc* m_pDebugProc;			// デバッグ表示へのポインタ
	static CTexture* m_pTexture;				// テクスチャへのポインタ
	static CCamera* m_pCamera;					// カメラへのポインタ
	static CLight* m_pLight;					// ライトへのポインタ
	static CXModel* m_pXModel;					// モデルへのポインタ
	static CScene* m_pScene;					// シーンへのポインタ
	static CBlurManager* m_pBlur;				// ブラーへのポインタ
	static CPauseManager* m_pPauseManager;		// ポーズマネージャーへのポインタ
	static CFade* m_pFade;						// フェードへのポインタ
	static CLoadMotionManagerS* m_pLoadMotionManager;// モーション読み込みマネージャへのポインタ
	static CShaderManager* m_pShaderManager;	// シェーダマネージャへのポインタ
};
#endif