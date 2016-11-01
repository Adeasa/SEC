/*
 error.h : 
 created by dgshin, in 2012/10/26

 �Լ� return type�� ����, ���п��� ������ 
  ���ϴ��� ����Ÿ�Ե��� ���� 
*/

#ifndef __ERROR_H__
#define __ERROR_H__


#ifdef __cplusplus
extern "C"
{
#endif

typedef  int SRESULT;

/*��������.. 1st byte */
#define S_SUCCESS	0x00000000
#define S_FAILED    0xF0000000

/* ��з�.. 2-3nd byte */

/* CryptoLib */
#define SL_BIGNUM	0x00100000
#define SL_BLOCK	0x00200000
#ifndef NO_ECC
#define SL_ECC		0x00300000
#endif
#define SL_HASH		0x00400000
#define SL_MAC		0x00500000
#define SL_PUBLIC	0x00600000
#define SL_RANDOM	0x00700000
#define SL_SIGN		0x00800000
#define SL_VERIFY	0x00900000

/* �߹����� (�Լ�����) 4-5th byte */

/* ���� 6-8th byte */


/* status �� S_FAILED ���� �������� �Լ�  */
#define ERROR_FAILED(status)	((SRESULT) status < 0)
#define ERROR_DETECTED(status, error) ((status & error) == error)

/* ���� ������ status �� function library ���� code�� ������ش�. */
#define ERROR_MKFUNC(status) ((SRESULT) status  << 12 ) & 0x000FF000
/* ���� ������ status �� library ���� code�� ����� �ش�.  */
#define ERROR_MKLIB(status)   ((SRESULT) status  << 20 ) & 0x0FF00000

#ifdef __cplusplus
}
#endif


#endif