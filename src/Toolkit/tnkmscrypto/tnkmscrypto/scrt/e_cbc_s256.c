/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cbc_s256.c SEED256, CBC mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_SEED
#include "../include/scrt.h"
#include "../include/seed256.h"
#include <stdio.h>
#include <string.h>

static void S_seed256_cbc_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key,unsigned char *iv);
static void S_seed256_cbc_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl);

static SCRT_CIPHER s256_cbc_cipher=
{
	NID_seed256_cbc,
	16,32,16,
	S_seed256_cbc_init_key,
	S_seed256_cbc_cipher,
	NULL,
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed256_ks))
};

/*	
	Name : SCRT_seed256_cbc
	Description: SEED CBC ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : s_cbc_cipher ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_seed256_cbc(void)
{
	return(&s256_cbc_cipher);
}

/*	
	Name : S_seed256_cbc_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� SEED CBC ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	Return Value : 
	Note :
*/	
static void S_seed256_cbc_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv)
{
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);
	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);
	S_SEED256_KeySchedule(&(ctx->c.seed256_ks),key);
}

/*	
	Name : S_seed256_cbc_cipher
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� SEED CBC ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_seed256_cbc_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl)
{
	if(ctx->encrypt)
		S_SEED256_CBC_Encrypt(in,out,&(ctx->c.seed256_ks),inl,(U8 *)&(ctx->iv));
	else
		S_SEED256_CBC_Decrypt(in,out,&(ctx->c.seed256_ks),inl,(U8 *)&(ctx->iv));
}
#endif
