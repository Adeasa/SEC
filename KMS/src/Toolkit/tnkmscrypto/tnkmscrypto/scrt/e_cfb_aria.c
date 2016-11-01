
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cbc_aria.c  ARIA, CBC mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_ARIA

#include "../include/scrt.h"
#include "../include/aria.h"
#include <string.h>
#include <math.h>

static int S_aria_cfb128_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_aria_cfb128_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_aria_cfb128_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);

static void S_aria_cfb128_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* ARIA 128bit CFB128 ��� �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_cfb128_128_aria=
{
	NID_aria_128_cfb128, /* nid */
	1,  /* block length */
	16, /* key length */
	16,  /* iv length */
	S_aria_cfb128_128_init_key, /* iv, key setting �Լ� */
	S_aria_cfb128_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

/* ARIA 192bit CFB128 ��� �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_cfb128_192_aria=
{
	NID_aria_192_cfb128, /* nid */
	1,  /* block length */
	24, /* key length */
	16,  /* iv length */
	S_aria_cfb128_192_init_key, /* iv, key setting �Լ� */
	S_aria_cfb128_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

/* ARIA 256bit CFB128 ��� �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_cfb128_256_aria=
{
	NID_aria_256_cfb128, /* nid */
	1,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_aria_cfb128_256_init_key, /* iv, key setting �Լ� */
	S_aria_cfb128_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_aria_128_cfb128
	Description: ARIA 128 bit CFB128 ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_cfb128_128_aria ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aria_128_cfb128(void)
{
	return(&d_cfb128_128_aria);
}

/*	
	Name : SCRT_aria_192_cfb128
	Description: ARIA 192 bit CFB128 ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_cfb128_192_aria ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aria_192_cfb128(void)
{
	return(&d_cfb128_192_aria);
}

/*	
	Name : SCRT_aria_256_cfb128
	Description: ARIA 256 bit CFB128 ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_cfb128_256_aria ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aria_256_cfb128(void)
{
	return(&d_cfb128_256_aria);
}

/*	
	Name : S_aria_cfb128_128_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 128bit CFB128 ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aria_cfb128_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 128);
	else
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 128);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_aria_cfb128_192_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 192bit CFB128 ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aria_cfb128_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 192);
	else
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 192);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_aria_cfb128_256_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 256bit CFB128 ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aria_cfb128_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 256);
	else
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 256);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_aria_cfb128_cipher
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA CFB128 ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_aria_cfb128_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if (ctx->encrypt) 
		S_ARIA_CFB128_Encrypt(&(ctx->c.aria_ks), (U8 *)&(ctx->iv), in, inl, out, &(ctx->num));
	else
		S_ARIA_CFB128_Decrypt(&(ctx->c.aria_ks), (U8 *)&(ctx->iv), in, inl, out, &(ctx->num));
}
#endif
