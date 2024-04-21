#ifndef BRIDGE_H
#define BRIDGE_H

#include "system.h"
#include "copyright.h"
#include "synch.h"

enum BridgeStatus {BUSYL,BUSYR, FREE};//�ֱ��ʾ������ߵĳ������ţ����ұߵĳ������š����ڿ���״̬��

class Bridge
{

	public:
        Bridge();
        ~Bridge();
        void ArriveBridge(int direc);//������
        void CrossBridge(int direc);//����
        void ExitBridge(int direc);//����
    private:
        Lock *lock;
        BridgeStatus status;//�ſ�������״̬���ֱ�ΪFREE,BUSYL,BUSYR
        Condition *BridgeBusyL;
        Condition *BridgeBusyR;
        int count;	//���ڹ��ŵĳ�������
        int valuel;  //�ȴ�������ߵĳ�������
        int valuer;	 //�ȴ������ұߵĳ�������
        bool flag;	//flagΪtrueʱ����������г��ڹ��Ų��Ҳ�������ʱ�������ͬ����ĳ��������ɸ��Ź��ţ�
                   //flagΪfalseʱ����������г������Ҳ�����������ͬ����ĳ�����ʱ��Ҳ��Ҫ�ȴ���
                   //����Ϊ��ʵ�����ߵĳ��ܽ���Ĺ�����ֹ�˼����Ĳ�����
};
//�ŵ������Ҳ೵��������ţ���ͬһ�����������������ϵĳ���ʱ����ͬʱ�����������ţ�
//Ȼ�����ŵ���һ�����ϵȴ�����������������û��һ���������ѱ�����ĳ�����
//��������������ʱ������������µ���ĳ����Ź��ţ�����ȵ����������껻����һ�������һ�κ������

#endif
