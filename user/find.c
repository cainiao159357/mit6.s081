#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find(char *file_path,char *res){
    struct stat st;
    struct dirent de;
    char buf[100];
    memset(buf,0,sizeof(buf));
    char *p;
    memmove(buf,file_path,strlen(file_path));
    int fd=open(file_path,0);
    if(fd<0){
        fprintf(2,"open fail\n");
        exit(1);
    }
    while (read(fd,&de,sizeof(de))==sizeof(de)){
        if(de.inum==0){
            break;
        }
        if(!strcmp(de.name,".")||!strcmp(de.name,"..")){
            continue;
        }
        p=buf+strlen(buf);
        memmove(p, de.name, DIRSIZ);
        if(stat(buf,&st)<0){
            fprintf(2,"stat() fail filename :%s\n",buf);
            exit(1);
        }
        if(st.type==T_FILE){
            if(!strcmp(de.name,res)){
                fprintf(1,"%s\n",buf);
            }
        }else if (st.type==T_DIR)
        {
            p=buf+strlen(buf);
            *p='/';
            find(buf,res);
        }
         memset(buf,0,sizeof(buf));
         memmove(buf,file_path,100);
    }
    close(fd);
    return;
}
void main(int argc,char *argv[]){
    char buf[100];
    char *p;
    memset(buf,0,sizeof(buf));
    if(argc!=3){
        fprintf(2,"parameter fail\n");
        exit(1);
    }
    memmove(buf,argv[1],DIRSIZ);
    p=buf+strlen(buf)-1;
    if(*p!='/'){
        p++;
        *p='/';
    }
    find(buf,argv[2]);
    exit(0);
}
