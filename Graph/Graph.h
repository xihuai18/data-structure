#ifndef __GRAPH__
#define __GRAPH__

#include <climits>
#include "../Queue/Queue.h"
#include "../Stack/Stack.h"

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //边在遍历树中所属的类型

template <typename Tv, typename Te> //顶点类型、边类型
class Graph { //图Graph模板类
private:
   void reset() { //所有顶点、边的辅助信息复位
      for ( int i = 0; i < n; i++ ) { //所有顶点的
         status ( i ) = UNDISCOVERED; dTime ( i ) = fTime ( i ) = -1; //状态，时间标签
         parent ( i ) = -1; priority ( i ) = INT_MAX; //（在遍历树中的）父节点，优先级数
         for ( int j = 0; j < n; j++ ) //所有边的
            if ( exists ( i, j ) ) type ( i, j ) = UNDETERMINED; //类型
      }
   }
   void BFS ( int , int& ); //（连通域）广度优先搜索算法
   void DFS ( int, int& ); //（连通域）深度优先搜索算法
   void BCC ( int, int&, Stack<int>& ); //（连通域）基于DFS的双连通分量分解算法
   bool TSort ( int, int&, Stack<Tv>* ); //（连通域）基于DFS的拓扑排序算法
   template <typename PU> void PFS ( int, PU ); //（连通域）优先级搜索框架
public:
// 顶点
   int n; //顶点总数
   virtual int insert ( Tv const& ) = 0; //插入顶点，返回编号
   virtual Tv remove ( int ) = 0; //删除顶点及其关联边，返回该顶点信息
   virtual Tv& vertex ( int ) = 0; //顶点v的数据（该顶点的确存在）
   virtual int inDegree ( int ) = 0; //顶点v的入度（该顶点的确存在）
   virtual int outDegree ( int ) = 0; //顶点v的出度（该顶点的确存在）
   virtual int firstNbr ( int ) = 0; //顶点v的首个邻接顶点
   virtual int nextNbr ( int, int ) = 0; //顶点v的（相对于顶点j的）下一邻接顶点
   virtual VStatus& status ( int ) = 0; //顶点v的状态
   virtual int& dTime ( int ) = 0; //顶点v的时间标签dTime
   virtual int& fTime ( int ) = 0; //顶点v的时间标签fTime
   virtual int& parent ( int ) = 0; //顶点v在遍历树中的父亲
   virtual int& priority ( int ) = 0; //顶点v在遍历树中的优先级数
// 边：这里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例
   int e; //边总数
   virtual bool exists ( int, int ) = 0; //边(v, u)是否存在
   virtual void insert ( Te const&, int, int, int ) = 0; //在顶点v和u之间插入权重为w的边e
   virtual Te remove ( int, int ) = 0; //删除顶点v和u之间的边e，返回该边信息
   virtual EType & type ( int, int ) = 0; //边(v, u)的类型
   virtual Te& edge ( int, int ) = 0; //边(v, u)的数据（该边的确存在）
   virtual int& weight ( int, int ) = 0; //边(v, u)的权重
// 算法
   void bfs ( int ); //广度优先搜索算法
   void dfs ( int ); //深度优先搜索算法
   void bcc ( int ); //基于DFS的双连通分量分解算法
   Stack<Tv>* tSort ( int ); //基于DFS的拓扑排序算法
   void prim ( int ); //最小支撑树Prim算法
   void dijkstra ( int ); //最短路径Dijkstra算法
   template <typename PU> void pfs ( int, PU ); //优先级搜索框架
};

template<typename Tv, typename Te>
inline void Graph<Tv, Te>::BFS(int v, int & clock)
{
  Queue<int> Q;
  status(v) = DISCOVERED;
  Q.enqueue(v);
  while (!Q.empty())
  {
	int v = Q.dequeue();
	dTime(v) = ++clock;
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
	{
	  if (UNDISCOVERED == status(u)) {
		status(u) = DISCOVERED; Q.enqueue(u);
		status(v, u) = TREE; parent(u) = v;
	  }
	  else {
		status(v, u) = CROSS;
 	  }
	}
	status(v) = VISITED;
  }
}

template<typename Tv, typename Te>
inline void Graph<Tv, Te>::DFS(int v, int & clock)
{
  dTime(v) = ++clock;
  status(v) = DISCOVERED;
  for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
	switch (status(u))
	{
	case UNDISCOVERED:
	  status(v, u) = TREE; parent(u) = v; DFS(u, clock); break;
	case DISCOVERED:
	  status(v, u) = BACKWARD; break;
	default:
	  status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
	}//dTime(v) < dTime(u)意味着v有多于一条路径到u；
  status(v) = VISITED;
  fTime(v) = ++clock;
}

/*
概念：
1. 割点：除去此点及其相关联的边，形成多个连通域；
2. 双连通域：无割点（最少含顶点数为2）
*/
#define hca(x) (fTime(x))
template<typename Tv, typename Te>
inline void Graph<Tv, Te>::BCC(int v, int & clock, Stack<int>& S)
{
  hca(v) = dTime(v) = ++clock; status(v) = DISCOVERED; S.push(v);
  for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
	switch (status(u))
	{
	case UNDISCOVERED:
	  parent(u) = v; status(v, u) = TREE; BCC(u, clock, S);
	  if (hca(u) < dTime(v))
		hca(v) = min(hca(u), hca(v));
	  else {
		while (v != S.pop());//此处弹出的即为一个双连通子图；
		S.push(v);
	  }
	  break;
	case DISCOVERED:
	  status(v, u) = BACKWARD;
	  if (u != parent(v)) hca(v) = min(hca(v), dTime(u));
	  break;
	default://VISITED
	  status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
	  break;
	}
  status(v) = VISITED;
}
#undef hca

template<typename Tv, typename Te>
inline bool Graph<Tv, Te>::TSort(int v, int & clock, Stack<Tv>* S)
{
  dTime(v) = ++clock;
  status(v) = DISCOVERED;
  for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
	switch (status(u))
	{
	case UNDISCOVERED:
	  status(v, u) = TREE; parent(u) = v; if (!TSort(u, clock)) return false; break;
	case DISCOVERED:
	  status(v, u) = BACKWARD; return false; break;
	default:
	  status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
	}//dTime(v) < dTime(u)意味着v有多于一条路径到u；
  status(v) = VISITED;
  S->push(vertex(v));
  fTime(v) = ++clock;
  return true;
}

template<typename Tv, typename Te>
inline void Graph<Tv, Te>::bfs(int s)
{
  reset();
  int clock = 0;
  int v = s;
  do {
	if (UNDISCOVERED = status(v))
	  BFS(v, clock);
  } while (s != (v = (++v%n)));
}

template<typename Tv, typename Te>
inline void Graph<Tv, Te>::dfs(int s)
{
  reset(); int clock = 0; int v = s; 
  do 
	if (UNDISCOVERED == status(v)) 
	DFS(v, clock); 
  while (s != (v = (++v % n)));
}

template<typename Tv, typename Te>
inline void Graph<Tv, Te>::bcc(int s)
{
  reset(); int clock = 0; int v = s;
  Stack<int> S;
  do
	if (UNDISCOVERED == status(v)) {
	  BCC(v, clock, S);
	  S.pop();
	}
  while (s != (v = (++v % n)));
}

template<typename Tv, typename Te>
inline Stack<Tv>* Graph<Tv, Te>::tSort(int s)
{
  reset(); int clock = 0; int v = s;
  Stack<Tv> * S = new Stack<Tv>;
  do {
	if (UNDISCOVERED == status(v))
	  if (!TSort(v, clock, S))
		while (!S->empty())
		{
		  S->pop();
		  break;
		}
  } while (s != (v = (++v % n)));
  return S;//S is empty if the graph is not DAG(Directed Acyclic Graph);
}

template <typename Tv, typename Te> struct PrimPU {
  virtual void operator()(Graph<Tv, Te>* g, int uk, int v) {
	if (g->status(v) == UNDISCOVERED)
	  if (g->priority(v) > g->weight(uk, v)) {
		g->priority(v) = g->weight(uk, v);
		g->parent(v) = uk;
	  }
  }
};

template<typename Tv, typename Te>
inline void Graph<Tv, Te>::prim(int s)
{
  pfs<Tv, Te>(s, PrimPU<Tv, Te>());
}

template <typename Tv, typename Te> struct DijkstraPU {
  virtual void operator ()(Graph<Tv, Te>* G, int p, int v)
  {
	if(G->status(v) == UNDISCOVERED)
	  if (G->priority(v) > G->priority(p) + G->weight(p, v))
	  {
		G->priority(v) = G->priority(p) + G->weight(p, v);
		G->parent(v) = p;
	  }
  }
};

template<typename Tv, typename Te>
inline void Graph<Tv, Te>::dijkstra(int s)
{
  pfs(s, DijkstraPU<Tv, Te>());
}

template <typename Tv, typename Te> template <typename PU> //优先级搜索（全图）
void Graph<Tv, Te>::pfs(int s, PU prioUpdater) {
  reset(); int v = s; 
  do 
	if (UNDISCOVERED == status(v)) 
	  PFS(v, prioUpdater); 
  while (s != (v = (++v % n))); 
}

template <typename Tv, typename Te> template <typename PU> 
void Graph<Tv, Te>::PFS(int s, PU prioUpdater) { 
  priority(s) = 0; status(s) = VISITED; parent(s) = -1; 
  while (1) { 
	for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) 
	  prioUpdater(this, s, w); 
	for (int shortest = INT_MAX, w = 0; w < n; w++)
	  if (UNDISCOVERED == status(w)) 
		if (shortest > priority(w)) 
		{
		  shortest = priority(w); s = w;
		} //优先级最高的顶点s
	if (VISITED == status(s)) break; //直至所有顶点均已加入
	status(s) = VISITED; status(parent(s), s) = TREE; 
  }
} //通过定义具体的优先级更新策略prioUpdater，即可实现不同的算法功能


#endif