//�γ����-�������ػ��
//1����������ָ������Զ���ʻģʽ���߶ȣ�
//2��ʵʱ����������¶ȣ�
//3����ǰ���Զ���ʻģʽ�����¶ȳ���120�ȣ�
//     ����ת�١�
//4��ʵʱ������߶ȣ�
//5����ǰ���Զ���ʻģʽ���Ҹ߶ȷ����仯ʱ��
//     �����߶ȼӴ�������
//6��ָ�����
//7��gps  ����
//8���ڴ洦��
// 9.�ɼ�ת�٣�������ֵʱ��ȼ�ϲ��ͷŹ���Һ��ú��
//10.�л�ģʽ���߷�����Ϊ�ͷ�
// 11.�ɼ������������Ӵ����ر�
// 12.������ת�ٿ��ƣ�pid�㷨��
//�ź�������ƣ�   �¶�:2���߶�:1���Զ���ʻ:0


#include <Winsock2.h>
#include <includes.h>  
#pragma comment(lib, "ws2_32.lib")
//////////////////////////////////////////////////////////

OS_FLAG_GRP* Sem_F;//�����ź�����
INT8U err;   //����һ��������Ϣ 
int gaodu = 500;
void* msgPtr[5];
OS_EVENT* Semp;    //��Ϣ����   
OS_EVENT* Sempyx;  //����  
OS_MEM* intBuffer;//�����ڴ���ƿ�ָ��
INT8U intPart[50][64];//���ַ������ڴ��
INT8U* intBlkPtr;//����ָ��

OS_EVENT* Semp2;    //�����ź���  
OS_EVENT* Semp3;    //�����ź���  

INT8U buffer[5];


void WinSocketServer()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	//int err1;
	int sockSrv;
	SOCKADDR_IN 	 addrSrv;
	SOCKADDR_IN addrClient;// �����ϵĿͻ���ip��ַ
	int len;

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return;
	}
	sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));// �󶨶˿� 
	listen(sockSrv, 5);
	len = sizeof(SOCKADDR);
	while (1)
	{
		//unsigned char mm[2];
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);// ���ܿͻ�������,��ȡ�ͻ��˵�ip��ַ
		//  sprintf(sendBuf,"Welcome %s to here!",inet_ntoa(addrClient.sin_addr));// �����Ϣ���ͳ�ȥ 
		intBlkPtr = OSMemGet(intBuffer, &err);//�����ڴ棬����ڴ����ָ��
		recv(sockConn, intBlkPtr, 64, 0);// ���ܿͻ�����Ϣ
		OSQPost(Semp, intBlkPtr);  //�е����ݿ��ܲ��������ͶԵ��ˡ�    

		closesocket(sockConn);//�Ͽ�����
	}
}
#define  TASK_STK_SIZE    128

OS_STK        AppStk_1[TASK_STK_SIZE];
OS_STK        AppStk_2[TASK_STK_SIZE];
OS_STK        AppStk_3[TASK_STK_SIZE];
OS_STK        AppStk_4[TASK_STK_SIZE];
OS_STK        AppStk_5[TASK_STK_SIZE];
OS_STK        AppStk_6[TASK_STK_SIZE];
OS_STK        AppStk_7[TASK_STK_SIZE];
OS_STK        AppStk_8[TASK_STK_SIZE];
OS_STK        AppStk_9[TASK_STK_SIZE];
OS_STK        AppStk_10[TASK_STK_SIZE];
OS_STK        AppStk_11[TASK_STK_SIZE];
OS_STK        AppStk_12[TASK_STK_SIZE];
OS_STK        AppStk_13[TASK_STK_SIZE];
OS_STK        AppStk_14[TASK_STK_SIZE];
OS_STK        AppStk_15[TASK_STK_SIZE];
OS_STK        AppStk_16[TASK_STK_SIZE];
OS_STK        AppStk_17[TASK_STK_SIZE];
OS_STK        AppStk_18[TASK_STK_SIZE];
OS_STK        AppStk_19[TASK_STK_SIZE];
OS_STK        AppStk_20[TASK_STK_SIZE];
OS_STK        AppStk_21[TASK_STK_SIZE];
OS_STK        AppStk_22[TASK_STK_SIZE];
OS_STK        AppStk_23[TASK_STK_SIZE];


struct _pid {
	float SetSpeed;  //�趨�ٶ�
	float ActualSpeed;	//ʵ���ٶ�
	float err;	//ƫ��ֵ
	float err_next; //��һ��ƫ��ֵ
	float err_last;	//����һ��ƫ��ֵ
	float Kp, Ki, Kd; //���������֣�΢��ϵ��
	float voltage;	//��ѹ	
	float integral;	//����ֵ

}pid;
void PID_init() {
	printf("Pid_init \n");
	pid.SetSpeed = 0.0;
	pid.ActualSpeed = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.err_next = 0.0;
	pid.voltage = 0.0;
	pid.integral = 0.0;
	pid.Kd = 0.2;
	pid.Ki = 0.015;
	pid.Kd = 0.2;
}
float PID_realize(float speed) {
	/*����Ŀ��ֵ*/
	pid.SetSpeed = speed;
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
	pid.err = pid.SetSpeed - pid.ActualSpeed;
	/*PID�㷨ʵ��*/
	float increment_val = pid.Kp * (pid.err - pid.err_next) + pid.Ki * pid.err + pid.Kd * (pid.err - 2 * pid.err_next + pid.err_last);
	/*�ۼ�*/
	pid.ActualSpeed += increment_val;
	/*�������*/
	pid.err_last = pid.err_next;
	pid.err_next = pid.err;
	/*���ص�ǰʵ��ֵ*/
	return pid.ActualSpeed;


}
void App_Socket(void* p_arg)
{
	while (1)
	{
		WinSocketServer(); //����socket��������
	}
	//OSTimeDlyHMSM(0, 0, 1, 0); //
}

// �¶ȣ��߶ȣ����䣬���������л�ģʽ�Զ���ʻģʽ
void App_gaodu_caiji(void* p_arg)
{
	while (TRUE)
	{
		int temp = gaodu + rand() % 10;
		printf("�ɼ��߶ȣ� %d\n", temp);
		if (temp - gaodu > 50)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ����Ϊ��Ԥ������
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void App_wendu_caiji(void* p_arg)
{
	while (TRUE)
	{
		int temp = 40 + rand() % 30;
		printf("�ɼ��¶ȣ� %d\n", temp);
		if (temp > 50)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void App_youxiang_caiji(void* p_arg)
{
	while (TRUE)
	{
		int temp = 30 + rand() % 30;
		printf("�ɼ����������� %d\n", temp);
		if (temp > 40)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

/*void App_fadonji_caiji(void* p_arg)
{
	while (TRUE)
	{
		int temp = 30 + rand() % 30;
		printf("�ɼ�������ת�٣� %d\n", temp);
		if (temp > 40)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}*/
void App_fadonji_caiji(void* p_arg) {
	while (TRUE)
	{
		OSSemPend(Semp3, 0, &err);
		float speed = PID_realize(2000.0);
		OS_Printf("������ת��-%f  \n", speed);
		OSSemPost(Semp3);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

void App_sport_caiji(void* p_arg)
{
	while (TRUE)
	{
		int temp = 60 + rand() % 20;
		printf("��̬������ %d\n", temp);
		if (temp > 100)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

void App_youxiang_kongzhi(void* p_arg)
{
	while (TRUE)
	{
		int temp = 45 + rand() % 30;
		printf("�ɼ����䣺�Ӵ����ر� %d\n", temp);
		if (temp > 90)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void App_gaodu_kongzhi(void* p_arg)
{
	while (TRUE)
	{
		OSFlagPend(Sem_F, (OS_FLAGS)4, OS_FLAG_WAIT_SET_ALL, 0, &err);//�����ź�����
		OS_Printf("ά�ָ߶ȣ�������̬���ڶ�̬ˮƽ�ߵ�\n");
		OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_CLR, &err); //����ź�����Ӧ���
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void App_wendu_kongzhi(void* p_arg)
{
	while (TRUE)
	{
		OSFlagPend(Sem_F, (OS_FLAGS)4, OS_FLAG_WAIT_SET_ALL, 0, &err);//�����ź�����
		OS_Printf("�����¶ȣ����ͷ�����ת��ά���ٶ�\n");
		OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_CLR, &err); //����ź�����Ӧ���
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void App_fadonji_kongzhi(void* p_arg)
{
	while (TRUE)
	{
		int temp = 50 + rand() % 30;
		printf("�ɼ�������ת�٣��������ͷŹ���Һ��ú�� %d\n", temp);
		if (temp > 50)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void App_sport_kongzhi(void* p_arg)
{
	while (TRUE)
	{
		int temp = 60 + rand() % 20;
		printf("�л�ģʽ�� %d\n", temp);
		if (temp > 100)
			OSFlagPost(Sem_F, (OS_FLAGS)4, OS_FLAG_SET, &err); //���ñ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void zhiling(void* p_arg)
{
	while (TRUE)
	{
		unsigned char* mm;
		mm = (char*)OSQPend(Semp, 0, &err);
		if (mm[0] == 1)
		{
			OSFlagPost(Sem_F, (OS_FLAGS)0, OS_FLAG_SET, &err); //���ñ�� 
			OS_Printf("��ʼ������̬��\n");
			gaodu = (int)mm[1];
			//OS_Printf("1-%d  ,%s\n", gaodu, mm);
			OSMemPut(intBuffer, mm);//�ͷ��ڴ��

		}
		else if (mm[0] == 2)
		{
			OS_Printf("��ʼ���������̬�����ٷ���\n");
			//OS_Printf("2-%d  ,%s\n", mm[1], mm);
			OSTaskSuspend(10);
			OSTaskSuspend(11);
			OSTaskSuspend(12);
			OSTaskSuspend(13);
			OSTaskSuspend(32);
			OSTaskSuspend(33);
			OSTaskSuspend(34);
			OSTaskSuspend(35);
			OSTaskSuspend(36);
			OSTaskSuspend(37);
			OSTaskSuspend(38);
			OSTaskSuspend(39);
			OSTaskSuspend(40);
			OSTaskSuspend(41);
			OSTaskSuspend(42);
			OSTaskSuspend(43);
			OSTaskSuspend(44);
			OSTaskSuspend(45);
			//OSTaskSuspend(45);
		}
		else if (mm[0] == 3)
		{
			OSTaskSuspend(10);
			OSTaskSuspend(11);
			OSTaskSuspend(12);
			OSTaskSuspend(13);
			OSTaskSuspend(32);
			OSTaskSuspend(33);
			OSTaskSuspend(34);
			OSTaskSuspend(35);
			OSTaskSuspend(36);
			OSTaskSuspend(37);
			OSTaskSuspend(38);
			OSTaskSuspend(39);
			OSTaskSuspend(40);
			OSTaskSuspend(41);
			OSTaskSuspend(42);
			OSTaskSuspend(43);
			OSTaskSuspend(44);
			OSTaskSuspend(45);
		}
		//OSTimeDlyHMSM(0,0,0,200);

	}
}
void gps1(void* p_arg)
{
	int count = 0;
	while (TRUE)
	{
		int* ptr = 0;
		count++;
		ptr = &count;
		OSMboxPost(Sempyx, ptr);//����һ���������ݣ�ptr������ָ��
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void gps2(void* p_arg)
{
	while (TRUE)
	{
		int* ss = (int*)OSMboxPend(Sempyx, 0, &err);//��������
		OS_Printf("��׽��̬����gps-%d  \n", *ss);
	}
}
void shengyicaiji(void* p_arg) {

	while (TRUE)
	{
		buffer[0]++;
		OSSemPost(Semp2);
		//OS_Printf("ˮƽ�����ж�����켣-%d  \n", *ss);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void shengyichuli(void* p_arg) {
	while (TRUE)
	{
		OSSemPend(Semp2, 0, &err);
		OS_Printf("����Ernst ��Mach-%d  \n", buffer[0]);
	}
}
void sportcaiji(void* p_arg) {

	while (TRUE)
	{
		buffer[0]++;
		OSSemPost(Semp2);
		//OS_Printf("���������-%d  \n", *ss);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void sportchuli(void* p_arg) {
	while (TRUE)
	{
		OSSemPend(Semp2, 0, &err);
		OS_Printf("��̬ģʽ���Զ�-%d  \n", buffer[0]);
	}
}
void fadonjicaiji(void* p_arg) {

	while (TRUE)
	{
		buffer[0]++;
		OSSemPost(Semp);
		//OS_Printf("�����������-%d  \n", *ss);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void fadonjichuli(void* p_arg) {
	while (TRUE)
	{
		OSSemPend(Semp2, 0, &err);
		OS_Printf("�Ӵ����ر�-%d  \n", buffer[0]);
	}
}
void youxiangcaiji(void* p_arg) {

	while (TRUE)
	{
		buffer[0]++;
		OSSemPost(Semp2);
		//OS_Printf("�����������-%d  \n", *ss);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
void youxiangchuli(void* p_arg) {
	while (TRUE)
	{
		OSSemPend(Semp2, 0, &err);
		OS_Printf("���δ�ȼ�ϲ�-%d  \n", buffer[0]);
	}
}
int main()
{
	OSInit();    /* Initialize "uC/OS-II"*/
	Sem_F = OSFlagCreate(0, &err); //��ʼ���ź�����
	Semp = OSQCreate(msgPtr, 5);//��Ϣ����
	Sempyx = OSMboxCreate((void*)0); // ��������
	//Sempyx = OSMboxCreate((void*)0); // ��������
	//Sempyx = OSMboxCreate((void*)0); // ��������
	//Sempyx = OSMboxCreate((void*)0); // ��������
	//Sempyx = OSMboxCreate((void*)0); // ��������

	intBuffer = OSMemCreate(intPart, 50, 64, &err);//��ʼ���ڴ����
	Semp2 = OSSemCreate(0); //�����ź��� ����ֵΪ�������ź���ָ�룬�������ź����ļ�������ֵ
	Semp3 = OSMutexCreate(0, &err);;//���������ź���

	OSTaskCreate(App_gaodu_kongzhi, 0, (OS_STK*)&AppStk_1[TASK_STK_SIZE - 1], (INT8U)10);
	OSTaskCreate(App_wendu_kongzhi, 0, (OS_STK*)&AppStk_2[TASK_STK_SIZE - 1], (INT8U)11);
	OSTaskCreate(App_youxiang_caiji, 0, (OS_STK*)&AppStk_3[TASK_STK_SIZE - 1], (INT8U)12);
	OSTaskCreate(App_youxiang_kongzhi, 0, (OS_STK*)&AppStk_4[TASK_STK_SIZE - 1], (INT8U)13);
	OSTaskCreate(App_Socket, 0, (OS_STK*)&AppStk_5[TASK_STK_SIZE - 1], (INT8U)44);
	OSTaskCreate(zhiling, 0, (OS_STK*)&AppStk_6[TASK_STK_SIZE - 1], (INT8U)40);
	OSTaskCreate(shengyicaiji, 0, (OS_STK*)&AppStk_9[TASK_STK_SIZE - 1], (INT8U)32);
	OSTaskCreate(shengyichuli, 0, (OS_STK*)&AppStk_10[TASK_STK_SIZE - 1], (INT8U)33);
	OSTaskCreate(App_youxiang_caiji, 0, (OS_STK*)&AppStk_11[TASK_STK_SIZE - 1], (INT8U)34);
	OSTaskCreate(App_youxiang_kongzhi, 0, (OS_STK*)&AppStk_12[TASK_STK_SIZE - 1], (INT8U)35);
	OSTaskCreate(App_sport_caiji, 0, (OS_STK*)&AppStk_13[TASK_STK_SIZE - 1], (INT8U)36);
	OSTaskCreate(App_fadonji_caiji, 0, (OS_STK*)&AppStk_14[TASK_STK_SIZE - 1], (INT8U)37);
	OSTaskCreate(App_sport_kongzhi, 0, (OS_STK*)&AppStk_15[TASK_STK_SIZE - 1], (INT8U)38);
	OSTaskCreate(App_fadonji_kongzhi, 0, (OS_STK*)&AppStk_16[TASK_STK_SIZE - 1], (INT8U)39);
	OSTaskCreate(youxiangchuli, 0, (OS_STK*)&AppStk_17[TASK_STK_SIZE - 1], (INT8U)40);
	OSTaskCreate(sportchuli, 0, (OS_STK*)&AppStk_18[TASK_STK_SIZE - 1], (INT8U)41);
	OSTaskCreate(shengyicaiji, 0, (OS_STK*)&AppStk_19[TASK_STK_SIZE - 1], (INT8U)42);
	OSTaskCreate(fadonjichuli, 0, (OS_STK*)&AppStk_20[TASK_STK_SIZE - 1], (INT8U)43);
	OSTaskCreate(gps1, 0, (OS_STK*)&AppStk_7[TASK_STK_SIZE - 1], (INT8U)44);
	OSTaskCreate(gps2, 0, (OS_STK*)&AppStk_8[TASK_STK_SIZE - 1], (INT8U)45);


	OSStart();                             /* Start multitasking */
	return 0;
}