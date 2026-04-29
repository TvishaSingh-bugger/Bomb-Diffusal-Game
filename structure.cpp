#include<iostream>
#include<vector>
#include<stdlib.h>
#include<ctime>
#include<windows.h>
using namespace std;
struct edge{
  int u,v;
  int d,o,eq;
};


vector<edge>generateGraph(int n){
  vector<edge>edges;
  for(int i=0;i<n;i++){
    for(int j=i+1;j<n;j++){
      edge e;
      e.u=i;
      e.v=j;
      e.d=rand()%10+5;
      if(rand()%3!=0){
        e.o=rand()%10+5;
      }else e.o=0;
      if(rand()%5<2){
        e.eq=rand()%4+2;
      }else e.eq=0;
    edges.push_back(e);
  }
}
return edges;
}


void printGraph(vector<edge>&edges){
  for(edge e:edges){
    cout<<e.u<<" "<<e.v<<" Distance:"<<e.d<<" Obstacle:"<<e.o<<" Equipment:"<<e.eq<<endl;
  }
}


vector<int>generateBombs(int n){
  vector<int>bombs(n);
  for(int i=0;i<n;i++){
    bombs[i]=(rand()%100<60);
  }
  cout<<"Bomb Loactions:";
  for(int i=0;i<n;i++){
    if(bombs[i]==1){
      cout<<i<<" ";
    }
  }
  cout<<"\n";
  return bombs;
}

void robotMission(vector<edge>& edges,vector<int>& bombs) {
  int n=0;
  for(int i=0;i<bombs.size();i++){
    if(bombs[i]==1){
      n++;
    }
  }
  int totalDistance=0;
  int bombsDiffused=0;
  vector<bool>bombVisited(bombs.size(),false);
  cout<<"Misson Start:\n"<<endl;

  for(edge e:edges){
    int finalDistance=e.d;
    if(e.o>0){
       finalDistance+=e.o;
    }
    if(e.eq>0){
      finalDistance-=e.eq;
    }
    if(finalDistance<1){
      finalDistance=1;
    }

    cout<<"Robot moved from:"<<e.u<<" to: "<<e.v<<" Distance:"<<finalDistance<<endl;
    Sleep(500);
    totalDistance+=finalDistance;

    if(bombs[e.u]==1 &&bombVisited[e.u]==false){
      cout<<"Bomb diffused at:"<<e.u<<endl;
      bombVisited[e.u]=true;
      bombsDiffused++;
    }
    if(bombs[e.v]==1 &&bombVisited[e.v]==false){
      cout<<"Bomb diffued at:"<<e.v<<endl;
      bombVisited[e.v]=true;
      bombsDiffused++;
    }
}

cout<<"Total Distance Covered:"<<totalDistance<<endl;
cout<<"Total Time Taken:"<<totalDistance*2<<endl;
cout<<"Total Bombs Diffused:"<<bombsDiffused<<endl;
if(bombsDiffused==n && totalDistance*2<=90){
  cout<<"Mission successful!";
}
else{
  cout<<"Mission not completed,you lose!";
}
}