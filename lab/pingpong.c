#include "kernel/types.h"
#include "user/user.h"


int main(int argc){
    int p[2];
    pipe(p);
    char buff[6];
    int pid = fork();
    if (pid == 0){
        // 子进程从管道读取ping，如果没有就先阻塞
        read(p[0], buff, 6);
        // 输出到shell
        fprintf(1, "%d: received %s\n", pid, buff);
        //写入pong
        write(p[1], "pong\n", 6);
        // 关闭管道然后退出
        close(p[0]); 
        close(p[1]);
        exit(0);
    }else{
        // 父进程先写入ping
        write(p[1], "ping\n", 6);
        // 从管道读取pong，在子进程写入pong之前一直阻塞
        read(p[0], buff, 6);
        // 输出到shell
        fprintf(1, "%d: received %s\n", pid, buff);
        // 关闭管道然后退出
        close(p[1]);
        close(p[0]);
        exit(0);
    }
}