
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ecb_s.c SEED, ECB mode

	 Creadted by DEV3

************************************************/

#ifndef NO_SEED

#include "../include/scrt.h"
#include "../include/seed.h"
#include <stdio.h>
#include <string.h>

static void S_seed_ecb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv);
static void S_seed_ecb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out,unsigned char *in, unsigned int inl);


static SCRT_CIPHER s_ecb_cipher=
{
	NID_seed_ecb,
	16,16,0,
	S_seed_ecb_init_key,
	S_seed_ecb_cipher,
	NULL,
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed_ks))
};

/*	
	Name : SCRT_seed_ecb
	Description: SEED �˰����� ECB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : SEED �˰��� ECB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_seed_ecb(void)
{
	return(&s_ecb_cipher);
}

/*	
	Name : S_seed_ecb_init_key
	Description: SEED �˰����� ECB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	Return Value : 
	Note : 
*/		
static void S_seed_ecb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv)
{
		S_SEED_KeySchedule(&(ctx->c.seed_ks),key);
}

/*	
	Name : S_seed_ecb_cipher
	Description: SEED �˰����� ECB ����� ��ȣȭ�� �����Ѵ�.
	Parameters
	[in] ctx : Ű ����ü���� �����ϰ� �ִ� ���ؽ�Ʈ
	[out] out : ��ȣȭ �����
	[in] in : ��ȣȭ ��� ������
	[in] inl : ��ȣȭ ��� ������ ����
	Return Value : 
	Note : 
*/	
static void S_seed_ecb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl)
{
		if(ctx->encrypt)
			S_SEED_ECB_Encrypt(in,out,&(ctx->c.seed_ks),inl);
		else
			S_SEED_ECB_Decrypt(in,out,&(ctx->c.seed_ks),inl);
}



#endif
