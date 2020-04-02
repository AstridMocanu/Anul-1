////nota!! in problema sa cere sa folosim fct chmod!
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pwd.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include <sys/stat.h>
struct stat fs;
void permisiunile_sunt(struct stat x){
    printf( (x.st_mode & S_IRUSR) ? "r" : "-");
    printf( (x.st_mode & S_IWUSR) ? "w" : "-");
    printf( (x.st_mode & S_IXUSR) ? "x" : "-");
    printf( (x.st_mode & S_IRGRP) ? "r" : "-");
    printf( (x.st_mode & S_IWGRP) ? "w" : "-");
    printf( (x.st_mode & S_IXGRP) ? "x" : "-");
    printf( (x.st_mode & S_IROTH) ? "r" : "-");
    printf( (x.st_mode & S_IWOTH) ? "w" : "-");
    printf( (x.st_mode & S_IXOTH) ? "x" : "-");
}



int main (int argc, char* argv[])
{
char fis1[500];
char a,b;
//int x,z;


if(argc>2)
{
perror("prea multe arg citite");
exit(1);
}

else
if(argc == 1)
{
printf("dati fisier:");
scanf("%s",fis1);
printf("Pentru a oferi perm proprietar apasati 1;\n");
printf("Pentru a oferi perm grup apasati 2;\n");
printf("Pentru a oferi perm altii apasati 3;\n");

scanf("%c",&a);
scanf("%c",&a);
printf("Dati numarul corespunzator permisiunii(1-x,2-w,4-r)\n");
scanf("%c",&b);
scanf("%c",&b);
//printf("%c",a);
//printf("%c",b);
}
else
if(argc == 2)
{
strcpy(fis1, argv[1]);
printf("Pentru a oferi perm proprietar apasati 1;\n");
printf("Pentru a oferi perm grup apasati 2;\n");
printf("Pentru a oferi perm altii apasati 3;\n");

scanf("%c",&a);
scanf("%c",&a);
printf("Dati numarul corespunzator permisiunii(1-x,2-w,4-r)\n");
scanf("%c",&b);
scanf("%c",&b);


}

stat(fis1,&fs);
permisiunile_sunt(fs);
printf("\n");

///chmod imi schimba in true r,w sau x doar pe una si pe restul le face 0


//printf("Pentru a oferi perm tuturor apasati 3;");
if(a=='1'){
//printf("am intrat");

if(b=='4'){
if(chmod(fis1,S_IRUSR)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}}
else
if(b=='2'){
if(chmod(fis1,S_IWUSR)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}}
else
if(b=='1')
if(chmod(fis1,S_IXUSR)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}




//acces(fis1,b*100+x);

}

if(a=='2'){
if(b=='4'){
if(chmod(fis1,S_IRGRP)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}
}

else
if(b=='2'){
if(chmod(fis1,S_IWGRP)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}}
else
if(b=='1'){
if(chmod(fis1,S_IXGRP)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}}


}

if(a=='3'){
if(b=='4'){
if(chmod(fis1,S_IROTH)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}
}
else
if(b=='2'){
if(chmod(fis1,S_IWOTH)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}}
else
if(b=='1'){
if(chmod(fis1,S_IXOTH)==-1){printf("Nu s-a putut efectua modificarea");exit(0);}}


}
close(fis1);
stat(fis1,&fs);
permisiunile_sunt(fs);
printf("\n");


return 0;
}

