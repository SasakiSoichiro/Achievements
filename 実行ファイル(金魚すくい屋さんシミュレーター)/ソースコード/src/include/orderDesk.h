//**************************************
//
// 注文机処理 [orderDesk.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _ORDERDESK_H_ // このマクロ定義がされていなかったら
#define _ORDERDESK_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"

class CBlock;

//********************
// 注文机クラス
//********************
class COrderDesk :public CObjectX
{
public:

	COrderDesk(int nPriority = 4);	// コンストラクタ
	~COrderDesk();					// デストラクタ
	static COrderDesk* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetRot(D3DXVECTOR3 rot);	// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	// ゲッター
	D3DXVECTOR3 GetPos(void);		// 位置情報の取得
	D3DXVECTOR3 GetRot(void);		// 向き情報の取得
	CXModel::XMODEL GetXModel(void);// モデル情報の取得
	bool Area(D3DXVECTOR3 pos);		// 注文範囲
private:

	static constexpr float fAreaDistance = 30.0f;
	static constexpr float fAreaRadius = 20.0f;
	// ポインタ
	CBlock* m_pOrderArea;	// オーダーエリア
	// 座標
	D3DXVECTOR3 m_orderPos;	// 注文を受け付ける位置
	// 数値
	int m_nIdxXFile;		// 使うモデルの番号

};
#endif