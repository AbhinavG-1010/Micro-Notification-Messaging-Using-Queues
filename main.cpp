#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
struct Qnode
{
 char message[1000];
 struct Qnode *next;
 int number,admin;
};
struct Queue{
 struct Qnode *front,*rear;
};
void enQueue(struct Queue* q, char m[1000],int n,int ad)
{
 if (q->rear == NULL)
 {
 q->rear = (struct Qnode *)malloc(1*sizeof(struct Qnode));
 q->rear->next = NULL;
 q->rear->number = n;
 q->rear->admin = ad;
 strcpy(q->rear->message,m);
 q->front = q->rear;
 }
 else
 {
 struct Qnode *temp=(struct Qnode *)malloc(1*sizeof(struct
 Qnode));
 q->rear->next = temp;
 temp->number = n;
 strcpy(temp->message,m);
 temp->next = NULL;
 temp->admin=ad;
 q->rear = temp;
 }
}
void deQueue(struct Queue *q)
{
 FILE *f;
 char buf[0x100];
 snprintf(buf, sizeof(buf), "%d_Inbox.txt",q->front->number);
 f=fopen(buf,"a");
 time_t currentTime;
 time(&currentTime);
 if(q->front->admin==1)
 fprintf(f,"%s: %s<sent by ADMIN>\n\n",ctime(&currentTime),q->front-
>message);
 else
 fprintf(f,"%s: %s\n\n",ctime(&currentTime),q->front->message);
 fclose(f);
 f=fopen("sentmessages.txt","a");
 if(q->front->admin==1)
 fprintf(f,"%s: %sReceiver: Customer no. %d\n<sent by ADMIN>\n\n",ctime(&curre
ntTime),q->front->message,q->front->number);
 else
 fprintf(f,"%s: %sReceiver: Customer no. %d\n\n",ctime(&currentTime),q->front-
>message,q->front->number);
 fclose(f);
 struct Qnode *temp = q->front;
 q->front = q->front->next;
 if (q->front == NULL)
 q->rear = NULL;
 free(temp);
}
int toint(char str[])
{
 int len = strlen(str);
 int i, num = 0;
 for (i = 0; i < len; i++)
 {
 num = num + ((str[len - (i + 1)] - '0') * pow(10, i));
 }
 return num;
}
void tostring(char str[], int num)
{
 int i, rem, len = 0, n;
 n = num;
 while (n != 0)
 {
 len++;
 n /= 10;
 }
 for (i = 0; i < len; i++)
 {
 rem = num % 10;
 num = num / 10;
 str[len - (i + 1)] = rem + '0';
 }
 str[len] = '\0';
}
int main()
{
 int customerlist,customeradd,adm;
 char pwd[20],pin[20];
 printf("\nEnter the pin to access the program as admin: ");
 scanf("%s",pwd);
 FILE *x;
 x=fopen("pin.txt","r");
 fgets(pin,20,x);
 fclose(x);
 if(strcmp(pwd,pin)==0)
 adm=1;
 else
 adm=0;
 if (adm==1)
 {
 int key;
 char pswd[20];
 printf("Do you want to change the pin (1-Yes,0-No): ");
 scanf("%d",&key);
 if(key==1)
 {
 x=fopen("pin.txt","w");
 printf("Enter the new pin: ");
 scanf("%s",pswd);
 fprintf(x,pswd);
 fclose(x);
 }
 do
 {
 printf("Do you want to add a customer (1-Yes, 0-No): ");
 scanf("%d",&customeradd);
 if(customeradd==1)
 {
 char n[10],name[100];
 printf("\nEnter the name of the customer: ");
 scanf("%s", name);
 FILE *f;
 f=fopen("customernumber.txt","r");
 fgets(n,10,f);
 fclose(f);
 int num=toint(n);
 num=num+1;
 f=fopen("customernumber.txt","w");
 tostring(n,num);
 fputs(n,f);
 fclose(f);;
 char buf[0x100];
 snprintf(buf, sizeof(buf), "%s_Inbox.txt", n);
 f=fopen(buf,"w");
 fprintf(f,"Customer Name: %s\nReceived Messages:\n",name);
 fclose(f);
 f=fopen("customerlist.txt","a");
 fprintf(f,"%d: %s\n",num,name);
 fclose(f);
 }
 }
 while(customeradd!=0);
 }
 customerlist=1;
 if (customerlist==1)
 {
 int size;
 char string[1000];
 FILE *f;
 f=fopen("customerlist.txt","r");
 fseek(f,0,SEEK_END);
 size=ftell(f);
 if(size==0)
 {
 printf("No customers to display\n");
 exit(0);
 }
 else
 {
 printf("Customer List:\n");
 fseek(f,0,SEEK_SET);
 while(fgets(string,1000,f)!=NULL)
 printf("%s",string);
 fclose(f);
 }
 }
 printf("\n");
 FILE *f;
 char m[1000],cn[4];
 int num,no_of_messages=0,i,cnum;
 struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
 queue->front = queue->rear = NULL;
 FILE *t;
 t=fopen("customernumber.txt","r");
 fgets(cn,4,t);
 fclose(t);
 cnum=toint(cn);
 f=fopen("typemessage.txt","r");
 if(f!=NULL)
 {
 while(fgets(m,1000,f)!=NULL)
 {
 printf("%s\nEnter the number of the customer to whom the above messag
e is to be sent: ",m);
 scanf("%d",&num);
 printf("\n");
 if(num==0)
 {
 int z;
 for (z=1;z<=cnum;z++)
 {
 enQueue(queue,m,z,adm);
 no_of_messages=no_of_messages+1;
 }
 }
 else if(num>0&&num<=cnum)
 {
 enQueue(queue,m,num,adm);
 no_of_messages=no_of_messages+1;
 }
 else
 {
 while(num>cnum||num<0)
 {
 printf("Customer does not exist! Enter a customer number (0-
%d): ",cnum);
 scanf("%d",&num);
 }
 if(num==0)
 {
 int z;
 for (z=1;z<=cnum;z++)
 {
 enQueue(queue,m,z,adm);
 no_of_messages=no_of_messages+1;
 }
 }
 else
 {
 enQueue(queue,m,num,adm);
 no_of_messages=no_of_messages+1;
 }
 }
 }
 for(i=0;i<no_of_messages;i++)
 {
 deQueue(queue);
 }
 printf("All messages are sent!\n");
 }
 else
 printf("No messages to be sent!\n");
 fclose(f);
 getchar();
 return 0;
}
