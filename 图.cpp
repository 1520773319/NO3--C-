#include<stdio.h>
#include<iostream>
#include<map>
#include<stack> 
#include<string>
#include<queue> 
#define INFINITY INT_MAX     //最大值 ∞ 
#define MAX_VERTEX_NUM 20   //最大顶点数 
#define OK 1
#define ERROR 0
typedef int Status;
using namespace std;


typedef struct ArcCell{       //边 
	int adj;                  // 对于无权图，用01表示连接与否；对于有权图表示权值 
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; 
typedef struct{
	string vex[MAX_VERTEX_NUM];  //顶点向量
	AdjMatrix arcs;              //邻接矩阵 
	int vexnum,arcnum;           //顶点数、边数
	int kind;              //图的种类 
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
				cout<<"∞"<<"\t"; 
			else 
				cout<<G.arcs[i][j].adj<<"\t";
		}
	}
}

Status CreateGraph(Graph &G){
	cout<<"输入图的种类（0有向图，1无向图）：";
	cin>>G.kind;
	switch(G.kind){
		case 0:return CreateDG(G);
		case 1:return CreateUDG(G);
		default:return ERROR;
	}
	return OK;
}

Status CreateUDG(Graph &G){
	cout<<"输入顶点数："; 
	cin>>G.vexnum;
	cout<<"输入所有顶点名："; 
	for(int i=0;i<G.vexnum;i++){  //初始化顶点向量 
		cin>>G.vex[i];
	}
	cout<<"输入边数："; 
	cin>>G.arcnum;
	
	for(int i=0;i<G.vexnum;i++){
		for(int j=0;j<G.vexnum;j++){
			G.arcs[i][j].adj=INFINITY;      //初始化边权值，INFINITY表示两点无连接 
			G.arcs[j][i].adj=INFINITY;
		} 
	}
	
	string v1,v2;
	int w;
	for(int k=0;k<G.arcnum;k++){
		cout<<"输入第"<<k+1<<"条边依附的顶点及权值：" ;
		cin>>v1>>v2>>w; 
		int i=LocateVex(G,v1);
		int j=LocateVex(G,v2);
		G.arcs[i][j].adj=w;
		G.arcs[j][i].adj=w;
	}
	return OK;
}

Status CreateDG(Graph &G){
	cout<<"输入顶点数："; 
	cin>>G.vexnum;
	cout<<"输入所有顶点名："; 
	for(int i=0;i<G.vexnum;i++){  //初始化顶点向量 
		cin>>G.vex[i];
	}
	cout<<"输入边数："; 
	cin>>G.arcnum;
	
	for(int i=0;i<G.vexnum;i++){
		for(int j=0;j<G.vexnum;j++){
			G.arcs[i][j].adj=INFINITY;      //初始化边权值，INFINITY表示两点无连接 
			G.arcs[j][i].adj=INFINITY;
		} 
	}
	
	string v1,v2;
	int w;
	for(int k=0;k<G.arcnum;k++){
		cout<<"输入第"<<k+1<<"条边依附的顶点及权值：" ;
		cin>>v1>>v2>>w; 
		int i=LocateVex(G,v1);
		int j=LocateVex(G,v2);
		G.arcs[i][j].adj=w;
	}
	return OK;
}

/********************************************************************************/
// prim 最小生成树 
typedef struct PrimVexsInfo{
	string name;
	bool isjoin=false;
	int mincost=INFINITY;
	string minCostVex="null";   //该顶点与生成树相连且最小权值的边上的另一顶点 
}PrimVexs[MAX_VERTEX_NUM];

void RenewMincost(Graph G,PrimVexs &VexInfo){    //更新mincost、mincostVex; 
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

int LocateMinCostVex(Graph G, PrimVexs VexInfo){  //找到未被纳入且cost最小的顶点，返回其坐标.
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
		cout<<"错误！非无向图！"; 
		return ERROR;
	}
	PrimVexs VexInfo;
	for(int i=0;i<G.vexnum;i++){
		VexInfo[i].name=G.vex[i];  //初始化prim顶点名字
	}
		
	VexInfo[0].isjoin=true;         //假设从V0开始,即V0最先纳入最小生成树 
	VexInfo[0].mincost=0;
	RenewMincost(G,VexInfo);
	
	for(int i=0;i<G.vexnum-1;i++){  //其余点共需要 G.vexnum-1纳入 
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
map<string,bool> visited; //访问标志数组

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
	for(int i=0;i<G.vexnum;i++)   //初始化visited 
		visited.insert({G.vex[i],false});
	
	cout<<endl<<endl<<"从v0开始的DFS序列：";
	for(int i=0;i<G.vexnum;i++){
		if(!visited[G.vex[i]])
			DFS(G,G.vex[i]);
	}
} 

/********************************************************************************/
// BFS 
void BFSTraverse(Graph G){
	for(int i=0;i<G.vexnum;i++)   //初始化visited 
		visited.insert({G.vex[i],false});
		
	cout<<endl<<endl<<"从v0开始的BFS序列为:";
	queue<string> Queue;
	for(int i=0;i<G.vexnum;i++){   //从v0开始的BFS序列 
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
