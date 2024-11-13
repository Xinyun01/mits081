#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  //从第二个参数开始遍历所有命令行参数 (因为 argv[0] 通常是程序名)
  for(i = 1; i < argc; i++){
    write(1, argv[i], strlen(argv[i]));
    if(i + 1 < argc){
      write(1, " ", 1);// 如果当前不是最后一个参数，打印空格，否则打印换行
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}
