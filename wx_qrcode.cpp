#include "stdafx.h"
#include "wx_helper.h"
#include <atlimage.h>
#include <direct.h>


#define CWD_BUFFER_LEN 0x3000

char* GetCurrentCwd()
{
	char buffer[CWD_BUFFER_LEN];
	if (_getcwd(buffer, CWD_BUFFER_LEN))
	{
	}
	return buffer;
}


//�����ά������
VOID SaveQrCodeImg(DWORD qrcode)
{
	//ͼƬ����
	DWORD picLen = qrcode + 0x4;
	//ͼƬ������,4M
	char PicData[0xFFF] = {0};
	size_t cpyLen = (size_t)*((LPVOID *)picLen);
	//��ȡ��ͼƬ������
	memcpy(PicData, *((LPVOID *)qrcode), cpyLen);
	FILE* pFile;

	//���ļ� �����ھʹ���
	char path[0x3000];
	sprintf_s(path, "%s%s", GetCurrentCwd(), "\\qr-code.png");
	fopen_s(&pFile, path, "wb");

	//ͼƬ����
	fwrite(PicData, sizeof(char), sizeof(PicData), pFile);
	//�ر�
	fclose(pFile);
}

//�����Ĵ���
//pushad
//pushsd
//popad
//popsd
DWORD pEax = 0;
DWORD pEcx = 0;
DWORD pEdx = 0;
DWORD pEbx = 0;
DWORD pEbp = 0;
DWORD pEsp = 0;
DWORD pEsi = 0;
DWORD pEdi = 0;
//����һ���㺯������Ӱ���ջ,������������Ĳ��������߱�����,�㺯���������ڲ����ر���
DWORD _QrCodeRetAdd = 0;
VOID __declspec(naked) WeChat_QrCodeHook_ShowPic()
{
	////���ݼĴ���
	__asm {
		mov pEax, eax
		mov pEcx, ecx
		mov pEdx, edx
		mov pEbx, ebx
		mov pEbp, ebp
		mov pEsp, esp
		mov pEsi, esi
		mov pEdi, edi
	}
	//���ǵĶ�ά��������ecx����
	SaveQrCodeImg(pEcx); //��������дһ���������������ά������
	//////���صĵ�ַ 1E463D
	_QrCodeRetAdd = GetWechatWinDllAddress() + PIC_CALL_BACK_ADRESS;
	__asm {
		mov eax, pEax
		mov ecx, pEcx
		mov edx, pEdx
		mov ebx, pEbx
		mov ebp, pEbp
		mov esp, pEsp
		mov esi, pEsi
		mov edi, pEdi
		jmp _QrCodeRetAdd
	}
}

//���ݵ�5���ֽ�
BYTE buf[HOOK_COMMAND_LEN] = {0}; //����Ҫ5���ֽڵ����ݾͿ�����

//���ڴ��ַ�е���Ϣ�滻�����ǵ�DLL����,hook
BOOL WeChat_QrCodeHook_Install()
{
	DWORD funShow = (DWORD)(&WeChat_QrCodeHook_ShowPic); //ȡ������ַ

	DWORD dw = GetWechatWinDllAddress(); //��ȡdll��ַ
	DWORD hookCall = dw + PIC_CALL_FUN_ADDRESS; //hook�ĺ�����ʵ��ַ

	//E8 D34D2900    E8����call�����������ֵ= Ҫ��ת�ĵ�ַ-hook�ĵ�ַ-5���ֽ�
	BYTE jmpCode[HOOK_COMMAND_LEN] = {0}; //��Ҫ�滻����ָ�����
	jmpCode[0] = 0xE9; //jmpָ��
	*(DWORD *)&jmpCode[1] = (DWORD)funShow - hookCall - HOOK_COMMAND_LEN; //��ȡ��ַ��ֵ

	//1.����ԭ��������,�Ժ�ж��hook��Ҫ�õõ�
	HANDLE hd = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (FALSE == ReadProcessMemory(hd, (LPCVOID)hookCall, buf, sizeof(buf), NULL))
	{
		return FALSE;
	}

	//2.д�����ǵ��Լ���ָ��
	if (FALSE == WriteProcessMemory(hd, (LPVOID)hookCall, jmpCode, HOOK_COMMAND_LEN, NULL))
	{
		return FALSE;
	}
	OutputDebugStringA("��װQrCodeHook�ɹ�");
	//�رվ��
	CloseHandle(hd);
	return TRUE;
}


BOOL WeChat_QrCodeHook_UnInstall()
{
	//�ָ�HOOK

	DWORD hookCallAddr = GetWechatWinDllAddress() + PIC_CALL_FUN_ADDRESS;
	HANDLE hd = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (FALSE == WriteProcessMemory(hd, (LPVOID)hookCallAddr, buf, HOOK_COMMAND_LEN, NULL))
	{
		return FALSE;
	}
	//�رվ��
	CloseHandle(hd);
	OutputDebugStringA("��ж��QrCodeHook");
	return TRUE;
}
