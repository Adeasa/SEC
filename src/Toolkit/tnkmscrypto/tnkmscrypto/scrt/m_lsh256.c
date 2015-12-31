
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     m_lsh256.c

	 Creadted by DEV3

************************************************/

#ifndef NO_LSH256

#include "../include/scrt.h"
#include <stdio.h>

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_MD lsh256_md=
{
	NID_lsh256,
	NID_lsh256WithRSAEncryption,
	LSH256_DIGEST_LENGTH,
	lsh256_init,
	lsh256_update,
	lsh256_final,
	SCRT_PKEY_NULL_method,
	LSH_CBLOCK,
	sizeof(SCRT_MD *)+sizeof(LSH256_CTX)
};

/*
	Name : SCRT_lsh256
 	Description : LSH256 ��ȣ �˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters 
 	Return Value : lsh256_md ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_MD *SCRT_lsh256(void)
{
	return(&lsh256_md);
}

#endif

