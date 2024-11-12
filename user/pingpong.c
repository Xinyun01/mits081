#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
 /*$ make qemu
    ...
    init: starting sh
    $ pingpong
    4: received ping
    3: received pong
*/
int main(int argc, char const *argv[]){

    if (argc != 1) { 
        fprintf(2, "error\n");
        exit(1);
    }
    int p_child[2];//创建管道
    int p_parent[2];//创建管道
    char buf[20];
    pipe(p_child);//建立管道
    pipe(p_parent);
    int pid = fork();//创建子进程

    if(pid == 0){
        close(p_parent[1]);//关闭写
        read(p_parent[0],buf,sizeof(buf));
        //打印
        printf("%d ：received %s",getpid(),buf);
        close(p_child[0]);
        write(p_child[1],"pong\n",5);
        close(p_child[1]);
        exit(0);
    }
    else{
       close(p_parent[0]);//关闭读
       write(p_parent[1],"ping\n",5);
       close(p_parent[1]);
       wait(0);
       read(p_child[0],buf,sizeof(buf));
       printf("%d ：received %s",getpid(),buf);
       close(p_child[0]);//关闭读
       
    }
    exit(0);
}