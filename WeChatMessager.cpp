#include "stdafx.h"
#include <stdio.h>
#include "malloc.h"
#include "resource.h"
#include  "GetWxInfo.h"
#include "WeChatHelper.h"

//������Ϣ�Ŀ�

struct WeChatTextMessage
{
	wchar_t *pStr;//��Ϣ
	int strLen;//���ݵĳ���
	int iStrLen;//�����ı�����wxid*2
};


//1015A2D0    53              push ebx
//1015A2D1    8D8D ECFDFFFF   lea ecx, dword ptr ss : [ebp - 0x214]
//1015A2D7    E8 F49BD7FF     call WeChatWi.0FED3ED0
//2. ��һƬ�հ����򴴽��Լ����ַ���
//11767D24  11317048  UNICODE "eee"  �ı�����
//11767D28  00000003  ����
//11767D2C  00000004  ��д���ȵ�2��
//
//3.�޸Ķ�ջ��ַ���޸�ebx�ĵ�ַ������
//���͵��ô�������
//0FF443F8    8B55 CC         mov edx, dword ptr ss : [ebp - 0x34]
//0FF443FB    8D43 14         lea eax, dword ptr ds : [ebx + 0x14]
//0FF443FE    6A 01           push 0x1  ��һ������ 0x1
//0FF44400    50              push eax	�ڶ�������eax
//0FF44401    53              push ebx ����������ebx ���Ƿ�����Ϣ������
//0FF44402    8D8D E4F7FFFF   lea ecx, dword ptr ss : [ebp - 0x81C]
//0FF44408    E8 E35C2100     call WeChatWi.1015A0F0
//0FF4440D    83C4 0C         add esp, 0xC

///atid ���Ǳ�at�˵�΢��ID
BOOL WeChat_SendTextMessage(wchar_t* wxid, wchar_t* message, wchar_t* atid) {

	//1.���췢�͵���
	WeChatTextMessage recv = { 0 };
	recv.pStr = wxid;
	recv.strLen = wcslen(wxid);
	recv.iStrLen = 2 * recv.strLen;

	//2.������Ϣ
	WeChatTextMessage msg = { 0 };
	msg.pStr = message;
	msg.strLen = wcslen(message);
	msg.iStrLen = 2 * msg.strLen;


	//3.����Ⱥ��At�˵���Ϣ
	WeChatTextMessage atRecv = { 0 };
	atRecv.pStr = atid;
	atRecv.strLen = wcslen(atid);
	atRecv.iStrLen = 2 * msg.strLen;

	//��ȡ����
	char* asmWxId = (char*)&recv.pStr;//���ֱ���ǵ�ַ
	char* asmMsg = (char*)&msg.pStr;//��Ϣ
	char* asmAt = (char*)atRecv.pStr;//at����

	char buff[0x81C] = { 0 };

	//call�ĵ�ַ
	DWORD sendCallAddr = GetWechatWinDllAddress() + WECHAT_SEND_MSG_CALL_RL_ADDRESS;

	//�ȼ���ǲ���һ����ȷ��call��ַ
	char txt[0x100] = { 0 };
	sprintf_s(txt, "WXID=%s,��Ϣ����=%s,����=%s", wxid, message, atid);
	OutputDebugStringA(txt);
	//ִ�л��,eaxһֱ������0
	__asm {
		mov edx, asmWxId
		mov eax, asmAt
		push 0x1
		push eax
		mov ebx, asmMsg
		push ebx
		lea ecx, buff
		call sendCallAddr
		add esp, 0xC
	}
	return TRUE;
}

//
VOID WeChat_SendImageMessage(wchar_t * wxid, BYTE *picPath) {


}

VOID On_RecvMessage() {

}

//hook����Ϣ��ַ ��װ���Ӻ���
VOID WeChat_RecvMessage_Hook_Install(DWORD hookCallAddr) {





}
//ж�ع��Ӻ���
VOID WeChat_RecvMessage_Hook_UnInstall() {

}