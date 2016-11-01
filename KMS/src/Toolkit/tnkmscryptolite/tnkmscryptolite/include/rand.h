#ifndef _RAND_H_
#define _RAND_H_

#include "type.h"

#ifdef __cplusplus
extern "C"
{
#endif

/////////////////////////////////////////////////////////////////////////
typedef struct _KL_ATTRIBUTE {
	U32		type;			/* �������� Ÿ��				*/
	void	*pValue;			/* ������						*/
	U32		ulValueLen;		/* �������� byte ���� ũ��		*/
	int	bSensitive;		/* ���Ȼ� �ΰ��� ������ ����	*/
	int	bAlloc;			/* pValue�� ���� ���̺귯��
									   ���ο����� �޸� �Ҵ� ����	*/
} KL_ATTRIBUTE, *KL_ATTRIBUTE_PTR;

#define	KL_SZ_OBJECT			8		
typedef KL_ATTRIBUTE			KL_OBJECT[KL_SZ_OBJECT];
typedef KL_OBJECT*				KL_OBJECT_PTR;

#define	KL_SZ_CONTEXT_INTERNAL	10
#define	KL_SZ_CONTEXT_EXTERNAL	10
#define	KL_SZ_CONTEXT	KL_SZ_CONTEXT_INTERNAL+KL_SZ_CONTEXT_EXTERNAL

typedef KL_ATTRIBUTE	KL_CONTEXT[KL_SZ_CONTEXT];
typedef KL_CONTEXT*		KL_CONTEXT_PTR;
/////////////////////////////////////////////////////////////////////////

/* mode */
#define DRBG_RAND_SHA256 2
#define DRBG_RAND_SHA512 3

void S_RAND_Bytes(U8 *buf, int num);
void S_GenerateRandom_LSH256(U8 *out, U32 bytes);
void S_GenerateRandom(U8 *out, U32 bytes, int mode);

#ifdef __cplusplus
}
#endif

#endif
