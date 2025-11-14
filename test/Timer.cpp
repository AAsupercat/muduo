#include<iostream>
#include<algorithm>
#include <unistd.h>
#include<sys/timerfd.h>

//int timerfd_create(int clockid, int flags); 1.创造一个定时器。
    //clockid CLOCK_REALTIME:以系统时间为基准值（随系统时间改变而改变）;
    //        CLOCK_MONOTONIC:以系统启动时间作为一个基准值（定时器不会随着时间的改变而改变）;
    //flags 0-阻塞操作
    //返回文件描述符（定时器），Linux下一切皆文件，定时器操作也跟文件操作没什么区别；
    //定时器定时的原理：每隔一段时间（定时器超时时间），系统就会给这个定时器写入一个8字节的数据。

    //int timerfd_settime(int fd, int flags,const struct itimerspec *new_value,struct itimerspec *old_value);2.启动定时器
    //fd: timerfd_create返回值；
    //flags: 默认设置为0，使用相对时间
    //new：设置超时时间
    //old：用于接收当前定时器原有的超时时间设置
    //struct itimerspec： 
        //     struct timespec {
        //        time_t tv_sec;                /* Seconds */   秒
        //        long   tv_nsec;               /* Nanoseconds */ 纳秒
        //    };
        //    struct itimerspec { 
        //        struct timespec it_interval;  /* Interval for periodic timer */ 第一次超时以后，每隔多少秒进行超时
        //        struct timespec it_value;     /* Initial expiration */ 第一次超时时间
        //    };
int main()
{
    int timerfd=timerfd_create(CLOCK_MONOTONIC,0);
    if(timerfd<0)
    {
        perror("timer_create erro!");
        return -1;
    }
    struct itimerspec timer;
    timer.it_value.tv_sec=5;
    timer.it_value.tv_nsec=0;
    timer.it_interval.tv_sec=1;
    timer.it_interval.tv_nsec=0;
    timerfd_settime(timerfd,0,&timer,NULL); //设置定时器
    printf("第一次超时事件为%ld秒\n",timer.it_value.tv_sec);
    while(1)
    {
        uint64_t times;
        int ret=read(timerfd,&times,8);
        if(ret<0)
        {
            perror("读取失败");
            return -1;
        }
        printf("第一次超时后，每隔%ld秒后，进行超时提醒！\n",times);
    }
    close(timerfd);
    return 0;
}



