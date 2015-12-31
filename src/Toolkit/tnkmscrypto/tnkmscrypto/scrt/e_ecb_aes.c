/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ecb_aes.c AES, ECB mode

	 Creadted by DEV3

************************************************/

#ifndef NO_AES
#include "../include/scrt.h"
#include "../include/aes.h"
#include <string.h>
#include <math.h>

static int S_aes_ecb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_ecb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);
static int S_aes_ecb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc);

static void S_aes_ecb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ecb_128_aes=
{
	NID_aes_128_ecb, /* nid */
	16,  /* block length */
	16, /* key length */
	0,  /* iv length */
	S_aes_ecb_128_init_key, /* iv, key setting �Լ� */
	S_aes_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_ecb_192_aes=
{
	NID_aes_192_ecb, /* nid */
	16,  /* block length */
	24, /* key length */
	0,  /* iv length */
	S_aes_ecb_192_init_key, /* iv, key setting �Լ� */
	S_aes_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

static SCRT_CIPHER d_ecb_256_aes=
{
	NID_aes_256_ecb, /* nid */
	16,  /* block length */
	32, /* key length */
	0,  /* iv length */
	S_aes_ecb_256_init_key, /* iv, key setting �Լ� */
	S_aes_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aes_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_aes_128_ecb
	Description: 128��Ʈ Ű�� ����ϴ� AES �˰����� ECB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : AES �˰����� ECB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_aes_128_ecb(void)
{
	return(&d_ecb_128_aes);
}

/*	
	Name : SCRT_aes_192_ecb
	Description: 192��Ʈ Ű�� ����ϴ� AES �˰����� ECB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : AES �˰����� ECB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_aes_192_ecb(void)
{
	return(&d_ecb_192_aes);
}

/*	
	Name : SCRT_aes_256_ecb
	Description: 256��Ʈ Ű�� ����ϴ� AES �˰����� ECB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : AES �˰����� ECB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_aes_256_ecb(void)
{
	return(&d_ecb_256_aes);
}

/*	
	Name : S_aes_ecb_128_init_key
	Description: 128��Ʈ Ű�� ����ϴ� AES �˰����� ECB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	[in] enc : ��ȣȭ���� ��ȣȭ���� ����. 1�̸� ��ȣȭ. 0�̸� ��ȣȭ
	Return Value : �ʱ�ȭ�� �����ϸ� 1
	               �ʱ�ȭ�� �����ϸ� 0
	Note : 
*/	
static int S_aes_ecb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
{
	int ret ;
	/*
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  
	*/
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
	Name : S_aes_ecb_192_init_key
	Description: 192��Ʈ Ű�� ����ϴ� AES �˰����� ECB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	[in] enc : ��ȣȭ���� ��ȣȭ���� ����. 1�̸� ��ȣȭ. 0�̸� ��ȣȭ
	Return Value : �ʱ�ȭ�� �����ϸ� 1
	               �ʱ�ȭ�� �����ϸ� 0
	Note : 
*/	
static int S_aes_ecb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
{
	int ret ;
	/*
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  
	*/
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
	Name : S_aes_ecb_256_init_key
	Description: 256��Ʈ Ű�� ����ϴ� AES �˰����� ECB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	[in] enc : ��ȣȭ���� ��ȣȭ���� ����. 1�̸� ��ȣȭ. 0�̸� ��ȣȭ
	Return Value : �ʱ�ȭ�� �����ϸ� 1
	               �ʱ�ȭ�� �����ϸ� 0
	Note : 
*/	
static int S_aes_ecb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int enc)
{
	int ret ;
	/*
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  
	*/
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
	Name : S_aes_ecb_cipher
	Description: AES �˰����� ECB ���忡�� �����͸� ��/��ȣȭ�Ѵ�.
	Parameters
	[in/out] ctx : ��ȣŰ�� ����Ǿ� �ִ� ���ؽ�Ʈ
	[out] out : ��ȣȭ �����
	[in] in : ��ȣȭ ��� ������
	[in] inl : ��ȣȭ ��� �������� ����
	Return Value :
	Note : 
*/
static void S_aes_ecb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	int nb = inl >> 4;

	if(ctx->encrypt) 
	{
		while(nb--)
		{
			S_AES_ECB_Encrypt(in, out, &(ctx->c.aes_ks)) ;
			in += AES_BLOCK_SIZE;
            out += AES_BLOCK_SIZE;
		}
	}
	else
	{
		while(nb--)
		{
			S_AES_ECB_Decrypt(in, out, &(ctx->c.aes_ks)) ;
			in += AES_BLOCK_SIZE;
			out += AES_BLOCK_SIZE;
		}
	}

}
#endif