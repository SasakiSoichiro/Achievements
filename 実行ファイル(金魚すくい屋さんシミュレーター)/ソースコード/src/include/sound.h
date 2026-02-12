//***************************************
//
// サウンド処理 [sound.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _SOUND_H_// このマクロ定義がされていなかったら
#define _SOUND_H_// 二重インクルード防止のマクロ定義

//*********************
// インクルード
//*********************
#include "main.h"

//******************
// サウンドクラス
//******************
class CSound
{
public:
	// ラベル
	typedef enum
	{
		LABEL_BGM00 = 0,
		LABEL_SE00,
		LABEL_SE01,
		LABEL_SE02,
		LABEL_SE03,
		LABEL_SE04,
		LABEL_SE05,
		LABEL_SE06,
		LABEL_SE07,
		LABEL_SE08,
		LABEL_MAX,
	} LABEL;
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	CSound();	// コンストラクタ
	~CSound();	// デストラクタ
	HRESULT Init(HWND hWnd);	// 初期化
	void Uninit(void);			// 終了
	HRESULT Play(LABEL label);	// 再生
	void Stop(LABEL label);		// 停止
	void Stop(void);			// すべて停止
private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	IXAudio2* m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];			// サウンドの情報
};

#endif
