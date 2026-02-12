//******************************************
// 
// オブジェクトX処理 [objectX.h]
// Author:Sasaki Soichiro
//
//******************************************
#ifndef _OBJECTX_H_// このマクロ定義がされていなかったら
#define _OBJECTX_H_// 2重インクルード防止のマクロ定義
//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "Xmodel.h"

//******************
// 前方宣言
//******************
class CShadow;

//***********************
// Xファイルの表示クラス
//***********************
class CObjectX :public CObject
{
public:
	CObjectX(int nPriority = 4);	// コンストラクタ
	~CObjectX();					// デストラクタ
	static CObjectX* Create(D3DXVECTOR3 pos,bool bQua);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }					// 位置の設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }					// 向きの設定
	void SetSize(D3DXVECTOR3 size) { m_size = size; }				// 大きさの設定
	void SetMatrix(D3DMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }	// マトリックスの設定
	void SetIsQuaternion(bool bQua) { m_bQua = bQua; }				// クォータニオンを使うか
	void SetQuaternion(D3DXQUATERNION qua) { m_qua = qua; }
	void SetShadowScreen(bool bScreen);
	void BindXFile(CXModel::XMODEL pXFile);							// Xファイルの割り当て
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; };						// 位置情報
	D3DXVECTOR3 GetRot(void) { return m_rot; };						// 向き情報
	D3DXVECTOR3 GetSize(void) { return m_size; }					// 大きさ情報
	CXModel::XMODEL GetXModel(void){return m_XModel;}				// XModel情報
private:
	// 定数
	static constexpr int nMaxTexture = 128;
	static constexpr int nShadowHeightRand = 100;
	static constexpr float nShadowOffsetHeight = -10.0f;
	static constexpr float nShadowRandSmall = 0.001f;
	// ポインタ
	CXModel::XMODEL m_XModel;			// Xmodel情報
	LPDIRECT3DTEXTURE9 m_apTextureModel;// テクスチャのポインタ
	CShadow* m_pShadow;
	// 座標
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_size;					// 大きさ
	D3DXQUATERNION m_qua;				// クォータニオン
	D3DXMATRIX m_mtxWorld;				// マトリックス
	// フラグ
	bool m_bQua;						// クォータニオンを使うか
	// 数値
	int  m_nIdxTexture[nMaxTexture];	// テクスチャのインデックス
};

#endif