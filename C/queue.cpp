#include"iostream.h"
 #include "stdlib.h"
 #include "stdio.h"
 #define MAX 30000     //宏定义
 #define TRUE 1
 #define FALSE 0
 #define R rand()

 float wait_length;                /*等待队列的总长度*/
 int   totalnum;                   /*总共顾客数*/
 float totaltime;                  /*顾客理发所需总时间*/                                          /*开张时间*/
 int   curtime;                    /*当前时间*/
 int   chairnum;                   /*当前可用的椅子数*/
 int   addtime;                    /*扫尾工作时间*/
 int   addpay=0;                   /*总营业额*/
 int   grade,one,two,three;        /*等级及收费标准*/
 int   sum1,sum2,sum3;             /*每等级选择人数*/

 struct customer
 {
     int NO;                       /*编号*/
     int intime;                   /*进入理发店时间*/
     int durtime;                  /*理发所需时间*/
     int intertime;                /*他的下一个人到来的时间间隔*/
     int starttime;                /*开始理发时间*/
     int leavetime;                /*离开理发店的时间*/
     int serve_flag;               /*是否在理发(1表示该顾客在理发，0表示处于等待队列中)*/
 };
 customer cus[MAX];

 typedef struct Qnode
 {
    int  num;                       /*理发者的编号*/
    struct Qnode *next;
 }Qnode,*QueuePtr;

class LinkQueue
 {
  public:
      LinkQueue():rear(NULL),front(NULL){}  //构造函数，建立空队列
	  ~LinkQueue(){}                       //析构函数
      void InitQueue(LinkQueue &Q);    /*等待队列初始化*/
      void outQueue(LinkQueue &Q);    /*输出等待队列中的元素*/
      int Queue_Length(LinkQueue &Q);  /*求等待队列的当前长度*/
      void EnQueue(LinkQueue &Q,int e);  /*将编号为e的顾客插入队尾*/
      int DeQueue(LinkQueue &Q);   /*队头元素出队，并用e返回其编号*/
      int QueueEmpty(LinkQueue &Q); /*判断等待队列是否为空，若空返回1*/
  protected:
      QueuePtr front;                 /*队头指针*/
      QueuePtr rear;                  /*队尾指针*/

 };
 LinkQueue W;                      /*等待队列*/

 void LinkQueue:: InitQueue(LinkQueue &Q)      /*等待队列初始化*/
 {
    Q.front=Q.rear=(QueuePtr)malloc(sizeof(Qnode));
    Q.front->next=NULL;
 };

 void LinkQueue::outQueue(LinkQueue &Q)       /*输出等待队列中的元素*/
 {
    QueuePtr p;
    p=Q.front;
    while(p->next)
    {
         p=p->next;
        cout<<p->num;
    }
    cout<<" ";
 };

 int LinkQueue::Queue_Length(LinkQueue &Q)     /*求等待队列的当前长度*/
 {
    int length=0;
    QueuePtr p;
    p=Q.front;
     while(p->next)
     {
         p=p->next;
         ++length;
     }
    return length;
 };

 void LinkQueue::EnQueue(LinkQueue &Q,int e)               /*将编号为e的顾客插入队尾*/
 {
    QueuePtr p;
    p=(QueuePtr)malloc(sizeof(Qnode));
    p->num=e;
    p->next=NULL;
    Q.rear->next=p;
    Q.rear=p;
 };

 int LinkQueue::DeQueue(LinkQueue &Q)                      /*队头元素出队，并用e返回其编号*/
 {
    QueuePtr p;
    int e;
    p=Q.front->next;
    e=p->num;
    Q.front->next=p->next;
    if(Q.rear==p)
        Q.rear=Q.front;
    free(p);
    return e;
 };

 int LinkQueue::QueueEmpty(LinkQueue &Q)                    /*判断等待队列是否为空，若空返回1*/
 {
    return(Q.front==Q.rear? TRUE:FALSE);
 };

 void customer_serve(int n)                      /*为顾客理发*/
 {
      cus[n].starttime=curtime;                  /*得出开始理发时间和离开时间*/
      cus[n].leavetime=cus[n].durtime+curtime;
	  switch(grade=1+R%3)                           /*选择等级*/
	  {
	  case 1 :
		  {one+=5*(1+R%10);sum1++;break;};                   /*消费者选择消费即不同级别理发师的创收以及每等级选择人数*/
      case 2 :
		  {two+=10*(5+R%6);sum2++;break;};
      case 3 :
		  {three+=100*(1+R%2);sum3++;break;};
	  }
      chairnum--;                                /*当前可用理发椅数减1*/
      cus[n].serve_flag=TRUE;
 }

 void customer_in()                              /*顾客进入理发店*/
 {
    LinkQueue  q;
    totalnum++;                                 /*顾客数加1*/
    cus[totalnum].NO=totalnum;
    cus[totalnum].intime=curtime;                /*记录顾客进入时间*/
    cus[totalnum].durtime=15+R%60;               /*产生随机数记录顾客理发所需时间*/
    cus[totalnum].intertime=2+R%30;              /*此顾客的下一个顾客来的时间间隔*/
    if(q.QueueEmpty(W) &&chairnum>0)
          customer_serve(totalnum);              /*有空闲位置并无人参与竞争，调用服务函数*/
    else
        {
          cus[totalnum].serve_flag=FALSE;        /*否则入队等待*/
          q.EnQueue(W,totalnum);
          wait_length+=q.Queue_Length(W);          /*累计队长*/
        }
 }

 void customer_leave(int n)                      /*顾客离开理发店*/
 {
   cus[n].serve_flag=FALSE;
   chairnum++;
   totaltime=curtime-cus[n].intime+totaltime;    /*得出该顾客在理发馆的时间，累加到总时间上*/
 }

 void list()                                     /*输出所有值*/
 {
   float aver_serve_time,aver_wait_len;          /*顾客平均等待时间，顾客平均等待长度*/
   aver_serve_time=totaltime/totalnum;
   aver_wait_len=wait_length/totalnum;
   cout<<"*  一天内的顾客总数为 : "<<totalnum<<endl;
   cout<<"*  顾客平均逗留时间为（分钟） : "<<aver_serve_time<<endl;
   cout<<"*  等候队列的平均长度 : "<<aver_wait_len<<endl;
   cout<<"*  关门后扫尾工作时间（分钟） : "<<addtime<<endl;
   cout<<"*  每天的营业额（元）     :"<<addpay<<endl;
   cout<<"*  每天不同级别理发师的创收(元):"<<endl;
   cout<<"*           1级(选择人数)       *:"<<one<<"     "<<sum1<<endl;
   cout<<"*           2级(选择人数)       *:"<<two<<"     "<<sum2<<endl;
   cout<<"*           3级(选择人数)       *:"<<three<<"    "<<sum3<<endl;
 }

 void main()
 {
   LinkQueue  q;
   int i,N,T,max;
   sum1=sum2=sum3=0;
   curtime=0,totaltime=0,totalnum=0,wait_length=0;
   cout<<"请输入理发椅数: ";
   cin>>N;
   chairnum=N;
   cout<<"请输入营业时间（单位：分钟）: ";
   cin>>T;
   q.InitQueue(W);
   customer_in();
   while(curtime++<T)                             /*当前时间属于营业时间，允许顾客进入*/
   {
      for(i=1;i<=totalnum;i++)
        {                                         /*判断有没有人离开*/
      if((cus[i].serve_flag==TRUE)&&(cus[i].leavetime==curtime))
            customer_leave(i);
        }
      while(chairnum>0 && !q.QueueEmpty(W))         /*让等待队列中的人去理发*/
        customer_serve(q.DeQueue(W));
      if((cus[totalnum].intime+cus[totalnum].intertime)==curtime)
         customer_in();                          /*判断是否有人符合要进的条件*/

   }
   while(!q.QueueEmpty(W))                          /*为等待的顾客服务，但 不允许顾客进来了*/
   {
      curtime++;
      for(i=1;i<=totalnum;i++)
      {                                           /*判断有没有人离开*/
            if((cus[i].serve_flag==TRUE)&&(cus[i].leavetime==curtime))
               customer_leave(i);
      }
      while(chairnum>0 && !q.QueueEmpty(W))         /*让等待队列中的人去理发*/
        customer_serve(q.DeQueue(W));
   }
   max=cus[1].leavetime;                          /*求出最后离开的顾客的离开时间*/
   for(i=2;i<=totalnum;i++)
       max = max < cus[i].leavetime ? cus[i].leavetime : max;
   while(curtime++<max)                           /*队列为空，继续为正在理发的顾客服务*/
   {
     for(i=1;i<=totalnum;i++)
      {
            if((cus[i].serve_flag==TRUE)&&(cus[i].starttime+cus[i].durtime==curtime))
                customer_leave(i);
      }
   }
   addpay=one+two+three;                           /*总营业额*/
   addtime=max-T;
   list();
 }
