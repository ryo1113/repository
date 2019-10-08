//======================================================================================================================
//
// スコア処理[score.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"

class CNumber;

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define	MAX_DIGIT		(8)

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CScore : public CScene
{
public:
	CScore();
	~CScore();
	void Vertex(VERTEX_2D *pVtx) {}

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int num = -1);

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Sort();
	static void ScoreFlash();

	void SetScore(int nScore);
	void AddScore(int nNumber);

	static CNumber *GetScore() { return m_apNumber[0]; }

private:
	static void Display(const int num = -1);
	static void QuickSort(int nScore[], int nLeft, int nRight);

	static CNumber *m_apNumber[MAX_DIGIT];

	static int m_nScore;

	struct RANKING
	{
		CNumber *m_apNumber[MAX_DIGIT];
		int nRankScore;
	};
	
	static RANKING m_nRanking[RANKING_NUM];

	static float m_fFlashα;
	static int m_nCntFlash;
};

#endif