//**************************************
//
// 袋詰め机処理 [baggingDesk.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _BAGGINGDESK_H_ // このマクロ定義がされていなかったら
#define _BAGGINGDESK_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"

class CBlock;

//********************
// 袋詰め机クラス
//********************
class CBaggingDesk :public CObjectX
{
public:

	CBaggingDesk(int nPriority = 4);	// コンストラクタ
	~CBaggingDesk();					// デストラクタ
	static CBaggingDesk* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	bool Area(D3DXVECTOR3 pos);		// 注文範囲
	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetRot(D3DXVECTOR3 rot);	// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	D3DXVECTOR3 GetPos(void);				// 位置情報の取得
	D3DXVECTOR3 GetRot(void);				// 向き情報の取得
	CXModel::XMODEL GetXModel(void);		// モデル情報の取得
private:
	// 定数
	static constexpr float fOffsetDistance = 30.0f;
	static constexpr float fAreaRadius = 20.0f;
	// ポインタ
	CBlock* m_pArea;	// オーダーエリア
	// 座標
	D3DXVECTOR3 m_baggingPos;	// 注文を受け付ける位置
	// 数値
	int m_nIdxXFile;		// 使うモデルの番号
};
#endif