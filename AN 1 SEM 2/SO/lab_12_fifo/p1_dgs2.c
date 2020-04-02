#include<stdlib.h>



int main(int argc, char* argv[])
{
//.....
int p[2];
pid_t pid;
char c;


pipe(p);

if(-1==(pid=fork()))
{printf("eroare");
exit(1);}


if(pid==0){
close(p[1]);

char sir[1000];
int len;

read(p[0],&len,sizeof(len));
read(p[0],sir,len);
printf(sir);

close(p[0]);
}
else{
close(p[0]);

int len=0;
char sir[1000];


do{
scanf("%s",c);
if(c!='x');
if((c!='a')&&(c!='e')&&(c!='o')&&(c!='u')&&(c!='i'))
sir[len++]=c;

}while(c!='x');

write(p[1],&len,sizeof(len));
write(p[1],sir,len);

close(p[1]);
}

return 0;}
