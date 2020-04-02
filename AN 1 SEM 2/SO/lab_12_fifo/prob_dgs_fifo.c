#include<stdlib.h>



int main(int argc, char* argv[])
{
//.....
pid_t pid;
char c;
int fdr,fdw;

fdw=open("fifo",'w');

char sir[1000];
int len;

read(fdw,&len,sizeof(len));
read(fdw,sir,len);
printf(sir);

return 0;}
