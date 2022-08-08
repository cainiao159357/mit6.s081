#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
int get_line_length(char *buf){
    int line_length=0;
    while(buf[line_length]!='\n'){
        line_length++;
    }
    return line_length;
}


int main(int argc,char *argv[]){
    char buf[100]={0};
    int cur=0;
    int line_num=0;
    int status;
    if(argc<2){
        fprintf(2,"argv fail\n");
        exit(1);
    }
    int size=read(0,buf,100);
    char parm_buf[size+20];
    memset(parm_buf,0,sizeof(parm_buf));
    cur++;
    for(int i=0;i<size; i++,cur++){
        if(buf[i]==' '){
            continue;
        }
        if(buf[i]=='\n'){
            cur++;
            parm_buf[cur]='\n';
            line_num++;
            cur++;
            continue;
        }
        parm_buf[cur]=buf[i];
    }
    if(buf[size-1]!='\n'){
        line_num++;
    }
    cur=0;
    for(int i=0;i<line_num;i++){
        int add_parm_num=0;
        int temp_cur=cur;
        for(;parm_buf[temp_cur]!='\n';temp_cur++){
            if(parm_buf[temp_cur]==0&&parm_buf[temp_cur+1]!=0&&parm_buf[temp_cur+1]!='\n'){
                add_parm_num++;
            }
        }
        char *parm[add_parm_num+argc];
        memset(parm,0,sizeof(parm));
        int parm_cur=0;
        for(int j=1;j<argc;j++,parm_cur++){
            parm[parm_cur]=argv[j];
        }
        for(;parm_buf[cur]!='\n';cur++){
            if(parm_buf[cur]==0&&parm_buf[cur+1]!=0&&parm_buf[cur+1]!='\n'){
                parm[parm_cur]=&parm_buf[cur+1];
                parm_cur++;
            }
        }
        cur++;
        int pid=fork();
        if(pid==0){
            exec(parm[0],parm);
            fprintf(2,"exec fail\n");
            exit(1);
        }else{
            wait(&status);
            if(status==1){
                fprintf(2," wait fail\n");
                exit(1);
            }
        }
    }
    exit(0);
}