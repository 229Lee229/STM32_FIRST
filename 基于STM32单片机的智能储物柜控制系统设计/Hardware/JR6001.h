/**********************************************************************
 * flie:  PWM
 * brief: ???PWM  TIM1 TIM2 TIM3 
 * Input: ?
 * Output:?
 * Retval:?
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 3/30/2024	  Lee		
 ***********************************************************************/
#ifndef __JR6001_H
#define __JR6001_H

#include "My_include.h"


#define UART1_JR6001                USART1
#define PIN_JR6001_BUSY             PA4

#define CMD_JR6001_PLAY             "A2"//����
#define CMD_JR6001_PAUSE            "A3"//��ͣ
#define CMD_JR6001_STOP             "A4"//ֹͣ
#define CMD_JR6001_LAST             "A5"//��һ��
#define CMD_JR6001_NEXT             "A6"//��һ��
#define CMD_JR6001_SELECT           "A7:"//ָ���ļ�������
#define CMD_JR6001_SELECT_FLASH     "A8:02"//ָ��FLASH��·�����ļ�������
#define CMD_JR6001_SELECT_TF        "A8:01"//ָ��TF����·�����ļ�������
#define CMD_JR6001_SELECT_UDISK     "A8:00"//ָ��U����·�����ļ�������
#define CMD_JR6001_END_PLAY			"AE"	// ��������
#define CMD_JR6001_VOLUME_SET       "AF:"//���������ȼ�
#define CMD_JR6001_VOLUME_UP      	"B0"//������
#define CMD_JR6001_VOLUME_DOWN      "B1"//������
#define CMD_JR6001_MODE_SET         "B4:"//���ò���ģʽ
#define CMD_JR6001_PLAY_GROUP       "B7:"//��ϲ��� ZH �ļ����ڵ��ļ�
#define CMD_JR6001_STOP_GROUP       "B8"//��ϲ��� ZH �ļ����ڵ��ļ�




#define JR6001_VOLUME_MAX			"AF:30\r\n"
#define JR6001_VOLUME_MID			"AF:20\r\n"
#define JR6001_VOLUME_MIN			"AF:10\r\n"

#define JR6001_PleaseInputPWD		"A7:00001\r\n"		/* ���������� */
#define JR6001_PLAY_OPEN_WEL		"A7:00002\r\n"		/* ���ܴ����ӭ�� */
#define JR6001_1ST_OPENED			"A7:00003\r\n"		/* һ�Ź����ѿ� */
#define JR6001_2ND_OPENED			"A7:00004\r\n"		/* ���Ź����ѿ� */
#define JR6001_3RD_OPENED			"A7:00005\r\n"		/* ���Ź����ѿ� */
#define JR6001_4TH_OPENED			"A7:00006\r\n"		/* �ĺŹ����ѿ� */				
#define JR6001_5TH_OPENED			"A7:00007\r\n"		/* ��Ź����ѿ� */
#define JR6001_PWD_ERROR			"A7:00008\r\n"		/* �������  */
#define JR6001_SG90_CLOSED			"A7:00009\r\n"		/* �����ѹر�*/
#define JR6001_INPUT_PHONE			"A7:00010\r\n"		/* �����������ֻ��� */
#define JR6001_INPUT_CODE			"A7:00011\r\n"		/* ����������ȡ���� */
#define JR6001_DATA_CLEARED			"A7:00012\r\n"		/* ��������� */
#define JR6001_DATA_GENERATE		"A7:00013\r\n"		/* �洢��Ϣ������ */
#define JR6001_SCAN_QR				"A7:00014\r\n"		/* ��ɨ���ά�� */
#define JR6001_STORE_FAIL			"A7:00015\r\n"		/* ���ʧ��,�޿��ÿռ� */
#define JR6001_PICK_FAIL			"A7:00016\r\n"		/* ȡ��ʧ��,�޴洢���� */
#define JR6001_LINK_BULETOOTH		"A7:00017\r\n"		/* ����������ȡ�� */
typedef enum
{
	JR6001_FULL_CYCLE = 0,		// ȫ��ѭ��
    JR6001_CYCLE_SINGEL=1,		// ����ѭ��
    JsR6001_ONCE_SINGEL,		// ����ֹͣ ��������һ��
    JR6001_RANDOM_ALL,			// ȫ�����
	JR6001_ORDER_ALL,			// Ŀ¼ѭ�� ȫ��˳�򲥷�
    JR6001_RANDOM_DIR,			// Ŀ¼��� Ŀ¼����� ������Ŀ¼
    JsR6001_ORDER_DIR,			// Ŀ¼˳�򲥷� �������ֹͣ Ŀ¼��˳�򲥷�
	JsR6001_ORDER_ALL,			// ȫ�̲��� �������ֹͣ 


}PlayMode_JR6001;//����ģʽ


#define JR6001_SEND_CMD(n)       USART_SendString(UART1_JR6001,n)
#define JR6001_Play()            JR6001_SEND_CMD(CMD_JR6001_PLAY)
#define JR6001_Stop()            JR6001_SEND_CMD(CMD_JR6001_STOP)
#define JR6001_Pause()           JR6001_SEND_CMD(CMD_JR6001_PAUSE)
#define JR6001_LAST()        	 JR6001_SEND_CMD(CMD_JR6001_LAST)
#define JR6001_Next()            JR6001_SEND_CMD(CMD_JR6001_NEXT)
#define JR6001_VolumeUp()        JR6001_SEND_CMD(CMD_JR6001_VOLUME_UP)
#define JR6001_VolumeDown()      JR6001_SEND_CMD(CMD_JR6001_VOLUME_DOWN)
#define JR6001_StopGroup()       JR6001_SEND_CMD(CMD_JR6001_STOP_GROUP)

char* My_JR6001_select_order(u8 order);
void My_JR6001_Init(u8 volume);//��ʼ��
void My_JR6001_PlayByName(const char *name);
void My_JR6001_SetVolume(u8 level);
bool My_JR6001_IsBusy(void);
void My_JR6001_SetPlayMode(PlayMode_JR6001 mode);
void My_JR6001_PlayGroup(int count, ...);

#endif

