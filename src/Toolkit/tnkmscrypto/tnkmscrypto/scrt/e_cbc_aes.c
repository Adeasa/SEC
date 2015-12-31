/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_aes.c  AES, CBC mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_AES
#include "../include/scrt.h"
#include "../include/aes.h"
#include <string.h>

static int S_aes_cbc_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_cbc_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_cbc_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);

static void S_aes_cbc_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_cbc_128_aes=
{
	NID_aes_128_cbc, /* nid */
	16,  /* block length */
	16, /* key length */
	16,  /* iv length */
	S_aes_cbc_128_init_key, /* iv, key setting �Լ� */
	S_aes_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_cbc_192_aes=
{
	NID_aes_192_cbc, /* nid */
	16,  /* block length */
	24, /* key length */
	16,  /* iv length */
	S_aes_cbc_192_init_key, /* iv, key setting �Լ� */
	S_aes_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_cbc_256_aes=
{
	NID_aes_256_cbc, /* nid */
	16,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_aes_cbc_256_init_key, /* iv, key setting �Լ� */
	S_aes_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_aes_128_cbc
	Description: SCRT_CIPHER ����ü d_cbc_128_aes ���� (Ű���� 128��Ʈ)
	Parameters
	Return Value : SCRT_CIPHER ����ü d_cbc_128_aes ���� (Ű���� 128��Ʈ)
	Note :
*/
SCRT_CIPHER *SCRT_aes_128_cbc(void)
{
	return(&d_cbc_128_aes);
}
/*	
	Name : SCRT_aes_192_cbc
	Description: SCRT_CIPHER ����ü d_cbc_192_aes ���� (Ű���� 192��Ʈ)
	Parameters
	Return Value : SCRT_CIPHER ����ü d_cbc_192_aes ���� (Ű���� 192��Ʈ)
	Note :
*/
SCRT_CIPHER *SCRT_aes_192_cbc(void)
{
	return(&d_cbc_192_aes);
}

/*	
	Name : SCRT_aes_256_cbc
	Description: SCRT_CIPHER ����ü d_cbc_256_aes ���� (Ű���� 256��Ʈ)
	Parameters
	Return Value : SCRT_CIPHER ����ü d_cbc_256_aes ���� (Ű���� 256��Ʈ)
	Note :
*/
SCRT_CIPHER *SCRT_aes_256_cbc(void)
{
	return(&d_cbc_256_aes);
}

/*	
	Name : S_aes_cbc_128_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� AES CBC ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�(Ű���� 128��Ʈ)
	Parameters
	[out] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] enc : Ű����ü�� ��ȣȭŰ ������ 1, ��ȣȭ Ű������ 0
	Return Value : ���� 1, ���� 0
	Note :
*/
static int S_aes_cbc_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
		ret = S_AES_SET_Decrypt_Key((U8 *)key,  128, &(ctx->c.aes_ks));
	}

	if(ret < 0) return 0;

	return 1;
}

/*	
	Name : S_aes_cbc_192_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� AES CBC ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�(Ű���� 192��Ʈ)
	Parameters
	[out] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] enc : Ű����ü�� ��ȣȭŰ ������ 1, ��ȣȭ Ű������ 0
	Return Value : ���� 1, ���� 0
	Note :
*/
static int S_aes_cbc_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
		ret = S_AES_SET_Decrypt_Key((U8 *)key,  192, &(ctx->c.aes_ks));
	}

	if(ret < 0) return 0;

	return 1;
}

/*	
	Name : S_aes_cbc_256_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� AES CBC ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�(Ű���� 256��Ʈ)
	Parameters
	[out] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] enc : Ű����ü�� ��ȣȭŰ ������ 1, ��ȣȭ Ű������ 0
	Return Value : ���� 1, ���� 0
	Note :
*/
static int S_aes_cbc_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
		ret = S_AES_SET_Decrypt_Key((U8 *)key,  256, &(ctx->c.aes_ks));
	}

	if(ret < 0) return 0;

	return 1;
}

/*	
	Name : S_aes_cbc_cipher
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� AES CBC ��� ��ȣ�˰����� �̿��Ͽ� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : ���� 1, ���� 0
	Note :
*/
static void S_aes_cbc_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if(ctx->encrypt) 
		S_AES_CBC_Encrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv)) ;
	else
		S_AES_CBC_Decrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv)) ;
}

#endif
