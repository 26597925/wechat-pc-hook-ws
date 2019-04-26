#pragma once
#include "stdafx.h"
#include "cJSON.h"
#include  <list>

#define WECHAT_WIN_BASE (DWORD)GetModuleHandle(_T("WeChatWin.dll"))
//��ά���ƫ������ַ��1E4638
#define PIC_CALL_FUN_ADDRESS 0x1E4638

//�ָ���ַƫ����,Ҳ������һ���ֽ�
#define PIC_CALL_BACK_ADRESS 0x1E463D

#define HOOK_COMMAND_LEN 5

//������Ϣ������ƫ����
#define SEND_CALL_RL_ADDRESS 0x2DA2D7

//������Ϣ������ƫ����
#define WECHAT_SEND_MSG_CALL_RL_ADDRESS 0x2DA0F0
//#define WECHAT_SEND_MSG_CALL_RL_ADDRESS 0x2990320

//΢������Ϣ�Ļص�call
#define WECHAT_RECV_MSG_CALL_RL_ADDRESS 0x305753


//���ݻ�ַ
#define WXADDR_DATA_BASE *((PDWORD)(WECHAT_WIN_BASE+0x113227C))

//Ⱥ���������
#define WXADDR_ALL_GROUPS (WECHAT_WIN_BASE+0x678+0x64)


//��ά�벿��
BOOL WeChat_QrCodeHook_Install();
BOOL WeChat_QrCodeHook_UnInstall();

//������Ϣ��wxid Ⱥ�Ļ��߸��˵�id��message��Ϣid��atid@�˵�id
void wx_send_text_message(wchar_t* wxId, wchar_t* message, wchar_t* atId);
void wx_send_xml_message(wchar_t* wxid, wchar_t* fWxid, wchar_t* title, wchar_t* content, wchar_t* pic);
void wx_send_share_user_card(wchar_t* wxid, wchar_t* fWxid, wchar_t* name);


#pragma region �ṹ�岿��

struct wx_xml
{
	//����
	wchar_t* pTitleStr;
	int titleLen;
	int titleMaxLen;
	int titleFill = 0;
	int titleFill2 = 0;
	//url
	wchar_t* pUrlStr;
	int urlLen;
	int urlMaxLen;
	int urlFill = 0;
	int urlFill2 = 0;
	//
};


//3���
struct wx_str_3
{
	wchar_t* pStr;
	int strLen;
	int strMaxLen;
};

//5�����
struct wx_str_5
{
	wchar_t* str;
	int len;
	int maxLen;
	int f1 = 0;
	int f2 = 0;
};

struct chat_room_user_info
{
	wchar_t wxid[0x100];
	wchar_t username[0x100];
	wchar_t nickname[0x100];
	wchar_t v1[0x100];
	//�Ƿ��ȡ�ɹ�
	bool success = FALSE;
};

//΢���û�
struct we_chat_user
{
	wchar_t wxid[0x100] = { 0 };
	wchar_t username[0x100] = { 0 };
	wchar_t nickname[0x100] = { 0 };
	wchar_t avatar[0x200]{};
	wchar_t account[0x200]{};
	wchar_t remark[0x200]{};
	wchar_t addType[0x200]{};
	bool success = false;
	int type = 1;
};

/*�û���¼��Ϣ�ṹ��*/
struct wx_login_user_info
{
	wchar_t * wxid;
	wchar_t * account;
	wchar_t * avatar;
	wchar_t * nickname;
	wchar_t * phone;
	wchar_t * address;
	wchar_t * mobileType;
	wchar_t * qrcode;
};


//������Ϣ�ṹ��
struct WECHAT_RECV_MESSAGE
{
	CHAR* wxid = { 0 }; //���ܵ���Ϣ��Ŀ��
	CHAR* nickname = { 0 };
	CHAR* content = { 0 }; //��Ϣ����
	int recType = 0; //0:��ͨ�û���Ϣ��1:Ⱥ����Ϣ
	CHAR* senderWxId = { 0 }; //������΢��ID��Ⱥ�ĵ�ʱ��
	CHAR* senderName = { 0 };
};

#pragma endregion

//�޸ĺ��ѱ�ע

//��ȡȺ��Ա�б�
std::list<chat_room_user_info> get_chat_room_user_list(wchar_t* chat_room_wid);

// ��ȡ�����б�
std::list<we_chat_user> get_friends_list(int type);

//ͨ��΢��id�õ��û���Ϣ
chat_room_user_info get_user_info_by_wid(wchar_t* wid);

//ȷ����Ӻ���


//�ж��Ƿ��ѵ�¼
bool check_wx_is_login();

//��Ϣ�ص�����
typedef void(*RecvMessageCallbackFunc)(WECHAT_RECV_MESSAGE);

cJSON* user_recvmsg_to_json(WECHAT_RECV_MESSAGE msg);

//ͨ�ù��߰����Ŀ�
DWORD GetWechatWinDllAddress();


//��ȡ�û���Ϣ
wx_login_user_info wx_get_current_user_info();


//������Ϣ��Hook
BOOL WeChat_RecvMessage_Hook_Install(RecvMessageCallbackFunc recvCallFun);
BOOL WeChat_RecvMessage_Hook_UnInstall();


//ϵͳ״̬
bool WeChatStateChangeHookInstall(DWORD callback);


//�رյ�ǰϵͳ����
void close_current_process();


wchar_t* UTF8ToUnicode(const char* str);
char* UnicodeToUTF8(const WCHAR* str);


//utf8 תGBK
char* Tool_GbkToUtf8(const char* src_str);
char* Tool_UTF8ToGBK(const char* src_str);

wchar_t* Tool_CharToWchar(const char* c, size_t m_encode);
//CP_ACP
char* Tool_WcharToChar(const wchar_t* wp, size_t m_encode);
char * UnicodeToANSI(const wchar_t *str);
