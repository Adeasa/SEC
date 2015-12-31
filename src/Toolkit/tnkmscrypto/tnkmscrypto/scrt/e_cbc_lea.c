
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cbc_lea.c  LEA, CBC mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_ARIA

#include "../include/scrt.h"
#include "../include/lea.h"
#include <string.h>

static int S_lea_cbc_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_lea_cbc_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_lea_cbc_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);

static void S_lea_cbc_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_cbc_128_lea=
{
	NID_lea_128_cbc, /* nid */
	16,  /* block length */
	16,  /* key length */
	16,  /* iv length */
	S_lea_cbc_128_init_key, /* iv, key setting �Լ� */
	S_lea_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

static SCRT_CIPHER d_cbc_192_lea=
{
	NID_lea_192_cbc, /* nid */
	16,  /* block length */
	24, /* key length */
	16,  /* iv length */
	S_lea_cbc_192_init_key, /* iv, key setting �Լ� */
	S_lea_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

static SCRT_CIPHER d_cbc_256_lea=
{
	NID_lea_256_cbc, /* nid */
	16,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_lea_cbc_256_init_key, /* iv, key setting �Լ� */
	S_lea_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

/*
	Name :  SCRT_lea_128_cbc
 	Description : LEA CBC 128bit ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters
 	Return Value : d_cbc_128_lea ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_lea_128_cbc(void)
{
	return(&d_cbc_128_lea);
}

/*
	Name : SCRT_lea_192_cbc
 	Description : LEA CBC 192bit ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters 
 	Return Value : d_cbc_192_lea ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_lea_192_cbc(void)
{
	return(&d_cbc_192_lea);
}

/*
	Name : SCRT_lea_256_cbc
 	Description : LEA CBC 256bit ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters 
 	Return Value : d_cbc_256_lea ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_lea_256_cbc(void)
{
	return(&d_cbc_256_lea);
}

/*
	Name : S_lea_cbc_128_init_key
 	Description : ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 128bit CBC ��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
 	Parameters
 	[out] ctx : ARIA ��/��ȣȭ�� ���� Ű ����ü �� ������ ���� ����ü.
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ FLAG (1:enc, other : dec)
 	Return Value : �����Ͽ��� ��� 1, ���� �Ͽ��� ��� 0
 	Note :
 */
static int S_lea_cbc_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 16);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 16);

	if (ret < 0) return 0;

	return 1;
}

/*
	Name : S_aria_cbc_192_init_key
 	Description : ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 192bit CBC ��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
 	Parameters
 	[out] ctx : ARIA ��/��ȣȭ�� ���� Ű ����ü �� ������ ���� ����ü.
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ FLAG (1:enc, other : dec)
 	Return Value : �����Ͽ��� ��� 1, ���� �Ͽ��� ��� 0
 	Note :
 */
static int S_lea_cbc_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 24);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 24);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*
	Name : S_lea_cbc_256_init_key
	Description : ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� LEA 256bit CBC ��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
	Parameters
	[out] ctx : LEA ��/��ȣȭ�� ���� Ű ����ü �� ������ ���� ����ü.
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ ���� FLAG (1:enc, other : dec)
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0
 	Note :
 */
static int S_lea_cbc_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 32);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 32);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*
	Name : S_aria_cbc_cipher
 	Description : ��ȣŰ �� �ʱ⺤�� ������ ARIA CBC ��� ��ȣ �˰����� �̿��Ͽ� ��/��ȣȭ �Ѵ�.
 	Parameters
 	[in] ctx : ARIA ��/��ȣȭ�� ���� Ű����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ ��� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
 	Return Value :
 	Note :
 */
static void S_lea_cbc_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if (ctx->encrypt) 
		S_LEA_CBC_Encrypt(&(ctx->c.lea_ks), (U8 *)&(ctx->iv), in, inl, out);
	else
		S_LEA_CBC_Decrypt(&(ctx->c.lea_ks), (U8 *)&(ctx->iv), in, inl, out);
}

#endif
