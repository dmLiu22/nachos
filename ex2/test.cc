#include "dllist.cpp"
#include<iostream>
#include <cstdlib> 
#include <ctime>
using namespace std;

void Random(int *a,int n,int l,int r)//生成范围在l~r的随机数 
{
	srand(time(0));  //设置时间种子
	for(int i=0;i<n;i++){
		a[i]=rand()%(r-l+1)+l;//生成区间r~l的随机数 
	}
}
int main(){
    cout << "创建链表" << endl;
    int a[8];
    Random(a,8,1,100);
    for(int i = 0; i < 8; i ++){
        cout << a[i] << " ";
    }
    cout << endl;
    DLList list;
    for (int i = 0; i < 8; i++)
    {
        void *item;
        int key=a[i];
        int *items=new int[1];
        items[0]=-key;
        item=items;
        printf("in:%d %d\n",key,*(int*)item);
        list.SortedInsert(item,key);
    }
    DLLElement *node = list.first;
    while(node != NULL){
        cout << node->key << " ";
        node = node -> next;
    }
    cout << endl;
    cout << "末尾插入" << endl;
    void *item;
    int *items=new int[1];
    items[0]=-40;
    list.Append(items);
    node = list.first;
    while(node){
        cout << node->key << " ";
        node = node -> next;
    }
    cout << endl;
    cout << "开头插入" << endl;
    list.Prepend(items);
    node = list.first;
    while(node){
        cout << node->key << " ";
        node = node -> next;
    }
    cout << endl;
    cout << "指定删除30" << endl;
    list.SortedRemove(30);
    node = list.first;
    while(node){
        cout << node->key << " ";
        node = node -> next;
    }
    cout << endl;
    cout << "删除开头" << endl;
    int key;
    item=list.Remove(&key);
    node = list.first;
    while(node){
        cout << node->key << " ";
        node = node -> next;
    }
    cout << endl;
    return 0;
}