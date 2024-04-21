#include "dllist.cpp"
#include<iostream>
#include <cstdlib> 
#include <ctime>
using namespace std;

void Random(int *a,int n,int l,int r)//���ɷ�Χ��l~r������� 
{
	srand(time(0));  //����ʱ������
	for(int i=0;i<n;i++){
		a[i]=rand()%(r-l+1)+l;//��������r~l������� 
	}
}
int main(){
    cout << "��������" << endl;
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
    cout << "ĩβ����" << endl;
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
    cout << "��ͷ����" << endl;
    list.Prepend(items);
    node = list.first;
    while(node){
        cout << node->key << " ";
        node = node -> next;
    }
    cout << endl;
    cout << "ָ��ɾ��30" << endl;
    list.SortedRemove(30);
    node = list.first;
    while(node){
        cout << node->key << " ";
        node = node -> next;
    }
    cout << endl;
    cout << "ɾ����ͷ" << endl;
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