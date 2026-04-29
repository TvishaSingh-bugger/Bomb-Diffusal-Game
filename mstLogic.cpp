#include<iostream>
#include<vector>
#include<queue>
#include<map>
#include<climits>
using namespace std;

struct edge{
  int u,v;
  int d,o,eq;
};

int getWeight(edge e){
  int w=e.d;
  if(e.o>0){
    w+=e.o;
  }
  if(e.eq>0){
    w-=e.eq;
  }
  return w;
}

vector<edge>primMST(int n,vector<edge>&edges){
  vector<pair<int,int>>robotOrder;
  vector<vector<pair<int,int>>>adj(n);
  for(auto e: edges){
    int w=getWeight(e);
    adj[e.v].push_back({e.u,w});
    adj[e.u].push_back({e.v,w});
  }
  vector<bool>inMST(n,false);
  vector<int>key(n,INT_MAX);
  vector<int>parent(n,-1);
  priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>q;
  key[0]=0;
  q.push({0,0});
  while(!q.empty()){
    int u=q.top().second;
    q.pop();
    if(inMST[u])continue; 
    inMST[u]=true;
    if(parent[u]!=-1){
      robotOrder.push_back({parent[u],u});
    }
    for(auto it:adj[u]){
      int v=it.first;
      int w=it.second;
      if(!inMST[v]&&w<key[v]){ 
      key[v]=w;
      parent[v]=u;
      q.push({key[v],v});
    }
  }
}
vector<edge>mst;
for(int i=1;i<n;i++){
  if(parent[i]==-1){
    continue;
  }
  for(auto e:edges){
  if((e.u==parent[i]&&e.v==i)||(e.v==parent[i]&&e.u==i)){
    mst.push_back(e);
    break;
    }
  }
}
return mst;
}