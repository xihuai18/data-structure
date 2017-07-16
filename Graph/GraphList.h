#ifndef __GRAPHLIST__
#define __GRAPHLIST__

#include "../List/List.h"
#include "Graph.h"

template <typename Tv> struct Vertex { //顶点对象（为简化起见，并未严格封装）
   Tv data; int inDegree, outDegree; VStatus status; //数据、出入度数、状态
   int dTime, fTime; //时间标签
   int parent; int priority; //在遍历树中的父节点、优先级数
   Vertex ( Tv const& d = ( Tv ) 0 ) : //构造新顶点
      data ( d ), inDegree ( 0 ), outDegree ( 0 ), status ( UNDISCOVERED ),
      dTime ( -1 ), fTime ( -1 ), parent ( -1 ), priority ( INT_MAX ) {} //暂不考虑权重溢出
};

template <typename Te> struct Edge { //边对象（为简化起见，并未严格封装）
   Te data; int weight; EType status; //数据、权重、类型
   Edge ( Te const& d, int w ) : data ( d ), weight ( w ), status ( UNDETERMINED ) {} //构造
};

template <typename Tv, typename Te>
struct EAndV
{
	Vertex<Tv>* pV;
	Edge<Te>* pE;
	EAndV(Vertex<Tv> * V, Edge<Te> * E)
	:pV(V), pE(E){}
	~EAndV() { delete pV; delete pE; }
}; 

template <typename Tv, typename Te>
class GraphList:public Graph<Tv, Te>
{
	using Graph<Tv, Te>::n;
	using Graph<Tv, Te>::e;
private:
	List<List<EAndV<Tv, Te>*>> Adj;
	/*
	每个链表的第一个元素为该顶点，后面的元素为从该顶点出发的边的终点。
	A->B->C
	B->A
	*/
public:
	GraphList(){
		e = n = 0;
	}
	~GraphList(){
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < e; ++j)
				delete Adj[i][j];
	}
	// 点
	virtual int insert(Tv const& v)
	{
	  auto newEAndV = new EAndV<Tv, Te>(new Vector<Tv>(v), nullptr);
	  List<EAndV<Tv, Te>*> tmp;
	  tmp.insertAsFirst(newEAndV);
	  Adj.insertAsLast(tmp);
	  return Adj.size() - 1;
	}//插入顶点，返回编号
	virtual Tv remove(int i)
	{
	  EAndV<Tv, Te>* V = Adj[i][0];
	  auto tmp = Adj.search(Adj[i]);
	  Adj.remove(tmp);
	  for (int i = 0; i < Adj.size(); i++)
	  {
		for (int j = 0; j < Adj[i].size(); ++j)
		  if (Adj[i][j]->pV == V->pV)
			Adj[i].remove(Adj[i].search(Adj[i][j]));

	  }
	  Tv ret = V->pV->data;
	  delete V;
	  return ret;
	}//删除顶点及其关联边，返回该顶点信息
	virtual Tv& vertex ( int i) { return Adj[i][0]->pV->data; } //顶点v的数据（该顶点的确存在）
	virtual int inDegree ( int i) { return Adj[i][0]->pV->inDegree; } //顶点v的入度（该顶点的确存在）
	virtual int outDegree ( int i) { return Adj[i][0]->pV->outDegree; } //顶点v的出度（该顶点的确存在）
	virtual int firstNbr(int i) {
	  for (int j = 0; j < Adj.size(); j++)
	  {
		if (Adj[j][0]->pV == Adj[i][1]->pV)
		  return j;
	  }
	}//顶点v的首个邻接顶点
	virtual int nextNbr(int i, int j)
	{
	  int n;
	  for (int x = 0; x < Adj[i].size(); x++)
	  {
		if (Adj[i][x]->pV == Adj[j][0]->pV)
		  n = x + 1;
	  }
	  for (int z = 0; z < Adj.size(); z++)
	  {
		if (Adj[z][0]->pV == Adj[i][n]->pV)
		  return z;
	  }
	}//顶点v的（相对于顶点j的）下一邻接顶点
	virtual VStatus& status ( int i) { return Adj[i][0]->pV->status; } //顶点v的状态
	virtual int& dTime ( int i) { return Adj[i][0]->pV->dTime; } //顶点v的时间标签dTime
	virtual int& fTime ( int i) { return Adj[i][0]->pV->fTime; } //顶点v的时间标签fTime
	virtual int& parent ( int i) { return Adj[i][0]->pV->parent; }  //顶点v在遍历树中的父亲
	virtual int& priority ( int i) { return Adj[i][0]->pV->priority; } //顶点v在遍历树中的优先级数
	// 边
	virtual bool exists(int i, int j) { 
	  auto tmp = Adj[j][0]->pV;
	  for (int x = 0; x < Adj[i].size(); x++)
	  {
		if (Adj[i][x]->pV == tmp)
		  return true;
	  }
	  return false;
	} //边(v, u)是否存在
	virtual void insert(Te const& e, int v, int u, int w)
	{
	  Adj[v].insert(new EAndV<Tv, Te>(Adj[u][0], new Edge<Te>(e, w)));
	}//在顶点v和u之间插入权重为w的边e
	virtual Te remove(int i, int j)
	{
	  for (int x = 0; x < Adj[i].size(); x++)
	  {
		if (Adj[i][x]->pV == Adj[j][0]->pV)
		{
		  auto ret = Adj[i][x]->pE->data;
		  Adj.remove(Adj.search(Adj[i][x]));
		  return ret;
		}
	  }
	}//删除顶点v和u之间的边e，返回该边信息
	virtual EType & type(int i, int j)
	{
	  for (int x = 0; x < Adj[i].size(); x++)
	  {
		if (Adj[i][x]->pV == Adj[j][0]->pV)
		{
		  return Adj[i][x]->pE->status;
		}
	  }
	}//边(v, u)的类型
	virtual Te& edge(int i, int j)
	{
	  for (int x = 0; x < Adj[i].size(); x++)
	  {
		if (Adj[i][x]->pV == Adj[j][0]->pV)
		{
		  return Adj[i][x]->pE->data;
		}
	  }
	}//边(v, u)的数据（该边的确存在）
	virtual int& weight(int i, int j)
	{
	  for (int x = 0; x < Adj[i].size(); x++)
	  {
		if (Adj[i][x]->pV == Adj[j][0]->pV)
		{
		  return Adj[i][x]->pE->weight;
		}
	  }
	}//边(v, u)的权重
};
#endif