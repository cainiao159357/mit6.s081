#include "kernel/types.h"
#include "user/user.h"
#define rd 0
#define wt 1
int main(int argc,char *argv[]){
    int p_to_child[2];
    int p_to_parent[2];
    char buf[10];
    pipe(p_to_child);
    pipe(p_to_parent);
    int pid=fork();
    if(pid<0){
        fprintf(2,"fork fail\n");
        exit(1);
    }
    if(pid==0){
        close(p_to_child[wt]);
        close(p_to_parent[rd]);
        int ret=read(p_to_child[rd],buf,1);
        if(ret<=0){
            fprintf(2,"read fail\n");
            exit(1);
        }
        fprintf(1,"%d: received ping\n",getpid());
        ret=write(p_to_parent[wt],"1",1);
        if(ret<=0){
            fprintf(2,"write fail\n");
            exit(1);
        }
        close(p_to_child[rd]);
        close(p_to_parent[wt]);
        exit(0);
    }else{
        close(p_to_child[rd]);
        close(p_to_parent[wt]);
        int ret=write(p_to_child[wt],"1",1);
        if(ret<=0){
            fprintf(2,"write fail\n");
            exit(1);
        }
        ret=read(p_to_parent[rd],buf,1);
        if(ret<=0){
            fprintf(2,"read fail\n");
            exit(1);
        }
        fprintf(1,"%d: received pong\n",getpid());
        close(p_to_child[wt]);
        close(p_to_parent[rd]);
        exit(0);
    }
}