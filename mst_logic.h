#ifndef MST_LOGIC_H
#define MST_LOGIC_H
#include<vector>
using namespace std;

struct edges{
  int u,v;
  int wt;
};
int primsMST(int n,edges edge[],int c,edges mst[]);
int generateGraph(int n,edges edge[]);
void generateBombs(int n,int bombs[]);
#endif
