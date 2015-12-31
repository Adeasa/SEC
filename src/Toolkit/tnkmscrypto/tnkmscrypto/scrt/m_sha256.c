
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     m_sha256.c

	 Creadted by DEV3

************************************************/

#ifndef NO_SHA256

#include "../include/scrt.h"
#include <stdio.h>

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_MD sha256_md=
{
	NID_sha256,
	NID_sha256WithRSAEncryption,
	SHA256_DIGEST_LENGTH,
	sha256_init,
	sha256_update,
	sha256_final,
	SCRT_PKEY_NULL_method,
	SHA_CBLOCK,
	sizeof(SCRT_MD *)+sizeof(SHA256_CTX)
};

/*
	Name : SCRT_sha256
 	Description : SHA256 ��ȣ �˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters 
 	Return Value : sha256_md ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_MD *SCRT_sha256(void)
{
	return(&sha256_md);
}

#endif

