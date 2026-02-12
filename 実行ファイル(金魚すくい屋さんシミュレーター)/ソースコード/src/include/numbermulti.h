//********************************
// 
// マルチテクスチャ版ナンバークラス
// Author Sasaki Soichiro
// 
//********************************
#ifndef _NUMBERMULTI_H_ // このマクロ定義がされていなかったら
#define _NUMBERMULTI_H_ // 二重インクルード防止のマクロ定義

//****************
// インクルード
//****************
#include "main.h"
#include "object2DMulti.h"

//************************************
// マルチテクスチャナンバークラス
//************************************
class CNumberMulti:public CObject2DMulti
{
public:
	typedef enum
	{
		TEXTUREIDX_NUMBER=0,
		TEXTUREIDX_COLOR,
		TEXTUREIDX_MAX
	}TEXTUREIDX;
	CNumberMulti(int nPriority = 5);	// コンストラクタ
	~CNumberMulti();					// デストラクタ
	static CNumberMulti* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos) ;	// 位置
	void SetRot(D3DXVECTOR3 rot) ;	// 向き
	void SetCol(D3DXCOLOR col);		// 色
	void SetNumber(int nNum);		// ナンバーの設定
	void SetGauge(int nGauge);		// ゲージの設定
	void SetSize(D3DXVECTOR2 size);	// 大きさの設定
	// ゲッター
	D3DXVECTOR3 GetPos(void);		// 位置情報
private:
	// 定数
	static constexpr int nInitGauge = 50;		// ゲージの初期値
	static constexpr float fTexOffset = 0.1f;	// テクスチャ座標のオフセット
	static constexpr float fTexHalf = 0.5f;		// テクスチャ座標の半分
	static constexpr float fMaxRatio = 100.0f;
	// 数値
	int m_nIdxTexture[TEXTUREIDX_MAX];	// 使うテクスチャの番号
	int m_nNum;				// 数
};
#endif