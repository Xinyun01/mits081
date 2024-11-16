#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" 
#define MAXLEN 100
//echo "file1 file2 file3"  ---> (标准输出) ---> | (管道) ---> xargs  ---> (标准输入) ---> read() ---> 处理并传递参数给 rm
int
main(int argc, char *argv[])
{
    sleep(10);
    //先获取管道里面的命令参数保存到buf中
    char buf[1024];
    read(0,buf,sizeof(buf)-1);
   // printf(" %s",buf);
    //argv[1] 为输出命令 agrv[2]之后为命令参数
    //使用fork 完成子进程调用
    //char m_argv[MAXARG][MAXLEN];  // 定义一个二维数组，最多存储 MAXARG 个字符串，每个字符串长度 MAXLEN
    char *m_argv[MAXARG];
    // 确保命令行参数数量不超过 MAXARG，并复制字符串
    int i;
    for (i = 1; i < argc && i < MAXARG; i++) {
         m_argv[i-1] = argv[i]; //将xargs 丢弃 
    }

    char *p = buf;   // 指向 buf 数组的当前读取位置
    char *start = buf;  // 用于标记每一行字符串的起始位置
     while (*p) {
        if (*p == '\n') {  // 如果遇到换行符
            *p = '\0';     // 将换行符替换为字符串结束符
            m_argv[argc-1] = start;  // 将这一行存入 m_argv 数组
            argc++;         // 增加字符串计数
            start = p + 1;  // 更新下一行的起始位置
        }
        p++;  // 继续遍历
    }
        // // 打印 m_argv 数组中的内容
    // for (i = 0; i < argc-1 && i < MAXARG; i++) {
    //     printf("m_argv[%d]: %s\n", i, m_argv[i]);
    // }
    int pid = fork();//创建子进程
    if(pid > 0){

        wait(0); //等待子进程结束
    }
    else if(pid == 0){
        exec(m_argv[0],m_argv);
        printf("child:failed");
        exit(1);
    }
    else{
        printf("fork:failed");
    }
  exit(0);
}