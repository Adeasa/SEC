/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ctr_ofb.c AES, OFB128 mode

	 Creadted by DEV3

************************************************/

#ifndef NO_AES
#include "../include/scrt.h"
#include "../include/aes.h"
#include <string.h>
#include <math.h>

static int S_aes_ofb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_ofb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_ofb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);

static void S_aes_ofb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ofb_128_aes=
{
	NID_aes_128_ofb128, /* nid */
	1,  /* block length */
	16, /* key length */
	16,  /* iv length */
	S_aes_ofb_128_init_key, /* iv, key setting �Լ� */
	S_aes_ofb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_ofb_192_aes=
{
	NID_aes_192_ofb128, /* nid */
	1,  /* block length */
	24, /* key length */
	16,  /* iv length */
	S_aes_ofb_192_init_key, /* iv, key setting �Լ� */
	S_aes_ofb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_ofb_256_aes=
{
	NID_aes_256_ofb128, /* nid */
	1,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_aes_ofb_256_init_key, /* iv, key setting �Լ� */
	S_aes_ofb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_aes_128_ofb
	Description: AES 128bit OFB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ofb_128_aes ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aes_128_ofb(void)
{
	return(&d_ofb_128_aes);
}

/*	
	Name : SCRT_aes_192_ofb
	Description: AES 192bit OFB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ofb_192_aes ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aes_192_ofb(void)
{
	return(&d_ofb_192_aes);
}

/*	
	Name : SCRT_aes_256_ofb
	Description: AES 256bit OFB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ofb_256_aes ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aes_256_ofb(void)
{
	return(&d_ofb_256_aes);
}

/*	
	Name : S_aes_ofb_128_init_key
	Description: ��ȣŰ�� �̿��Ͽ� AES 128bit OFB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ǹ̾���
	[in] enc : ��ȣȭ 1, ��ȣȭ 0
	Return Value : 
	Note :
*/	
static int S_aes_ofb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
	Name : S_aes_ofb_192_init_key
	Description: ��ȣŰ�� �̿��Ͽ� AES 192bit OFB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ǹ̾���
	[in] enc : ��ȣȭ 1, ��ȣȭ 0
	Return Value : 
	Note :
*/	
static int S_aes_ofb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
	Name : S_aes_ofb_256_init_key
	Description: ��ȣŰ�� �̿��Ͽ� AES 256bit OFB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ǹ̾���
	[in] enc : ��ȣȭ 1, ��ȣȭ 0
	Return Value : 
	Note :
*/	
static int S_aes_ofb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
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
	Name : S_aes_ofb_cipher
	Description: ��ȣŰ�� �̿��Ͽ� AES OFB ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : AES ��/��ȣȭ�� ���� AES_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_aes_ofb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if(ctx->encrypt) 
	{
		int num =0;

		S_AES_OFB128_Encrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv), &num) ;
	}
	else
	{
		int num =0;

		S_AES_OFB128_Decrypt(in, out, (long)inl, &(ctx->c.aes_ks), (U8 *)&(ctx->iv), &num) ;
	}

}
#endif
