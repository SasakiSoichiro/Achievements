//***************************************
// 
// フェード処理 [fade.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _FADE_H_ // このマクロ定義がされていなかったら
#define _FADE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "scene.h"

//************************
// フェードクラス
//************************
class CFade
{
public:
	// 状態
	typedef enum
	{
		STATE_NONE = 0,	// 何もしていない状態
		STATE_IN,		// フェードイン状態
		STATE_OUT,		// フェードアウト状態
		STATE_MAX
	}STATE;

	CFade();	// コンストラクタ
	~CFade();	// デストラクタ
	HRESULT Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	static void Set(CScene::MODE modeNext);
	STATE Get(void) { return m_state; }
private:

	static constexpr float fAlphaDelta = 0.03f;
	// ポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//頂点バッファへのポインタ
	// 描画系
	D3DXCOLOR m_col;
	// enum
	static STATE m_state;
	static CScene::MODE m_modeNext;
};
#endif