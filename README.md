![iotoasis](https://github.com/iotoasis/SO/blob/master/logo_oasis_m.png)

# Oasis Project

본 Oasis (Open-source Architecture Semantic Iot Service-platform) 프로젝트는 국제 표준을 준용하는 오픈 소스 기반 지능형 사물 인터넷 서비스 플랫폼을 개발하는 것을 목표로 하고 있습니다.

본 Oasis 프로젝트는 2015년도 정보통신/방송 기술개발사업 신규지원 대상과제 "(ICBMS-3세부) 사물 가상화, 분산 자율지능 및 데이터 연계/분석을 지원하는 IoT 기반 플랫폼 기술 개발" 과제의 결과물로써 오픈소스로 제공됩니다.

본 Oasis 프로젝트는 오픈 소스 커뮤니티를 기반으로 오픈소스로써 계속적으로 성장해 나갈 계획입니다.

# KMS

KMS(Key Management Service)는 IoT 환경에서 디바이스게이트웨이와 디바이스 간 송수신 데이터에 대한 기밀성을 유지하기 위해 암호화를 수행할 때 사용할 수 있는 암호키 관리 방법을 제공합니다.
제공되는 소스코드는 KMS 서버에서 관리되는 암호키를 획득하기 위한 KMS Agent 와 암호키를 이용하여 데이터를 암호화하기 위한 KMS Toolkit 대한 것입니다.

## 주요 기능
- 경량환경용 암호키 관리 클라이언트 환경 지원
- 경량암호화 알고리즘 지원(LEA 등)
- 지원가능환경 : 라즈베리파이 및 유사 디바이스용 환경

# CAS

CAS(Certificate Authority Service)는 IoT 환경에서 SI서버가 디바이스게이트웨이를 인증할 때 인증강화를 위해 기기인증서를 발급하고 관리할 수 있는 방법을 제공합니다.
제공되는 소스코드는 CAS 서버에게 기기인증서 발급을 요청하고 인증서를 발급받아 저장하기 위한 CAClient 와 발급된 기기인증서를 이용하여 전자서명을 수행하는 Toolkit 대한 것입니다.

## Documents
- [Quick Start](./sec-doc/quick-start.md)

## Downloads
- [Latest Release](https://github.com/iotoasis/SEC/releases)
 
## 주요 기능
- 경량환경용 기기인증서 발급 클라이언트 환경 지원
- 경량암호화 전자서명 알고리즘 지원(ECC 등)
- 지원가능환경 : 라즈베리파이 및 유사 디바이스용 환경


## Modules
- KMS/src/Agent 는 KMS 서버에 암호키를 요청하고 수신받아 로컬 저장하는 역할을 합니다. 
- KMS/src/Toolkit 은 Agent가 보관하고 있는 암호키를 전달받아 데이터를 암호화/복호화하는 역할을 합니다.
- CAS/src/CAClient 는 CAS 서버에 기기인증서 발급을 요청하고 기기인증서를 수신받아 로컬 저장하는 역할을 합니다. 
- CAS/src/Toolkit 은 CAClient가 보관하고 있는 기기인증서 정보를 전달받아 디바이스게이트웨이가 SI서버에 접속 시 기기인증을 위한 전자서명을 수행하는 역할을 합니다.
<br>

## Q&A
 - [IoT Oasis Q&A -- Commong soon]

## License
Licensed under the BSD License, Version 2.0
<br>



