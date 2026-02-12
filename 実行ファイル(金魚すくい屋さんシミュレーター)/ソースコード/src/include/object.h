//********************************
//
// オブジェクト処理 [object.h]
// Author : Sasaki Soichiro
//
//********************************
#ifndef _OBJECT_H_ // このマクロ定義がされていなかったら
#define _OBJECT_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"

//******************
// マクロ定義
//******************
#define MAX_OBJ (4000)	// オブジェクトの最大数
#define MAX_PRI (8)		// 描画順の最大数

//********************
// オブジェクトクラス
//********************
class CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_NONE = 0,	// 何もなし
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,		// 敵
		TYPE_BOLT,		// ボルト
		TYPE_PAUSE,		// ポーズ
		TYPE_SCENE,		// シーン
		TYPE_MAX
	}TYPE;
	CObject(int nPriority = 3);	// コンストラクタ
	virtual ~CObject();			// デストラクタ
	virtual HRESULT Init(void) = 0;	// 初期化(純粋仮想関数)
	virtual void Uninit(void) = 0;	// 終了(純粋仮想関数)
	virtual void Update(void) = 0;	// 更新(純粋仮想関数)
	virtual void Draw(void) = 0;	// 描画(純粋仮想関数)

	static void ReleaseAll(void);				// 全てリリース
	static void UpdateAll(void);				// 全て更新
	static void DrawAll(void);					// 全て描画
	// セッター
	void SetType(TYPE type);					// 種類の設定
	static void Destroy(CObject* pObject);
	// ゲッター
	static CObject* GetTop(int nPriority) { return m_apTop[nPriority]; }			// トップ情報取得(リスト構造)
	CObject* GetNext(void) { return m_pNext; }										// ネクスト情報取得(リスト構造)
	TYPE GetType(void) { return m_type; }											// 種類情報の取得
	static int GetNumAll(void) { return m_nNumAll; }								// オブジェクトの総数取得
	static int GetNumPriority(int nPriority) { return m_nNumPriority[nPriority]; }	// 描画順ごとの総数取得

protected:
	void Release(void);					// リリース
private:
	// ポインタ
	static CObject* m_apTop[MAX_PRI];	// トップへのポインタ(リスト構造)
	static CObject* m_apCur[MAX_PRI];	// 現在へのポインタ(リスト構造)
	CObject* m_pNext;					// ネクストへのポインタ(リスト構造)
	CObject* m_pPrev;					// 前のポインタ(リスト構造)
	// enum
	TYPE m_type;						// 種類
	// フラグ
	bool m_bDeath;						// 死亡フラグ
	// 数値
	int m_nPriority;					// 優先順位の位置
	static int m_nNumAll;				// オブジェクトの総数
	static int m_nNumPriority[MAX_PRI];	// 描画順ごとの総数
};	
#endif