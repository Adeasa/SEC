/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ctr_aes.c AES, CTR128 mode

	 Creadted by DEV3

************************************************/
#ifndef NO_AES
#include "../include/scrt.h"
#include "../include/aes.h"
#include <string.h>

static int S_aes_ctr_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_ctr_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_ctr_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);

static void S_aes_ctr_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* AES 128bit CTR ��� �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ctr_128_aes=
{
	NID_aes_128_ctr128, /* nid */
	1,  /* block length */
	16, /* key length */
	16,  /* iv length */
	S_aes_ctr_128_init_key, /* iv, key setting �Լ� */
	S_aes_ctr_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

/* AES 192bit CTR ��� �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ctr_192_aes=
{
	NID_aes_192_ctr128, /* nid */
	1,  /* block length */
	24, /* key length */
	16,  /* iv length */
	S_aes_ctr_192_init_key, /* iv, key setting �Լ� */
	S_aes_ctr_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

/* AES 256bit CTR ��� �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ctr_256_aes=
{
	NID_aes_256_ctr128, /* nid */
	1,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_aes_ctr_256_init_key, /* iv, key setting �Լ� */
	S_aes_ctr_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_aes_128_ctr
	Description: AES 128 bit CTR ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ctr_128_aes ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aes_128_ctr(void)
{
	return(&d_ctr_128_aes);
}

/*	
	Name : SCRT_aes_192_ctr
	Description: AES 192 bit CTR ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ctr_192_aes ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aes_192_ctr(void)
{
	return(&d_ctr_192_aes);
}

/*	
	Name : SCRT_aes_256_ctr
	Description: AES 256 bit CTR ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ctr_256_aes ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aes_256_ctr(void)
{
	return(&d_ctr_256_aes);
}

/*	
	Name : S_aes_ctr_128_init_key
	Description: ��ȣŰ �� ī���͸� �̿��Ͽ� AES 128bit CTR ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : AES ��/��ȣȭ�� ���� AES_KEY_ST Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : ī����
	[in] enc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aes_ctr_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if(enc)
	{
		ret = S_AES_SET_Encrypt_Key((U8 *)key,  128, &(ctx->c.aes_ks));
	}else
	{
		ret = S_AES_SET_Encrypt_Key((U8 *)key,  128, &(ctx->c.aes_ks));
	}

	if(ret < 0) return 0;

	return 1;
}

/*	
	Name : S_aes_ctr_192_init_key
	Description: ��ȣŰ �� ī���͸� �̿��Ͽ� AES 192bit CTR ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� AES_KEY_ST Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : ī����
	[in] enc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aes_ctr_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if(enc)
	{
		ret = S_AES_SET_Encrypt_Key((U8 *)key,  192, &(ctx->c.aes_ks));
	}else
	{
		ret = S_AES_SET_Encrypt_Key((U8 *)key,  192, &(ctx->c.aes_ks));
	}

	if(ret < 0) return 0;

	return 1;
}

/*	
	Name : S_aes_ctr_256_init_key
	Description: ��ȣŰ �� ī���͸� �̿��Ͽ� AES 256bit CTR ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� AES_KEY_ST Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : ī����
	[in] enc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aes_ctr_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if(enc)
	{
		ret = S_AES_SET_Encrypt_Key((U8 *)key,  256, &(ctx->c.aes_ks));
	}else
	{
		ret = S_AES_SET_Encrypt_Key((U8 *)key,  256, &(ctx->c.aes_ks));
	}

	if(ret < 0) return 0;

	return 1;
}

/*	
	Name : S_aes_ctr_cipher
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� AES CTR ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : AES ��/��ȣȭ�� ���� AES_KEY_ST Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_aes_ctr_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{

	int num = 0;
	U8 ecount_buf[AES_BLOCK_SIZE];
	memset(ecount_buf, 0 , AES_BLOCK_SIZE);

	if(ctx->encrypt)
		S_AES_CTR128_Encrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv), ecount_buf, &num) ;
	else
		S_AES_CTR128_Decrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv), ecount_buf, &num) ;

}
#endif
