/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cbc_s.c SEED, CBC mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_SEED

#include "../include/scrt.h"
#include "../include/seed.h"
#include <stdio.h>
#include <string.h>

static void S_seed_cbc_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key,unsigned char *iv);
static void S_seed_cbc_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl);

static SCRT_CIPHER s_cbc_cipher=
{
	NID_seed_cbc,
	16,16,16,
	S_seed_cbc_init_key,
	S_seed_cbc_cipher,
	NULL,
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed_ks))
};


/*	
	Name : SCRT_seed_cbc
	Description: SEED CBC ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : s_cbc_cipher ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_seed_cbc(void)
{
	return(&s_cbc_cipher);
}

/*	
	Name : S_seed_cbc_init_key
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� SEED CBC ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	Return Value : 
	Note :
*/	
static void S_seed_cbc_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv)
{
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);
	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);
	S_SEED_KeySchedule(&(ctx->c.seed_ks),key);
}

/*	
	Name : S_seed_cbc_cipher
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� SEED CBC ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_seed_cbc_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl)
{
	if(ctx->encrypt)
		S_SEED_CBC_Encrypt(in,out,&(ctx->c.seed_ks),inl,(U8 *)&(ctx->iv));
	else
		S_SEED_CBC_Decrypt(in,out,&(ctx->c.seed_ks),inl,(U8 *)&(ctx->iv));
}

#endif
