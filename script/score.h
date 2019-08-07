//======================================================================================================================
//
// 処理[score.h]
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

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNum);

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetScore(int nScore);
	void AddScore(int nNumber);

	int GetScore() { return m_nScore[6]; }

private:
	void Display(int Cnt);
	void QuickSort(int nScore[], int nLeft, int nRight);

	CNumber *m_apNumber[MAX_DIGIT];

	static int m_nScore[];
};

#endif