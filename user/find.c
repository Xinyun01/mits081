#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


//格式命名
char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}

//使用递归
int recursion(char *path){
  char* buf = fmtname(path);
  if( strcmp(fmtname(buf),"..") == 0 ||strcmp(fmtname(buf),".") == 0){
    return 0;
  }
  return 1;
}

//传入文件路径和查找文件名
void find(char *path,char *filename)
{
  //printf("path = %s  filename = %s  fmtname(path) = %s\n",path,filename,fmtname(path));
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if( strcmp(fmtname(path),filename) == 0){
     // printf("path = %s filename = %s \n",path,filename);
  }
//#define T_DIR     1   // Directory
//#define T_FILE    2   // File
//#define T_DEVICE  3   // Device
  switch(st.type){
  case T_FILE:    
    break;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      //printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }

    
       if( strcmp(fmtname(buf),filename) == 0){
      // printf("path = %s buf = %s (fmtname(buf)= %s\n",path, buf,fmtname(buf));
         printf("%s\n",buf);
       }
     //使用递归  在该文件目录之下是否还有其他的文件进行递归 buf 为文件路径  不递归"." ".."
     if(recursion(buf)) find(buf,filename);

    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  
  if(argc == 1){
    printf("error");
    exit(1);
  }
  else if(argc == 2){
    
    find(".",argv[1]); //eg find sh
    exit(0);
  }
  else if(argc == 3){
    find( argv[1] , argv[2] );//eg find . sh
  }
   exit(0);

}
