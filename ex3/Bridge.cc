#include "Bridge.h"
#include "Alarm.h"
Bridge::Bridge()
{
    lock = new Lock("Bridge lock");
    status = FREE;
    BridgeBusyL = new Condition("Bridge busyl cond");
    BridgeBusyR = new Condition("Bridge busyr cond");
    count = 0;
	valuel = 0;
	valuer = 0;
	flag = true;
}

Bridge::~Bridge()
{
    delete lock;
    delete BridgeBusyL;
    delete BridgeBusyR;
}

void Bridge::ArriveBridge(int direc)
{
    lock->Acquire();
	printf("\033[40;35m 小汽车%s号 \033[0m到达, 它来自方向: %d,到达时刻为：\033[40;32m %d \033[0m.\n",currentThread->getName(),direc,stats->totalTicks);
    if (direc == 0) //车从左边来
	{
	    if(status == FREE)
		    status = BUSYL;
		else if (status == BUSYL) //桥上正有车从左边过
		{
		    if (count < 3 && flag == true) ; //车的数量小于3，并且flag为1，即桥上不多于三个车的时候同方向的车可以经过
			else //与上面情况相反，总之这个车不能同行
			{
			    valuel++;//左边等待的车的数量加一
				printf("\033[40;35m 小汽车%s号 \033[0m等待.\n",currentThread->getName());
			    BridgeBusyL->Wait(lock);//等待过桥信号
			}
		}
		else   //桥上正有车从右边过
		{
		    valuel++;
			printf("\033[40;35m 小汽车%s号 \033[0m等待.\n",currentThread->getName());
		    BridgeBusyL->Wait(lock);//等待过桥信号
		}
	}
	else//车从右边来
	{
	    if(status == FREE)
		    status = BUSYR;
		else if (status == BUSYR)
		{
		    if (count < 3 && flag == true);
			else
			{
			    valuer++;
				printf("\033[40;35m 小汽车%s号 \033[0m等待.\n",currentThread->getName());
			    BridgeBusyR->Wait(lock);
			}
		}
		else
		{
		    valuer++;
			printf("\033[40;35m 小汽车%s号 \033[0m等待.\n",currentThread->getName());
		    BridgeBusyR->Wait(lock);//等待过桥信号
		}
	}
	count++;//正在过桥的车的数量加一
	lock->Release();
}

void Bridge::CrossBridge(int direc)
{
    printf("\033[40;35m 小汽车%s号 \033[0m上桥, 它来自方向: %d,上桥时刻为\033[40;32m %d \033[0m.\n",currentThread->getName(),direc,stats->totalTicks);
	//currentThread->Yield();

}

void Bridge::ExitBridge(int direc)
{
    lock->Acquire();
	count--;
	flag = false;//
    if (direc == 0)//车来自左边
	{
	    if (valuer > 0)//右边有车在等待
		{
		    if (count == 0)//这个车是桥上最后一个车，
			{
			    status = FREE;
				flag = true;
	            for (int i = 0;i < 3 && valuer > 0;i++)//右边的车上桥，三个
				{
					valuer--;
		            BridgeBusyR->Signal(lock);
				}
			}
		}
		else//右边没有车在等待
		{
		    if (count == 0)//这个车是桥上最后一个车
			{
			    status = FREE;
				flag = true;
	            for (int i = 0;i < 3 && valuel > 0;i++)//左边的车上桥
				{
				    valuel--;
		            BridgeBusyL->Signal(lock);
				}
			}
		}
	}
	else
	{
	    if (valuel > 0)
		{
	        if (count == 0)
			{
			    status = FREE;
	            for (int i = 0;i < 3 && valuel > 0;i++)
				{
				    valuel--;
		            BridgeBusyL->Signal(lock);
				}
			}
		}
		else
		{
		    if (count == 0)
			{
			    status = FREE;
	            for (int i = 0;i < 3 && valuer > 0;i++)
				{
					valuer--;
					BridgeBusyR->Signal(lock);
				}
			}
		}
	}
	printf("\033[40;35m 小汽车%s号 \033[0m离开,离开时刻为\033[40;32m %d \033[0m.\n",currentThread->getName(),stats->totalTicks);
	lock->Release();
}

