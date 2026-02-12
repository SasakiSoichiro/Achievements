//***********************************
//
// モデル処理 [model.h]
// Author:Sasaki Soichiro
//
//***********************************
#ifndef _MODEL_H_// このマクロ定義がされていなかったら
#define _MODEL_H_// 2重インクルード防止のマクロ定義

//**********************
// インクルード
//**********************
#include"main.h"
#include"Xmodel.h"

//*********************
// モデルクラス
//*********************
class CModel
{
public:
    CModel();// コンストラクタ
    ~CModel();// デストラクタ
    static CModel*Create(D3DXVECTOR3 pos, const char* pFileName);// インスタンス生成
    HRESULT Init(void);	// 初期化
    void Uninit(void);	// 終了
    void Update(void);	// 更新
    void Draw(void);	// 描画
    void DrawShaodow(void);// 影
    // セッター
    void SetParent(CModel* pModel);// 親モデルの設定
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }               // 位置の設定
    void SetOffSetPos(D3DXVECTOR3 pos) { m_offsetPos = pos; }   // オフセットの設定(位置)
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }               // 向きの設定
    void SetSize(D3DXVECTOR3 size) { m_size = size; }           // 大きさの設定
    void SetOffSetRot(D3DXVECTOR3 rot) { m_offsetRot = rot; }   // オフセットの設定(向き)
    void SetIdxXFile(int nIdxXFile) { m_nIdxXFile = nIdxXFile; }
    // ゲッター
    D3DXVECTOR3 GetPos(void) { return m_pos; }                  // 位置情報取得(位置)
    D3DXVECTOR3 GetRot(void) { return m_rot; }                  // 位置情報取得(向き)
    D3DXVECTOR3 GetOffSetPos(void) { return m_offsetPos; }      // オフセット情報取得(位置)
    D3DXVECTOR3 GetOffSetRot(void) { return m_offsetRot; }      // オフセット情報取得(向き)
    D3DXVECTOR3 GetSize(void) { return m_size; }
    D3DXMATRIX GetMtxWorld(void);                               // ワールドマトリックス情報の取得
    int GetXModelIdx(void) { return m_nIdxXFile; }
private:
    // ポインタ
    CXModel::XMODEL m_pXModel;  // モデルへのポインタ
    CModel* m_pParent;          // 親のモデルへのポインタ
    // 座標
    D3DXVECTOR3 m_pos;          // 位置
    D3DXVECTOR3 m_offsetPos;    // オフセットの位置
    D3DXVECTOR3 m_rot;          // 向き
    D3DXVECTOR3 m_size;         // 大きさ
    D3DXVECTOR3 m_offsetRot;    // オフセットの向き
    D3DXMATRIX m_mtxWorld;      // ワールドマトリックス
    // 数値
    int m_nIdxXFile;            // 使うモデルの番号
};
#endif