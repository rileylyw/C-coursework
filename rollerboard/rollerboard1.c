#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char data[6][6];
int n,m;
typedef struct Node
{
    char board[6][6];

    struct Node *next;
}Node;
 
typedef struct
{
    Node *f,*r;
}LQ;
 

void EnQ(LQ *q,char board[][6]){
	int i;
	Node * p=(Node *)malloc(sizeof(Node));
    for(i=0;i<n;i++){
        strcpy(p->board[i],board[i]);
    }
    
    
    p->next=NULL;
    if(q->r==NULL){
        q->f=q->r=p;
    }else{
        q->r->next=p;
        q->r=p;
    }
	
}
void front(LQ *q,char board[][6]){
    int i;
    Node * p=q->f;
    for(i=0;i<n;i++){
        strcpy(board[i],p->board[i]);
    }
}
int DeQ(LQ *q){
	if(q->f==NULL)//判断队列是否为空
        return 0;
	Node * n=q->f;
    if(q->f==q->r){
    	

        q->f=q->r=NULL;
    }else{
        q->f=q->f->next;
       
        
    }
    free(n);
    return 1;

}//成功出队返回1，不成功返回0。出队元素通过实参返回。
int empty(LQ *q){
    if(q->f==NULL)//判断队列是否为空
        return 1;
    return 0;
}
void visit(LQ*q,int k){
    int i,j=0;
    Node * p=q->f;
    if(p==NULL){
        return ;//队列为空
    }
    while(p!=NULL){
        j++;
        if(j==k){
            for(i=0;i<n;i++){
                printf("%s\n",p->board[i]);
            }
            break;
        }
       
        
        p=p->next;
    }
    printf("\n");
}
int count(LQ *q,char data[][6]){
    int i;
     Node * p=q->f;
    if(p==NULL){
        return 0;//队列为空
    }
    int id=1;
    while(p!=NULL){
        int flag=0;
        
        for(i=0;i<n;i++){
            if(strcmp(data[i],p->board[i])==0){
                flag++;
            }
            //printf("%s\n",p->board[i]);
        }
        if(flag==n){
            return id;
        }
        id++;
        p=p->next;
    }
    return 0;
}
void clear(LQ *q){
    while(DeQ(q));
    free(q);
}
LQ* init(){
    LQ * q=(LQ*)malloc(sizeof(LQ));
    q->f=q->r=NULL;
    return q;
}


void read(char file[]){
    FILE *fp;
    int i;
    fp=fopen(file,"r");
    if(fscanf(fp,"%d%d",&n,&m)<0){
        return;
    }
    for(i=0;i<n;i++){
        if(fscanf(fp,"%s",data[i])<0){
            return;
        }
    }
}

//vis=malloc(sizeof(int)*)
int size=1;
int top=0;
int father[102400];
int num=0;
void path(LQ *q,int a){
    
    if(a==0){
        return;
    }
    path(q,father[a]);
    printf("%d:\n",num++);
    visit(q,a);
   
}
void bfs(){
    LQ * q= init();
    LQ * vis=init();
    EnQ(vis,data);
    EnQ(q,data);
    father[1]=0;
    size++;
    while(!empty(q)){
        int i,j;
        front(q,data);
        //printf("***\n");
        int sum=0;
        for(i=0;i<m;i++){
            if(data[0][i]=='1'){
                sum++;
            }
        }
        if(top<sum){
            top=sum;
        }
         int ba=count(vis,data);
        if(sum==m){
            // for(int i=0;i<n;i++){
            //     printf("%s\n",data[i]);
            // }

            path(vis,ba);
            break;
        }
        char tp[6][6];
       
        DeQ(q);
        
         for(i=0;i<m;i++){
             for(j=0;j<n;j++) strcpy(tp[j],data[j]);
    
            //strcpy(tp,data);
            char p=tp[0][i];
            for(j=1;j<n;j++){
                tp[j-1][i]=tp[j][i];
            }
            tp[n-1][i]=p;
            int t=0;
            for(j=0;j<m;j++){
                if(tp[0][j]=='1'){
                    t++;
                }
            }
            if(t>=top){
                if(count(vis,tp)==0){
                    EnQ(vis,tp);
                    father[size]=ba;
                    // for(j=0;j<n;j++){
                    //     printf("%s\n",tp[j]);
                    // }
                    // printf("------------------\n");
                    EnQ(q,tp);
                    size++;
                }
            }
        }
        for(i=0;i<n;i++){
            for(j=0;j<n;j++) strcpy(tp[j],data[j]);
          
            char p=tp[i][0];
            for(j=1;j<m;j++){
                tp[i][j-1]=tp[i][j];
            }
            tp[i][m-1]=p;
            
            int t=0;
            for(j=0;j<m;j++){
                if(tp[0][j]=='1'){
                    t++;
                }
            }
            if(t>=top){
                if(count(vis,tp)==0){
                    EnQ(vis,tp);
                    father[size]=ba;
                    // for(j=0;j<n;j++){
                    //     printf("%s\n",tp[j]);
                    // }
                    // printf("------------------\n");
                    EnQ(q,tp);
                    size++;
                }
            }
            
        }

    }
    clear(q);
    clear(vis);
}

int main(int argc ,char *argv[]){
    char file[52];
    if(argc==3){
        strcpy(file,argv[2]);
    }else if(argc==2){
        strcpy(file,argv[1]);
    }

    read(file);
    bfs();
}