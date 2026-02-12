//********************************************
//
// 怒りマークエフェクト処理 [angryEffect.h]
// Author Sasaki Soichiro
//
//********************************************
#ifndef _ANGRYEFFECT_H_//このマクロ定義がされていなかったら
#define _ANGRYEFFECT_H_//2重インクルード防止のマクロ定義
//*******************
// インクルード
//*******************
#include "main.h"
#include "objectBillboard.h"

//*****************************
// 怒りマークエフェクトクラス
//*****************************
class CAngryEffect :public CObjectBillboard
{
public:
	typedef enum
	{
		TYPE_ONE = 0,
		TYPE_TWO,
		TYPE_TREE,
		TYPE_MAX
	}TYPE;
	CAngryEffect(int nPriority = 4);	// コンストラクタ
	~CAngryEffect();					// デストラクタ
	static CAngryEffect* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetCol(D3DXCOLOR col);		// 色の設定
	void SetSize(D3DXVECTOR2 size);	// 大きさの設定
	D3DXVECTOR3 GetPos(void);		// 位置情報取得
	D3DXVECTOR2 GetSize(void);		// 大きさ情報取得
private:
	// 定数
	static constexpr int nMaxLife = 60;
	static constexpr float fSpeed = 0.4f;
	// 数値
	int  m_nIdxTexture;			// 使っているテクスチャの番号
	int m_nLife;				// 寿命
};
#endif