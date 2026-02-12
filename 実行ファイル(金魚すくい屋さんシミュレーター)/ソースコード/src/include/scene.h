//**************************************
//
// シーン処理 [scene.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _SCENE_H_ // このマクロ定義がされていなかったら
#define _SCENE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"

//****************
// マクロ定義
//****************
#define NONE (D3DXVECTOR3(0.0f,0.0f,0.0f))


//********************
// シーンクラス
//********************
class CScene:public CObject
{
public:
	typedef enum
	{
		MODE_TITLE=0,	// タイトル
		MODE_GAME,		// ゲーム
		MODE_RESULT,	// リザルト
		MODE_MAX
	}MODE;
	CScene(MODE mode);	// コンストラクタ
	~CScene();			// デストラクタ
	static CScene* Create(MODE mode);			// インスタンス生成
	virtual HRESULT Init(void) = 0;		// 初期化(純粋仮想関数)
	virtual void Uninit(void) = 0;		// 終了(純粋仮想関数)
	virtual void Update(void) = 0;		// 更新(純粋仮想関数)
	virtual void Draw(void) = 0;		// 描画(純粋仮想関数)
	// ゲッター
	MODE GetMode(void) { return m_mode; }		// モード情報取得
private:
	MODE m_mode;	// モード
};
#endif