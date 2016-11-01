/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ctr_s256.c SEED256, CTR128 mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_SEED
#include "../include/scrt.h"
#include "../include/seed256.h"
#include <string.h>

static int S_seed256_ctr_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static void S_seed256_ctr_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d256_ctr_128_seed=
{
	NID_seed256_ctr128, /* nid */
	1,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_seed256_ctr_128_init_key, /* iv, key setting �Լ� */
	S_seed256_ctr_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed256_ks))
};

/*	
	Name : SCRT_seed256_ctr
	Description: SEED 256 bit CTR ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ctr_128_seed ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_seed256_ctr(void)
{
	return(&d256_ctr_128_seed);
}


/*	
	Name : S_seed256_ctr_128_init_key
	Description: ��ȣŰ �� ī���͸� �̿��Ͽ� SEED CTR ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : ī����
	[in] enc : �ǹ̾���
	Return Value : ���� 1 
	Note :
*/
static int S_seed256_ctr_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
{
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	S_SEED256_KeySchedule(&(ctx->c.seed256_ks),key);

	return 1;
}

/*	
	Name : S_seed256_ctr_cipher
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� SEED CTR ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_seed256_ctr_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{

	int num = 0;
	U8 ecount_buf[AES_BLOCK_SIZE];
	memset(ecount_buf, 0 , AES_BLOCK_SIZE);

	if(ctx->encrypt)
		S_SEED256_CTR128_Encrypt(in, out, (long)inl, &(ctx->c.seed256_ks), (U8 *)&(ctx->iv), ecount_buf, &num) ;
	else
		S_SEED256_CTR128_Decrypt(in, out, (long)inl, &(ctx->c.seed256_ks), (U8 *)&(ctx->iv), ecount_buf, &num) ;
}
#endif
