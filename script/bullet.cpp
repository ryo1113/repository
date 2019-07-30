//======================================================================================================================
//
// ����[bullet.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "bullet.h"
#include "effect.h"
#include "explosion.h"

#include "enemy.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

//======================================================================================================================
// �v���g�^�C�v�錾
//======================================================================================================================

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = {};
LPDIRECT3DVERTEXBUFFER9 CBullet::m_pVtxBuff = {};

CScene *CBullet::m_pBulletScene[MAX_BULLET] = {};
int CBullet::m_nNumAll = 0;

// �R���X�g���N�^
CBullet::CBullet() : CScene2D::CScene2D(OBJTYPE_BULLET, OBJECT_BULLET)
{
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (!m_pBulletScene[nCnt])
		{
			m_pBulletScene[nCnt] = this;

			m_nID = nCnt;
			this->m_nDrawIdx = nCnt;

			m_nNumAll++;
			break;
		}
	}

	if (!m_pVtxBuff)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
}

// �f�X�g���N�^
CBullet::~CBullet()
{
	if (m_nNumAll == 1)
	{
		//���_�o�b�t�@�̊J��
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}
	m_nNumAll--;
}

HRESULT CBullet::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet_00.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CBullet::Unload()
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//======================================================================================================================
// ����
//======================================================================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	if (!GetShotNum())
		return NULL;

	CBullet *pBullet;

	pBullet = new CBullet;

	pBullet->m_move = move;
	pBullet->SetPos(pos);
	pBullet->Init();
	pBullet->BindTexture(m_pTexture);

	return pBullet;
}

//======================================================================================================================
// ������
//======================================================================================================================
void CBullet::Init()
{
	m_nCntLife = 120;

	SetSize(D3DXVECTOR3(30.0f, 30.0f, 0.0f));

	CScene2D::Init();

	SetTextureAnimationTex(1, 4);
}

//======================================================================================================================
// �J������
//======================================================================================================================
void CBullet::Uninit()
{
	CScene2D::Uninit();

	this->Release(m_pBulletScene);
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CBullet::Update()
{
	MoveSpeed();

	SetTextureAnimation(5, true);

	// �G�t�F�N�g����
	CEffect::Create(GetPos());


	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		if (!pScene)
			continue;

		CEnemy *pEnemy = (CEnemy*)pScene;

		if (HitShapeCollision(pEnemy))
		{
			pEnemy->HitEnemy();

			this->Uninit();
		}
	}

	if (--m_nCntLife == 0 || UnlookJudge())
	{
		if (m_nCntLife == 0)
		{
			// ��������
			CExplosion::Create(GetPos());
		}

		this->Uninit();
	}
}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CBullet::Draw()
{

}

void CBullet::DrawAll()
{
	if (!m_pVtxBuff)
	{
		return;
	}

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++, pVtx += 4)
	{
		if (m_pBulletScene[nCnt])
		{
			m_pBulletScene[nCnt]->Vertex(pVtx);
		}
	}

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (m_pBulletScene[nCnt])
		{
			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

bool CBullet::GetShotNum()
{
	if (MAX_BULLET > m_nNumAll)
		return true;

	return false;
}

//======================================================================================================================
// �ړ�����
//======================================================================================================================
void CBullet::MoveSpeed()
{
	D3DXVECTOR3 pos = GetPos();

	pos += m_move;

	SetPos(pos);
}
