#include "kernel/types.h"
#include "user/user.h"
#define rd 0
#define wt 1
int main(){
    int p[2];
    char buf;
    pipe(p);
    write(p[wt],"a",1);
    close(p[wt]);
    int ret=read(p[rd],&buf,1);
    printf("ret:%d buf:%c\n",ret,buf);
    exit(0);
}