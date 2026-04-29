from tkinter import *
import ctypes
import random
import os
import sys

root=Tk()
root.title("Bomb Diffusal Game!")
w=root.winfo_screenwidth()
h=root.winfo_screenheight()
root.geometry(f"{w}x{h}")
path=os.path.abspath("game.dll")
game=ctypes.CDLL(path)
game.getNumLocation.restype=ctypes.c_int
game.edgesCreate.restype=ctypes.c_int
game.primMST.restype=ctypes.c_int
game.generateBomb.restype=None

leftB=Frame(root,width=1000,height=h,bg="Black")
leftB.pack(side="left",fill="y")
leftB.pack_propagate(False)

rightB=Frame(root,width=600,height=h,bg="Blue")
rightB.pack(side="right",fill="y")
rightB.pack_propagate(False)

canvas=Canvas(leftB,bg="Black")
canvas.pack(fill="both",expand=True)

legend=Frame(leftB,bg="black")
legend.place(x=10, y=10)
Label(legend,text="MST Normal",bg="black",fg="cyan").pack(anchor="w")
Label(legend,text="Obstacle Path",bg="black",fg="orange").pack(anchor="w")
Label(legend,text="Equipment Path",bg="black",fg="yellow").pack(anchor="w")

def initGame():
    global n,bombs,edge,mst,arr2,mstSet,mstTotal,p,nodes,totalbombs
    global robot,curr,totalD,penalty,diffused,visited,gameOver

    canvas.delete("all")

    n=game.getNumLocation()
    bombs=(ctypes.c_int*n)()
    game.generateBomb(n,bombs)

    class edges(ctypes.Structure):
        _fields_=[("u",ctypes.c_int),("v",ctypes.c_int),("w", ctypes.c_int)]

    edge=(edges*(n*(n-1)//2))()
    game.edgesCreate(n,edge)

    mst=(edges*(n-1))()
    l=len(edge)
    s=game.primMST(n,edge,l,mst)

    arr2=[(mst[i].u,mst[i].v) for i in range(s)]
    mstSet=set()
    for u,v in arr2:
        mstSet.add((u,v))
        mstSet.add((v,u))

    mstTotal=0
    for u,v in arr2:
        for e in edge:
            if(e.u==u and e.v==v) or (e.u==v and e.v==u):
                mstTotal+=e.w
                break

    p=[]
    nodes=[]
    m=90

    def DisB(x,y):
        for px,py in p:
            if((px-x)**2+(py-y)**2)**0.5<80:
                return False
        return True

    for i in range(n):
        while True:
            x=random.randint(m,1000-m)
            y=random.randint(m,h-m)
            if DisB(x,y): break
        p.append((x,y))
        color="red" if bombs[i]==1 else "green"
        nodes.append(canvas.create_oval(x-15,y-15,x+15,y+15,fill=color))

    for e in edge:
        x1,y1=p[e.u]
        x2,y2=p[e.v]
        canvas.create_line(x1,y1,x2,y2,width=1,fill="blue")

    bombs[0]=0
    canvas.itemconfig(nodes[0],fill="grey")

    robot=canvas.create_oval(p[0][0]-15,p[0][1]-15,p[0][0]+15,p[0][1]+15,fill="yellow")

    curr=0
    totalD=0
    penalty=0
    diffused=0
    visited=[False]*n
    visited[0]=True
    gameOver=False

    totalbombs=sum(bombs)
    bombsDiff.config(text=f"Diffused Bombs:{diffused}")
    distance.config(text="Distance:0")
    Status.config(text="")
    totalBombLabel.config(text=f"Total bombs:{totalbombs}")
    targetLabel.config(text=f"Target weight:{mstTotal}")

def DrawPrims():
    for u,v in arr2:
        x1,y1=p[u]
        x2,y2=p[v]
        wt=wight(u,v)
        if wt>12:
            color="orange"
        elif wt<8:
            color="yellow"
        else:
            color="cyan"
        canvas.create_line(x1,y1,x2,y2,width=3,fill=color)

def wight(u,v):
    for e in edge:
        if (e.u==u and e.v==v) or (e.u==v and e.v==u):
            return e.w
    return 0

t=0
timer=False
Tlimit=45

def TimeFunction():
    global t,timer,gameOver
    if not timer or gameOver: return
    t+=1
    time.config(text=f"Time:{t}")
    if t>=Tlimit:
        timer=False
        gameOver=True
        Status.config(text="You lose",fg="red")
        blastAllBombs()
        return
    root.after(1000,TimeFunction)

def moveRobot(x1,y1,x2,y2,delay):
    steps=25
    dx=(x2-x1)/steps
    dy=(y2-y1)/steps
    for _ in range(steps):
        canvas.move(robot,dx,dy)
        root.update()
        root.after(delay)

def bombAnimation(x,y):
    for r in range(10,30,5):
        blast=canvas.create_oval(x-r,y-r,x+r,y+r,outline="yellow")
        root.update()
        root.after(30)
        canvas.delete(blast)

def blastAllBombs():
    for i in range(n):
        if bombs[i]==1:
            x,y=p[i]
            root.after(100,lambda x=x,y=y:bombAnimation(x,y))

def Movement(v):
    global curr,totalD,penalty,diffused,gameOver,timer
    if gameOver or v==curr: 
        return
    w=wight(curr,v)
    if w==0: 
        return

    x1,y1=p[curr]
    x2,y2=p[v]

    delay=70 if w>12 else 8 if w<8 else 15

    moveRobot(x1,y1,x2,y2,delay)
    curr=v

    if not visited[v]:
        totalD+=w
        visited[v]=True
        canvas.itemconfig(nodes[v],fill="grey")

    distance.config(text=f"Distance:{totalD}")

    if bombs[v]==1:
        bombAnimation(x2,y2)
        bombs[v]=0
        diffused+=1
        bombsDiff.config(text=f"Diffused Bombs:{diffused}")

    if diffused==totalbombs:
        timer=False
        gameOver=True
        if totalD<=mstTotal:
            Status.config(text="You win",fg="green")
        else:
            Status.config(text="You lose",fg="red")

def Click(event):
    if not timer or gameOver:
        return
    x,y=event.x,event.y
    for i in range(n):
        nx,ny=p[i]
        if nx-20<=x<=nx+20 and ny-20<=y<=ny+20:
            Movement(i)
            break

def mission():
    global timer,t
    if timer:
        return
    if gameOver:
        initGame()
        return;
    DrawPrims()
    t=0
    timer=True
    TimeFunction()

canvas.bind("<Button-1>",Click)

totalBombLabel=Label(rightB,text="",bg="white")
totalBombLabel.pack(pady=10)

targetLabel=Label(rightB,text="",bg="white")
targetLabel.pack(pady=10)

distance=Label(rightB,text="Distance:0",bg="white")
distance.pack(pady=10)

bombsDiff=Label(rightB,text="Diffused Bombs:0",bg="white")
bombsDiff.pack(pady=10)

Status=Label(rightB,text="",bg="white")
Status.pack(pady=10)

time=Label(rightB,text="Time:0",bg="white")
time.pack(pady=10)

Button(rightB,text="Start",command=mission).pack(pady=20)

initGame()

root.mainloop()