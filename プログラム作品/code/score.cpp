//======================================================================================================================
//
// �X�R�A����[score.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "score.h"
#include "number.h"

#include "game.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define SCORE_TXT			("data/ranking.txt")
#define TXT_LOADS			(20)

#define FLASH_CONT_RANK		(5.0f)

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
int CScore::m_nScore = 0;
CNumber *CScore::m_apNumber[MAX_DIGIT] = {};
CScore::RANKING CScore::m_nRanking[RANKING_NUM] = {};

int CScore::m_nCntFlash = 0;
float CScore::m_fFlash�� = 1.0f / FLASH_CONT_RANK;

// �R���X�g���N�^
CScore::CScore()
{

}

// �f�X�g���N�^
CScore::~CScore()
{

}

//======================================================================================================================
// ����
//======================================================================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int num)
{
	CScore *pScore;

	pScore = new CScore;

	if (num == -1)
	{
		for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
		{
			pScore->m_apNumber[nCnt] = CNumber::Create(pos - D3DXVECTOR3(size.x * nCnt, 0.0f, 0.0f), size);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
		{
			pScore->m_nRanking[num].m_apNumber[nCnt] = CNumber::Create(pos - D3DXVECTOR3(size.x * nCnt, 0.0f, 0.0f), size);
		}		

		Display(num);
	}

	pScore->Init();

	return pScore;
}

//======================================================================================================================
// ������
//======================================================================================================================
void CScore::Init()
{
	m_nCntFlash = 0;
	m_fFlash�� = 1.0f / FLASH_CONT_RANK;

	if (CManager::GetMode() == MODE_GAME)
	{
		m_nScore = 0;
	}
}

//======================================================================================================================
// �I������
//======================================================================================================================
void CScore::Uninit()
{
	if (CManager::GetMode() == MODE_RESULT)
	{
		m_nScore = 0;
	}

	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < RANKING_NUM; nCnt++)
	{
		for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
		{
			if (m_nRanking[nCnt].m_apNumber[nCount])
			{
				m_nRanking[nCnt].m_apNumber[nCount] = NULL;
			}
		}
	}
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CScore::Update()
{

}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CScore::Draw()
{

}

//======================================================================================================================
// ���ёւ��Ăяo�� && ���[�h && �Z�[�u
//======================================================================================================================
void CScore::Sort()
{
	int nScore[TXT_LOADS] = {};

	// ���[�h
	FILE *pFile = fopen(SCORE_TXT, "r");

	if (pFile)
	{
		for (int nCnt = 0; nCnt < TXT_LOADS; nCnt++)
		{
			fscanf(pFile, "%d", &nScore[nCnt]);
		}
		fclose(pFile);
	}

	// ������������Ε��ёւ��Ȃ�
	bool bSame = false;

	for (int nCnt = 0; nCnt < TXT_LOADS; nCnt++)
	{
		if (m_nScore == 0 || nScore[nCnt] == m_nScore)
		{
			bSame = true;
			break;
		}
	}

	if (!bSame)
	{
		// ���ȃX�R�A���
		nScore[TXT_LOADS - 1] = m_nScore;

		QuickSort(nScore, 0, TXT_LOADS - 1);

		// �Z�[�u
		pFile = fopen(SCORE_TXT, "w");

		if (pFile)
		{
			for (int nCnt = 0; nCnt < TXT_LOADS; nCnt++)
			{
				fprintf(pFile, "%d\n\n", nScore[nCnt]);
			}
			fclose(pFile);
		}

	}

	for (int nCnt = 0; nCnt < RANKING_NUM; nCnt++)
	{
		m_nRanking[nCnt].nRankScore = nScore[nCnt];
	}
}

//======================================================================================================================
// �����X�R�A���L��Γ_��
//======================================================================================================================
void CScore::ScoreFlash()
{
	if (m_nScore != 0)
	{
		for (int nCnt = 0; nCnt < RANKING_NUM; nCnt++)
		{
			if (m_nScore == m_nRanking[nCnt].nRankScore)
			{
				for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
				{
					D3DXCOLOR col = m_nRanking[nCnt].m_apNumber[nCount]->GetColor();;

					col.a -= m_fFlash��;

					m_nRanking[nCnt].m_apNumber[nCount]->SetColor(col);
				}

				Display(nCnt);

				break;
			}
		}

		if (++m_nCntFlash == FLASH_CONT_RANK)
		{
			m_nCntFlash = 0;
			m_fFlash�� *= -1;
		}
	}
}

//======================================================================================================================
// �X�R�A����
//======================================================================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	Display();
}

void CScore::AddScore(int nNumber)
{
	m_nScore += nNumber;

	Display();
}

//======================================================================================================================
// �\������
//======================================================================================================================
void CScore::Display(const int num)
{
	if (num == -1)
	{
		for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
		{
			int nDigit = m_nScore % (int)pow(10, nCnt + 1.0f) / (int)pow(10, nCnt);

			m_apNumber[nCnt]->SetNum(nDigit);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
		{
			int nDigit = m_nRanking[num].nRankScore % (int)pow(10, nCnt + 1.0f) / (int)pow(10, nCnt);

			m_nRanking[num].m_apNumber[nCnt]->SetNum(nDigit);
		}
	}
}

//======================================================================================================================
// �N�C�b�N�\�[�g�ɂ����ёւ�����
//======================================================================================================================
void CScore::QuickSort(int nScore[], int nLeft, int nRight)
{
	int	nLeftHold = nLeft;

	int nRightHold = nRight;

	int nQivot = nScore[nLeft];

	while (nLeft < nRight)
	{
		while ((nScore[nRight] <= nQivot) && (nLeft < nRight))
		{
			nRight--;
		}

		if (nLeft != nRight)
		{
			nScore[nLeft] = nScore[nRight];
			nLeft++;
		}

		while ((nScore[nLeft] >= nQivot) && (nLeft < nRight))
		{
			nLeft++;
		}

		if (nLeft != nRight)
		{
			nScore[nRight] = nScore[nLeft];
			nRight--;
		}
	}
	nScore[nLeft] = nQivot;

	nQivot = nLeft;

	nLeft = nLeftHold;
	nRight = nRightHold;

	if (nLeft < nQivot)
	{
		QuickSort(nScore, nLeft, nQivot - 1);
	}
	if (nRight > nQivot)
	{
		QuickSort(nScore, nQivot + 1, nRight);
	}
}
