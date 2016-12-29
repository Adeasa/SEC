# Quick Start - KMS

Oasis Security �����ؼ� KMS ������Ʈ �� ��Ŷ�� ó�� ���ϴ� �е��� �ҽ��� �ٿ�ް� ���� ������ �� �ֵ��� �ȳ��Ѵ�.

KMS ������Ʈ �� ��Ŷ�� ������ ���� ������ ����� �� �ִ�.

> 1. ������ �ٿ�ε� �� ��ġ
> 2. ������Ʈ �� ��Ŷ �ҽ� �ٿ�ε�
> 3. ������Ʈ �� ��Ŷ �ҽ� ����
> 4. ������Ʈ �� ��Ŷ ���̺귯�� ����
> 5. ������Ʈ ���� ���

## Requirements
* Raspberrypi
* Rasbian OS

## �����ϱ�

#### (1) ������ �ٿ�ε� �� ��ġ
- [������ �ٿ�ε�] (https://www.raspberrypi.org/downloads/raspbian/)
- [������ ��ġ�ȳ�] (https://www.raspberrypi.org/documentation/installation/installing-images/README.md)

#### (2) ������Ʈ �� ��Ŷ �ҽ� �ٿ�ε�
- [������ ������](https://github.com/iotoasis/SEC/releases)���� �ҽ� �� ��ġ���� ������ �ٿ���� �� KMS ������ �ҽ��� ����Ѵ�.

#### (3) ������Ʈ �� ��Ŷ �ҽ� ����
- ������Ʈ �ҽ� ���� : Agent �������� make ?f Makefile.linux.arm ����
- ��Ŷ �ҽ� ���� : Toolkit/TrustKeystoreCstk/src �������� make ?f make.raspberry.32 ����

#### (4) ������Ʈ �� ��Ŷ ���̺귯�� ����
- ������Ʈ ���̺귯�� : libTKSAgent.so, libTKSAgent.so, libTKSAgentAdv.so, libTKSAgentLite.so
- ��Ŷ ���̺귯�� : libTKSCstk.so, libTKSCstkLite.so, libTKSKmsCstk.so, libTKSKmsCstkLite.so

#### (5) ������Ʈ ���� ���
- ������ TrustKeystoreAgent.conf ���Ϸ� �ۼ��Ǹ� CA ������ unetsystem-rootca.pem�� ���� ������ ��ġ��Ų �� �ʱ�ȭ API�� �� ��θ� �Է��Ѵ�. ���������� �Ʒ� ���� ���������� ����Ѵ�. 

kmsIP=166.104.112.40
kmsPort=9002
agentID=oasis_test
agentType=1
agentHint=GeNiVZchB9QrjOy3fvViLoQuilB3im7Y3RzpRzLayp4=
Integrity=FpDvNYpuw2kZm11mdAgkmtGgaETFcWCB3kU52VS/uVU=

<br>
<br>
<br>

# Quick Start - CAS

Oasis Security �����ؼ� CAS Ŭ���̾�Ʈ �� ��Ŷ�� ó�� ���ϴ� �е��� �ҽ��� �ٿ�ް� ���� ������ �� �ֵ��� �ȳ��Ѵ�.

CAS Ŭ���̾�Ʈ �� ��Ŷ�� ������ ���� ������ ����� �� �ִ�.

> 1. ������ �ٿ�ε� �� ��ġ
> 2. Ŭ���̾�Ʈ �� ��Ŷ �ҽ� �ٿ�ε�
> 3. Ŭ���̾�Ʈ �� ��Ŷ �ҽ� ����
> 4. Ŭ���̾�Ʈ �� ��Ŷ ���̺귯�� ����
> 5. Ŭ���̾�Ʈ ���� ���

## Requirements
* Raspberrypi
* Rasbian OS

## �����ϱ�

#### (1) ������ �ٿ�ε� �� ��ġ
- [������ �ٿ�ε�] (https://www.raspberrypi.org/downloads/raspbian/)
- [������ ��ġ�ȳ�] (https://www.raspberrypi.org/documentation/installation/installing-images/README.md)

#### (2) Ŭ���̾�Ʈ �� ��Ŷ �ҽ� �ٿ�ε�
- [������ ������](https://github.com/iotoasis/SEC/releases)���� �ҽ� �� ��ġ���� ������ �ٿ���� �� CAS ������ �ҽ��� ����Ѵ�.

#### (3) Ŭ���̾�Ʈ �� ��Ŷ �ҽ� ����
- Ŭ���̾�Ʈ �ҽ� ���� : ?	CAClient �������� make ����
- ��Ŷ �ҽ� ���� : ?	TrustNETCASCstk �������� make ����

#### (4) Ŭ���̾�Ʈ �� ��Ŷ ���̺귯�� ����
- Ŭ���̾�Ʈ ���̺귯�� : libTrustNETCASClient.so
- ��Ŷ ���̺귯�� : libTrustNETCASCstk.so

#### (5) Ŭ���̾�Ʈ ���� ���
- ������ TrustNetCaClient.conf ���Ϸ� �ۼ��Ǹ� CA ������ trustnetcas-rootca.crt�� ���� ������ ��ġ��Ų �� �ʱ�ȭ API�� �� ��θ� �Է��Ѵ�.

casIP=166.104.112.40
casPort=9005

<br>
<br>
<br>