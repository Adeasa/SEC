
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ofb_lea.c  LEA, OFB mode

	 Creadted by DEV3

************************************************/

#ifndef NO_LEA

#include "../include/scrt.h"
#include "../include/lea.h"
#include <string.h>
#include <math.h>

static int S_lea_ofb128_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_lea_ofb128_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_lea_ofb128_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);

static void S_lea_ofb128_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ofb128_128_lea=
{
	NID_lea_128_ofb128, /* nid */
	1,  /* block length */
	16, /* key length */
	16, /* iv length */
	S_lea_ofb128_128_init_key, /* iv, key setting �Լ� */
	S_lea_ofb128_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

static SCRT_CIPHER d_ofb128_192_lea=
{
	NID_lea_192_ofb128, /* nid */
	1,  /* block length */
	24, /* key length */
	16, /* iv length */
	S_lea_ofb128_192_init_key, /* iv, key setting �Լ� */
	S_lea_ofb128_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

static SCRT_CIPHER d_ofb128_256_lea=
{
	NID_lea_256_ofb128, /* nid */
	1,  /* block length */
	32, /* key length */
	16, /* iv length */
	S_lea_ofb128_256_init_key, /* iv, key setting �Լ� */
	S_lea_ofb128_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

/*
	Name : SCRT_lea_128_ofb128
 	Description : ARIA 128bit OFB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�.
 	Parameters
 	Return Value : d_ofb128_128_lea ����ü�� ��ȯ�Ѵ�
 	Note :
 */
SCRT_CIPHER *SCRT_lea_128_ofb128(void)
{
	return(&d_ofb128_128_lea);
}

/*
	Name : SCRT_lea_192_ofb128
 	Description : ARIA 192 bit OFB��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�.
 	Parameters
 	Return Value : d_ofb128_192_lea ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_lea_192_ofb128(void)
{
	return(&d_ofb128_192_lea);
}

/*
	Name : SCRT_lea_256_ofb128
 	Description : ARIA 256 bit OFB��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�.
 	Parameters
 	Return Value : d_ofb128_256_lea ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_lea_256_ofb128(void)
{
	return(&d_ofb128_256_lea);
}

/*
	Name : S_lea_ofb128_128_init_key
 	Description : ��ȣŰ�� �̿��Ͽ� LEA 128bit OFB��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
 	Parameters
	[out] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ�ȭ ����
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ, etc : ��ȣȭ)
 	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0
 	Note :
 */
static int S_lea_ofb128_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 16); 
		//ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 128);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 16); 
		//ret = S_lea_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 128);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*
	Name : S_lea_ofb128_192_init_key
	Description : ��ȣŰ�� �̿��Ͽ� LEA 192bit OFB��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
	Parameters
	[out] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ�ȭ ����
	[in] isenc : ��/��ȣȭ FLAG ( 1 : ��ȣȭ, etc : ��ȣȭ)
 	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 
 	Note :
 */
static int S_lea_ofb128_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 24);
		//ret = S_lea_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 192);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 24);
		//ret = S_lea_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 192);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*
	Name : S_lea_ofb128_256_init_key
	Description : ��ȣŰ�� �̿��Ͽ� LEA 256bit OFB��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
	Parameters
	[out] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ  FLAG ( 1 : ��ȣȭ, etc : ��ȣȭ)
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 
 	Note :
 */
static int S_lea_ofb128_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 32);
		//ret = S_lea_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 256);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 32);
		//ret = S_lea_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 256);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*
	Name : S_lea_ofb128_cipher
	Description : ������ ��ȣŰ �� �ʱ⺤�Ϳ� LEA OFB ��� ��ȣ �˰����� �̿��Ͽ� ��/��ȣȭ �Ѵ�.
	Parameters
	[in] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ ��� ������
	[in] inl : ��/��ȣȭ ��� �������� ���� 
 	Return Value :
 	Note :
 */
static void S_lea_ofb128_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if (ctx->encrypt)
		S_LEA_OFB128_Encrypt(&(ctx->c.lea_ks), (U8 *)&(ctx->iv), in, inl, out);
		//S_ARIA_OFB128_Encrypt(&(ctx->c.lea_ks), (U8 *)&(ctx->iv), in, inl, out, &(ctx->num));
	else
		S_LEA_OFB128_Decrypt(&(ctx->c.lea_ks), (U8 *)&(ctx->iv), in, inl, out);
		//S_ARIA_OFB128_Decrypt(&(ctx->c.lea_ks), (U8 *)&(ctx->iv), in, inl, out, &(ctx->num));
}
#endif
