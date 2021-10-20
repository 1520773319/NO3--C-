#include<stdio.h>
#include<iostream>
#include<map>
#include<stack> 
#include<string>
#include<queue> 
#define INFINITY INT_MAX     //���ֵ �� 
#define MAX_VERTEX_NUM 20   //��󶥵��� 
#define OK 1
#define ERROR 0
typedef int Status;
using namespace std;


typedef struct ArcCell{       //�� 
	int adj;                  // ������Ȩͼ����01��ʾ������񣻶�����Ȩͼ��ʾȨֵ 
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; 
typedef struct{
	string vex[MAX_VERTEX_NUM];  //��������
	AdjMatrix arcs;              //�ڽӾ��� 
	int vexnum,arcnum;           //������������
	int kind;              //ͼ������ 
}Graph;

int LocateVex(Graph G , string v);
Status CreateGraph(Graph &G);
Status CreateDG(Graph &G);
Status CreateUDG(Graph &G);
void ShowGraph(Graph G);
Status Prim(Graph G);
void DFSTraverse(Graph G);
void DFS(Graph G, string v);
void BFSTraverse(Graph G);


int LocateVex(Graph G , string v){ 
	for(int i=0;i<G.vexnum;i++)
		if(G.vex[i]==v)
			return i;
}

void ShowGraph(Graph G){
	for(int i=0;i<G.vexnum;i++){
		cout<<endl;
		for(int j=0;j<G.vexnum;j++){
			if(G.arcs[i][j].adj==INFINITY)
				cout<<"��"<<"\t"; 
			else 
				cout<<G.arcs[i][j].adj<<"\t";
		}
	}
}

Status CreateGraph(Graph &G){
	cout<<"����ͼ�����ࣨ0����ͼ��1����ͼ����";
	cin>>G.kind;
	switch(G.kind){
		case 0:return CreateDG(G);
		case 1:return CreateUDG(G);
		default:return ERROR;
	}
	return OK;
}

Status CreateUDG(Graph &G){
	cout<<"���붥������"; 
	cin>>G.vexnum;
	cout<<"�������ж�������"; 
	for(int i=0;i<G.vexnum;i++){  //��ʼ���������� 
		cin>>G.vex[i];
	}
	cout<<"���������"; 
	cin>>G.arcnum;
	
	for(int i=0;i<G.vexnum;i++){
		for(int j=0;j<G.vexnum;j++){
			G.arcs[i][j].adj=INFINITY;      //��ʼ����Ȩֵ��INFINITY��ʾ���������� 
			G.arcs[j][i].adj=INFINITY;
		} 
	}
	
	string v1,v2;
	int w;
	for(int k=0;k<G.arcnum;k++){
		cout<<"�����"<<k+1<<"���������Ķ��㼰Ȩֵ��" ;
		cin>>v1>>v2>>w; 
		int i=LocateVex(G,v1);
		int j=LocateVex(G,v2);
		G.arcs[i][j].adj=w;
		G.arcs[j][i].adj=w;
	}
	return OK;
}

Status CreateDG(Graph &G){
	cout<<"���붥������"; 
	cin>>G.vexnum;
	cout<<"�������ж�������"; 
	for(int i=0;i<G.vexnum;i++){  //��ʼ���������� 
		cin>>G.vex[i];
	}
	cout<<"���������"; 
	cin>>G.arcnum;
	
	for(int i=0;i<G.vexnum;i++){
		for(int j=0;j<G.vexnum;j++){
			G.arcs[i][j].adj=INFINITY;      //��ʼ����Ȩֵ��INFINITY��ʾ���������� 
			G.arcs[j][i].adj=INFINITY;
		} 
	}
	
	string v1,v2;
	int w;
	for(int k=0;k<G.arcnum;k++){
		cout<<"�����"<<k+1<<"���������Ķ��㼰Ȩֵ��" ;
		cin>>v1>>v2>>w; 
		int i=LocateVex(G,v1);
		int j=LocateVex(G,v2);
		G.arcs[i][j].adj=w;
	}
	return OK;
}

/********************************************************************************/
// prim ��С������ 
typedef struct PrimVexsInfo{
	string name;
	bool isjoin=false;
	int mincost=INFINITY;
	string minCostVex="null";   //�ö�������������������СȨֵ�ı��ϵ���һ���� 
}PrimVexs[MAX_VERTEX_NUM];

void RenewMincost(Graph G,PrimVexs &VexInfo){    //����mincost��mincostVex; 
	for(int i=0;i<G.vexnum;i++){
		if(!VexInfo[i].isjoin){
			for(int j=0;j<G.vexnum;j++){
				if(VexInfo[j].isjoin && VexInfo[i].mincost>G.arcs[i][j].adj){
					VexInfo[i].mincost=G.arcs[i][j].adj;
					VexInfo[i].minCostVex=G.vex[j];
				}
			}
		}
	}
}

int LocateMinCostVex(Graph G, PrimVexs VexInfo){  //�ҵ�δ��������cost��С�Ķ��㣬����������.
	int cost=INFINITY;
	int index=0;
	int i=0;
	while(i<G.vexnum){
		if(VexInfo[i].isjoin)
			i++;
		else{
			if(cost>VexInfo[i].mincost){
				cost=VexInfo[i].mincost;
				index=i;
			}
			i++;
		}
	}
	return index;
}

Status Prim(Graph G){
	if(G.kind!=1){
		cout<<"���󣡷�����ͼ��"; 
		return ERROR;
	}
	PrimVexs VexInfo;
	for(int i=0;i<G.vexnum;i++){
		VexInfo[i].name=G.vex[i];  //��ʼ��prim��������
	}
		
	VexInfo[0].isjoin=true;         //�����V0��ʼ,��V0����������С������ 
	VexInfo[0].mincost=0;
	RenewMincost(G,VexInfo);
	
	for(int i=0;i<G.vexnum-1;i++){  //����㹲��Ҫ G.vexnum-1���� 
		int index=LocateMinCostVex(G, VexInfo);
		VexInfo[index].isjoin=true;
		VexInfo[index].mincost=0;
		RenewMincost(G,VexInfo);
	} 
	
	for(int i=0;i<G.vexnum;i++)
		if(VexInfo[i].minCostVex!="null")
			cout<<VexInfo[i].name<<"----"<<VexInfo[i].minCostVex<<endl;
	
	return OK;
}

/********************************************************************************/
// DFS 
map<string,bool> visited; //���ʱ�־����

void DFS(Graph G, string v){
	visited[v]=true;
	cout<<v<<" ";
	int indexV=LocateVex(G,v);
	for(int j=0;j<G.vexnum;j++){
		if(!visited[G.vex[j]] && G.arcs[indexV][j].adj!=INFINITY)
			DFS(G,G.vex[j]);
	}
}

void DFSTraverse(Graph G){
	for(int i=0;i<G.vexnum;i++)   //��ʼ��visited 
		visited.insert({G.vex[i],false});
	
	cout<<endl<<endl<<"��v0��ʼ��DFS���У�";
	for(int i=0;i<G.vexnum;i++){
		if(!visited[G.vex[i]])
			DFS(G,G.vex[i]);
	}
} 

/********************************************************************************/
// BFS 
void BFSTraverse(Graph G){
	for(int i=0;i<G.vexnum;i++)   //��ʼ��visited 
		visited.insert({G.vex[i],false});
		
	cout<<endl<<endl<<"��v0��ʼ��BFS����Ϊ:";
	queue<string> Queue;
	for(int i=0;i<G.vexnum;i++){   //��v0��ʼ��BFS���� 
		if(!visited[G.vex[i]]) {
			Queue.push(G.vex[i]);
			visited[G.vex[i]]=true;
		}
		
		while(!Queue.empty()){
			string cur=Queue.front();
			Queue.pop();
			cout<<cur<<" ";
			int curindex=LocateVex(G,cur);
			for(int j=0;j<G.vexnum;j++){
				if(!visited[G.vex[j]] && G.arcs[curindex][j].adj!=INFINITY){
					Queue.push(G.vex[j]);
					visited[G.vex[j]]=true;
				}
			}
		}	
	}    
}

/********************************************************************************/


int main(){
	Graph G;
	CreateGraph(G);
	ShowGraph(G);
}
