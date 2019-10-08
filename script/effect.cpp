//======================================================================================================================
//
// ����[effect.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "effect.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define BULLET_LIFE			(10)

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = {};
LPDIRECT3DVERTEXBUFFER9 CEffect::m_pVtxBuff = {};

CScene *CEffect::m_pEffectScene[MAX_EFFECT] = {};
int CEffect::m_nNumAll = 0;

// �R���X�g���N�^
CEffect::CEffect() : CScene2D::CScene2D(OBJTYPE_EFFECT, OBJECT_EFFECT)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (!m_pEffectScene[nCnt])
		{
			m_pEffectScene[nCnt] = this;

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
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
}

// �f�X�g���N�^
CEffect::~CEffect()
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

HRESULT CEffect::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_00.jpg", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CEffect::Unload()
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
CEffect *CEffect::Create(D3DXVECTOR3 pos, EFFECTTYPE type)
{
	if (MAX_EFFECT <= m_nNumAll)
		return NULL;

	CEffect *pEffect;

	pEffect = new CEffect;

	pEffect->SetPos(pos);
	pEffect->m_type = type;
	pEffect->Init();
	pEffect->BindTexture(NULL);

	return pEffect;
}

//======================================================================================================================
// ������
//======================================================================================================================
void CEffect::Init()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = BULLET_LIFE;

	CScene2D::Init();

	switch (m_type)
	{
	case CEffect::EFFECTTYPE_NONE:

		SetSize(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

		SetCollar(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		break;

	case CEffect::EFFECTTYPE_LASER:

		SetSize(D3DXVECTOR3(15.0f, 15.0f, 0.0f));

		SetCollar(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		break;
	}
}

//======================================================================================================================
// �J������
//======================================================================================================================
void CEffect::Uninit()
{
	CScene2D::Uninit();

	this->Release(m_pEffectScene);
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CEffect::Update()
{
	switch (m_type)
	{
	case CEffect::EFFECTTYPE_NONE:

		SetSize(GetSize() + D3DXVECTOR3(-0.7f, -0.7f, 0.0f));

		if (GetSize().x < 0.1f || GetSize().y < 0.1f)
		{
			this->Uninit();
		}

		break;

	case CEffect::EFFECTTYPE_LASER:

		if (--m_nLife <= 0)
		{
			this->Uninit();
		}

		break;
	}
}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CEffect::Draw()
{

}

void CEffect::DrawAll()
{
	if (!m_pVtxBuff)
	{
		return;
	}

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++, pVtx += 4)
	{
		if (m_pEffectScene[nCnt])
		{
			m_pEffectScene[nCnt]->Vertex(pVtx);
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

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��							//JPG�p
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				// ���f�B�X�e�B�l�C�V�����J���[�̎w�� 

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (m_pEffectScene[nCnt])
		{
			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DBLENDOP_ADD);	// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��						//�e�N�X�`���w�i�Ȃ��R�s
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�B�X�e�B�l�C�V�����J���[�̎w��
}
