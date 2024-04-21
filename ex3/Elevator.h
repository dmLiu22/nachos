/*

Here are the method signatures for the Elevator and Building classes.
You should feel free to add to these classes, but do not change the
existing interfaces.

*/
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "system.h"
#include "synch.h"
#include "EventBarrier.h"

#define UP 1
#define SLEEP 0
#define DOWN -1

class Elevator
{
   public:
     Elevator(char *debugName,int numFloors, int myID);
     ~Elevator();
     char *getName() { return name; }

     // elevator control interface: called by Elevator thread
     void OpenDoors();                // ���ݿ��ţ����뿪�ߺͽ����߷��źű�ʾҪ��ʼ�ж���
     void CloseDoors();               // �������뿪���µ��ݡ��������ϵ��ݺ����
     void VisitFloor(int floor);      // ����ȥָ��¥��

     // elevator rider interface (part 1): called by rider threads.
     bool Enter();                    //  �������
     void Exit();                     //  �뿪����
     void RequestFloor(int floor);    //  ���ߵ����Լ�Ҫȥ��¥��

     // insert your methods here, if needed
     void StartElevator(); // ��������

   //private:
   public:
     char *name;
     int currentfloor;		// ����Ŀǰͣ����¥��
     int occupancy;		// ����ʵ������

     // insert your data structures here, if needed
     int NumFloors;		// ¥����
     int ID;			// �������
     int status;		// ����״̬�����ϡ����¡���ͣ
     int Capacity;		// ���ݿ���������
     bool overload;		// �����Ƿ���
     EventBarrier **exitBarrier;//ȷ��Ҫ�µ��ݵĳ˿�ȫ���뿪
     bool *exitRequest;//�뿪����
};



class Building
{
   public:
     Building(char *debugname, int numFloors, int numElevators);
     ~Building();
     char *getName() { return name; }


     // elevator rider interface (part 2): called by rider threads
     void CallUp(int fromFloor);      // �����ź�Ҫ�ӵ�ǰ¥�������������
     void CallDown(int fromFloor);    // �����ź�Ҫ�ӵ�ǰ¥�������������
     Elevator *AwaitUp(int fromFloor); // �ڵ�ǰ¥��ȴ����ݵ���������
     Elevator *AwaitDown(int fromFloor); // �ڵ�ǰ¥��ȴ����ݵ���������

     void RecordCall(int fromFloor, int direction);	// ��¼�˿͵�����
     int HandleCall();	// ���ݴ����һ�����󣬸����ȵ��ȷ���ԭ��
     void HandleFloor(int fromFloor, int direction);// ������������д��ڵ�ǰ¥����ͬ���������

   //private:
   public:
     char *name;
     char **elevatorname;  //������
     Elevator **elevator;	// ����

     // insert your data structures here, if needed
     int NumFloors;		// ¥����
     int NumElevators;  // ������
     EventBarrier **upBarrier;	// ȷ��ͬ¥�����г˿ͳ˵��ݵ���¥
     EventBarrier **downBarrier;// ȷ��ͬ¥�����г˿ͳ˵��ݵ���¥
     bool *callUp, *callDown;	// ��ĳһ¥���Ƿ�������е��ݣ����ϡ����£�
     int *elevatorUp, *elevatorDown;	// ��¼�������µĵ��ݵ����
     Condition *suspend_cond;	// �յĵ��ݱ���������������suspend_cond��
     int *callQueue;// ���ڼ�¼����rider�����󣬶�����ÿ��Ԫ�ص�ֵΪrider���ڵ�¥��ͷ���ĳ˻���
                    //��ֵ��ʾ���ϣ���ֵ��ʾ���¡�
     int end;		// ���callQueue�Ľ�β
};

/*��ĳһ��¥��һ������һ�����ϵĳ˿�Ҫ��������ʱ������һ�������źŸ����ݣ����ݱ���Ӧ����������Ӧ�ĵ���״̬��
�����ƶ������г˿ͱ���������Ӧ¥�㣬�����ݵ���󣬵��ݿ��ţ������źŽ��˿ʹ��¼�դ�����ͷţ������Լ�������
�˿ͽ�����ݻ��߳����ݣ����˿������һ�������ݵĻ�����ݵ��򷢳��źŻ��ѵ��ݡ�
Ȼ����ݹ��ţ��˿��ڵ���������Ҫȥ��Ŀ��¥�㣬��������ֱ�������ݵ�����Ҫȥ��Ŀ��¥��ʱ��
���ݿ��ţ����˿ʹ����������ͷš��˿ͳ����ݣ����ˣ�һ���˿��߳̽�����*/


#endif
