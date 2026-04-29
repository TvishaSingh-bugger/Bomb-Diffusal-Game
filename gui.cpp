#include<cstdlib>
#include<climits>
#include<ctime>

extern "C"{
struct edges{
    int u,v,w;
};

int getNumLocation(){
    srand(time(0));
    int n=0;
    n=rand()%5+8;
    return n;
}

void generateBomb(int n,int bombs[]){
    for(int i=0;i<n;i++){
        if(rand()%100<=80){
            bombs[i]=1;
        }else{
            bombs[i]=0;
        }
    }
}

int edgesCreate(int n,edges sides[]){
    int idx=0;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            sides[idx].u=i;
            sides[idx].v=j;
            int d,o,eq;
            d=rand()%10+5;
            if(rand()%4!=0){
                o=rand()%10+5;
            }else{
                o=0;
            }
            int c=rand()%5;
            if(c<2){
                eq=rand()%4+2;
            }else{
                eq=0;
            }
            sides[idx].w=d+o-eq;
            if(sides[idx].w<1){
                sides[idx].w=1;
            }
            idx++;
        }
    }
    return idx;
}

int primMST(int n,edges edge[],int c,edges mst[]){
    bool visited[50]={false};
    visited[0]=true;
    int count=0;
    for(int i=0;i<n-1;i++){
        int best=-1;
        int bestW=INT_MAX;
        for(int j=0;j<c;j++){
            int u=edge[j].u;
            int v=edge[j].v;
            int w=edge[j].w;
            if(visited[u]==true && visited[v]==false){
                if(bestW>w){
                bestW=w;
                best=j;
                }
            }else if(visited[v]==true && visited[u]==false){
                if(bestW>w){
                    bestW=w;
                    best=j;
                }
            }
        }
        if(best!=-1){
            mst[count++]=edge[best];
            int p=edge[best].u;
            int q=edge[best].v;
            visited[p]=true;
            visited[q]=true;
        }
    }
    return count;
}
}