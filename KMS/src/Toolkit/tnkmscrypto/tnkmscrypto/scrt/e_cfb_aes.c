/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cbc_aes.c AES, CFB mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_AES
#include "../include/scrt.h"
#include "../include/aes.h"
#include <string.h>
#include <math.h>

static int S_aes_cfb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_cfb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_cfb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);

static void S_aes_cfb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_cfb_128_aes=
{
	NID_aes_128_cfb128, /* nid */
	1,  /* block length */
	16, /* key length */
	16,  /* iv length */
	S_aes_cfb_128_init_key, /* iv, key setting �Լ� */
	S_aes_cfb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_cfb_192_aes=
{
	NID_aes_192_cfb128, /* nid */
	1,  /* block length */
	24, /* key length */
	16,  /* iv length */
	S_aes_cfb_192_init_key, /* iv, key setting �Լ� */
	S_aes_cfb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_cfb_256_aes=
{
	NID_aes_256_cfb128, /* nid */
	1,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_aes_cfb_256_init_key, /* iv, key setting �Լ� */
	S_aes_cfb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_aes_128_cfb
	Description: 128��Ʈ Ű�� ����ϴ� AES �˰����� CFB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : AES �˰����� CFB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_aes_128_cfb(void)
{
	return(&d_cfb_128_aes);
}

/*	
	Name : SCRT_aes_192_cfb
	Description: 192��Ʈ Ű�� ����ϴ� AES �˰����� CFB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : AES �˰����� CFB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_aes_192_cfb(void)
{
	return(&d_cfb_192_aes);
}

/*	
	Name : SCRT_aes_256_cfb
	Description: 256��Ʈ Ű�� ����ϴ� AES �˰����� CFB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : AES �˰����� CFB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_aes_256_cfb(void)
{
	return(&d_cfb_256_aes);
}

/*	
	Name : S_aes_cfb_128_init_key
	Description: 128��Ʈ Ű�� ����ϴ� AES �˰����� CFB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	[in] enc : ��ȣȭ���� ��ȣȭ���� ����. 1�̸� ��ȣȭ. 0�̸� ��ȣȭ
	Return Value : �ʱ�ȭ�� �����ϸ� 1
	               �ʱ�ȭ�� �����ϸ� 0
	Note : 
*/	
static int S_aes_cfb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
	Name : S_aes_cfb_192_init_key
	Description: 192��Ʈ Ű�� ����ϴ� AES �˰����� CFB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	[in] enc : ��ȣȭ���� ��ȣȭ���� ����. 1�̸� ��ȣȭ. 0�̸� ��ȣȭ
	Return Value : �ʱ�ȭ�� �����ϸ� 1
	               �ʱ�ȭ�� �����ϸ� 0
	Note : 
*/
static int S_aes_cfb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
	Name : S_aes_cfb_256_init_key
	Description: 256��Ʈ Ű�� ����ϴ� AES �˰����� CFB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	[in] enc : ��ȣȭ���� ��ȣȭ���� ����. 1�̸� ��ȣȭ. 0�̸� ��ȣȭ
	Return Value : �ʱ�ȭ�� �����ϸ� 1
	               �ʱ�ȭ�� �����ϸ� 0
	Note : 
*/
static int S_aes_cfb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
	Name : S_aes_cfb_cipher
	Description: AES �˰����� CFB ���忡�� �����͸� ��/��ȣȭ�Ѵ�.
	Parameters
	[in/out] ctx : ��ȣŰ�� ����Ǿ� �ִ� ���ؽ�Ʈ
	[out] out : ��ȣȭ �����
	[in] in : ��ȣȭ ��� ������
	[in] inl : ��ȣȭ ��� �������� ����
	Return Value :
	Note : 
*/
static void S_aes_cfb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if(ctx->encrypt) 
	{
		int num =0;

		S_AES_CFB128_Encrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv), &num) ;
	}
	else
	{
		int num =0;

		S_AES_CFB128_Decrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv), &num) ;
	}

}
#endif
