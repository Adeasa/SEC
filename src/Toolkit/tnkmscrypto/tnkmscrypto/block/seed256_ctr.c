/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     seed256_ctr.c, SEED256 �˰���, CTR128 mode

	 Creadted by DEV3

************************************************/

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../include/seed256.h"
#include "../include/typeconvert.h"

typedef unsigned char seed_cblock[16];

/* NOTE: the IV/counter CTR mode is big-endian.  The rest of the SEED code
 * is endian-neutral. */

/* increment counter (128-bit int) by 1 */
/*	
	Name : S_SEED256_CTR128_Inc
	Description: �Էµ� ī���͸� �ϳ� 1�� ������Ų��
	Parameters
	[in/out] counter : ī����
	Return Value : 
	Note : the IV/counter CTR mode is big-endian.  The rest of the SEED code is endian-neutral
*/
static void S_SEED256_CTR128_Inc(U8 *counter) 
{
	unsigned long c;

	/* Grab bottom dword of counter and increment */
	c = GETU32(counter + 12);
	c++;	c &= 0xFFFFFFFF;
	PUTU32(counter + 12, c);

	/* if no overflow, we're done */
	if (c)
		return;

	/* Grab 1st dword of counter and increment */
	c = GETU32(counter +  8);
	c++;	c &= 0xFFFFFFFF;
	PUTU32(counter +  8, c);

	/* if no overflow, we're done */
	if (c)
		return;

	/* Grab 2nd dword of counter and increment */
	c = GETU32(counter +  4);
	c++;	c &= 0xFFFFFFFF;
	PUTU32(counter +  4, c);

	/* if no overflow, we're done */
	if (c)
		return;

	/* Grab top dword of counter and increment */
	c = GETU32(counter +  0);
	c++;	c &= 0xFFFFFFFF;
	PUTU32(counter +  0, c);
}

/* The input encrypted as though 128bit counter mode is being
 * used.  The extra state information to record how much of the
 * 128bit block we have used is contained in *num, and the
 * encrypted counter is kept in ecount_buf.  Both *num and
 * ecount_buf must be initialized with zeros before the first
 * call to SEED_ctr128_encrypt().
 *
 * This algorithm assumes that the counter is in the x lower bits
 * of the IV (ivec), and that the application has full control over
 * overflow and the rest of the IV.  This implementation takes NO
 * responsibility for checking that the counter doesn't overflow
 * into the rest of the IV when incremented.
 */
/*	
	Name : S_SEED256_CTR128_Encrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� SEED CTR ��� ��ȣ�˰������� �Է� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] in : ��ȣȭ �� ������
	[out] out : ��ȣȭ �� ������
	[in] length : ��ȣȭ �� ������ ����
	[in] key : SEED Ű ����ü
	[in] ivec : �ʱ⺤��
	[in/out] ecount_buf : ī����
	[in/out] num : ī���� ����
	Return Value : 
	Note : ecount_buf �� num�� �ʱ�ȭ �� �� ���Ǿ�� �Ѵ�
*/
void S_SEED256_CTR128_Encrypt(U8 *in, U8 *out, U32 length, SEED256_KEY *key, U8 ivec[SeedBlockSize], U8 ecount_buf[SeedBlockSize], unsigned int *num) 
{
	register U32 v0,v1,v2,v3,t;
	register int n= *num;
	register long l= length;
	seed_cblock d; 
	register U8 *dp;
	U32 tin[4];
	U8 *ivTmp;
	int save=0;

	ivTmp = (U8 *)ivec;
	U8ToU32(ivTmp,v0);
	U8ToU32(ivTmp,v1);
	U8ToU32(ivTmp,v2);
	U8ToU32(ivTmp,v3);
	tin[0]=v0;
	tin[1]=v1;
	tin[2]=v2;
	tin[3]=v3;
	dp=d;

	while (l--)
	{
		if (n == 0)
		{
			S_SEED256_Encrypt(tin,key);
			dp=d;
			t=tin[0]; U32ToU8(t,dp);
			t=tin[1]; U32ToU8(t,dp);
			t=tin[2]; U32ToU8(t,dp);
			t=tin[3]; U32ToU8(t,dp);
			save++;
			S_SEED256_CTR128_Inc(ivec);
			ivTmp = (U8 *)ivec;
			U8ToU32(ivTmp,v0);
			U8ToU32(ivTmp,v1);
			U8ToU32(ivTmp,v2);
			U8ToU32(ivTmp,v3);
			tin[0]=v0;
			tin[1]=v1;
			tin[2]=v2;
			tin[3]=v3;
		}
		*(out++)= *(in++)^d[n];
		n=(n+1)&0x0f;
	}

	t=v0=v1=v2=v3=tin[0]=tin[1]=tin[2]=tin[3]=0;
	*num=n;
}

/*	
	Name : S_SEED256_CTR128_Decrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� SEED CTR ��� ��ȣ�˰������� ��ȣȭ�� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] in : ��ȣȭ �� ������
	[out] out : ��ȣȭ �� ������
	[in] length : ��ȣȭ �� ������ ����
	[in] key : SEED Ű ����ü
	[in] ivec : �ʱ⺤��
	[in/out] ecount_buf : ī����
	[in/out] num : ī���� ����
	Return Value : 
	Note : ecount_buf �� num�� �ʱ�ȭ �� �� ���Ǿ�� �Ѵ�
*/
void S_SEED256_CTR128_Decrypt(U8 *in, U8 *out, U32 length, SEED256_KEY *key, U8 ivec[SeedBlockSize], U8 ecount_buf[SeedBlockSize], unsigned int *num) 
{
	register U32 v0,v1,v2,v3,t;
	register int n= *num;
	register long l= length;
	seed_cblock d; 
	register U8 *dp;
	U32 tin[4];
	U8 *ivTmp;
	int save=0;

	ivTmp = (U8 *)ivec;
	U8ToU32(ivTmp,v0);
	U8ToU32(ivTmp,v1);
	U8ToU32(ivTmp,v2);
	U8ToU32(ivTmp,v3);
	tin[0]=v0;
	tin[1]=v1;
	tin[2]=v2;
	tin[3]=v3;
	dp=d;

	while (l--)
	{
		if (n == 0)
		{
			S_SEED256_Encrypt(tin,key);
			dp=d;
			t=tin[0]; U32ToU8(t,dp);
			t=tin[1]; U32ToU8(t,dp);
			t=tin[2]; U32ToU8(t,dp);
			t=tin[3]; U32ToU8(t,dp);
			save++;
			S_SEED256_CTR128_Inc(ivec);
			ivTmp = (U8 *)ivec;
			U8ToU32(ivTmp,v0);
			U8ToU32(ivTmp,v1);
			U8ToU32(ivTmp,v2);
			U8ToU32(ivTmp,v3);
			tin[0]=v0;
			tin[1]=v1;
			tin[2]=v2;
			tin[3]=v3;
		}
		*(out++)= *(in++)^d[n];
		n=(n+1)&0x0f;
	}

	t=v0=v1=v2=v3=tin[0]=tin[1]=tin[2]=tin[3]=0;
	*num=n;
}
