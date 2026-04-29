#include<iostream>
#include<vector>
#include<stdlib.h>
#include<ctime>
using namespace std;

struct edge{
  int u,v;
  int d,o,eq;
};

vector<edge>generateGraph(int n);
void printGraph(vector<edge>&edges);
vector<int>generateBombs(int n);
void robotMission(vector<edge>&edges,vector<int>&bombs);
vector<edge>primMST(int n,vector<edge>&edges);

int main(){
  srand(time(0));
  int n=rand()%4+5;
  cout<<"Locations:"<<n<<endl;
  vector<edge>edges=generateGraph(n);
  printGraph(edges);
  vector<int>bombs=generateBombs(n);
  cout<<"Press Start:";
  cin.get();

  vector<edge>mstEdges=primMST(n,edges);
  cout<<"\nMinimum Spanning Tree Traversal:\n";
  printGraph(mstEdges);
  robotMission(mstEdges,bombs);
}