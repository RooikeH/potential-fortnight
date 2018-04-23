//---------------------------------客户端client.c---------------------------------------//  
#include <stdio.h>  
#include <netinet/in.h> //定义数据结构sockaddr_in  
#include <sys/socket.h> //定义socket函数以及数据结构  
#include <sys/types.h>  
#include <string.h>  
#include <stdlib.h>  
#include <netdb.h>  
#include <unistd.h>  
#include <signal.h>  
#include <time.h>  
   
int main(int argc, char *argv[])  
{  
    struct sockaddr_in clientaddr; //定义地址结构  
    pid_t pid;  
    int clientfd, sendbytes, recvbytes;
    struct hostent *host; //主机信息数据结构  
    char *buf, *buf_read, *usrname, *passwd, *key;  
    if (argc < 5)  
    {  
        printf("wrong usage");  
        printf("%s host port name\n", argv[0]);  
        exit(1);  
    }  
    host = gethostbyname(argv[1]);  
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
    {  
        perror("fail to create socket");  
        exit(1);  
    }  
    bzero(&clientaddr, sizeof(clientaddr));  
    clientaddr.sin_family = AF_INET;  
    clientaddr.sin_port = htons((uint16_t)atoi(argv[2]));  
    clientaddr.sin_addr = *((struct in_addr *)host->h_addr);  
    //客户端连接服务端  
    if (connect(clientfd, (struct sockaddr *)&clientaddr, sizeof(struct sockaddr)) == -1)  
    {  
        perror("fail to connect");  
        exit(1);  
    }  
    printf("connect success!\n");

    //this part is making sure the username and passwd
    usrname = (char *)malloc(10);
    passwd = (char *)malloc(10);
    memset(usrname, 0, 10);
    memset(passwd, 0, 10);
    strcpy(usrname, argv[3]);
    strcpy(passwd, argv[4]);
    strcat(usrname,passwd);
    if ((sendbytes = send(clientfd, usrname, strlen(usrname), 0)) == -1)  
    {  
        perror("fail to send");  
        exit(1);  
    }  
    printf("send USERNAME and PASSWD!\n");
    key = (char *)malloc(3);
    memset(key, 0, 3);
    if (recv(clientfd, key, 3, 0) == -1)  
    {  
        perror("fail to recv");  
        exit(1);  
    } 
    if (strcmp(key,"yes") != 0){
        printf("no user! please input again!\n");
        sleep(1000);
        exit(1);
    } 
    printf("user exist!\n");

    //welcome to chat room
    buf = (char *)malloc(120);  
    memset(buf, 0, 120);  
    buf_read = (char *)malloc(100);  
   
    if (recv(clientfd, buf, 100, 0) == -1)  
    {  
        perror("fail to recv");  
        exit(1);  
    }  
    printf("\n%s\n", buf);  
    pid = fork();  
    while (1)  
    {  
        if (pid > 0)  
        {  
            //父进程发送消息  
            strcpy(buf, argv[3]);  
            strcat(buf, ": ");  
            memset(buf_read, 0, 100);  
            fgets(buf_read, 100, stdin);//stdin: standard input 
            strncat(buf, buf_read, strlen(buf_read) - 1);  
            if ((sendbytes = send(clientfd, buf, strlen(buf), 0)) == -1)  
            {  
                perror("fail to send");  
                exit(1);  
            }  
        }  
        else if (pid == 0)  
        {  
            //子进程接受消息  
            memset(buf, 0, 100);  
            if (recv(clientfd, buf, 100, 0) <= 0)  
            {  
                perror("fail to recv");  
                close(clientfd);  
                raise(SIGSTOP);  
                exit(1);  
            }  
            printf("%s\n", buf);  
        }  
        else  
            perror("fork error");  
    }  
    close(clientfd);  
    return 0;  
}  