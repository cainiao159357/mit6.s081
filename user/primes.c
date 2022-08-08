#include "kernel/types.h"
#include "user/user.h"
#define rd 0
#define wt 1
int fun(int p_rd,int c_wt){
    int first_num=0;
    int temp=0;
    read(p_rd,&first_num,4);
    fprintf(1,"prime %d\n",first_num);
    int num;
    while(read(p_rd,&num,4)){
        if(num%first_num!=0){
            temp++;
            write(c_wt,&num,4);
        }
    }
    return temp;
}
int main(){
    int p[2];
    int c[2];
    int status;
    pipe(p);
    
    for(int i=2;i<=35;i++){
        write(p[wt],&i,4);
    }
    while (1)
    {
        int pid=fork();
        if(pid==0){
            pipe(c);
            close(p[wt]);
            int ret=fun(p[rd],c[wt]);
            close(p[rd]);
            if(ret==0){
                break;
            }
            p[rd]=c[rd];
            p[wt]=c[wt];
        }else{
            if(p[wt]){
                close(p[wt]);
            }
            if(p[rd]){
                close(p[rd]);
            }
            wait(&status);
            if(status==1){
                fprintf(2,"wait fail\n");
                exit(1);
            }
            exit(0);
        }
    }
    exit(0);
}