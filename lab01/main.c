#include <stdio.h>


int main()
{
    int integer1;
    int integer2;
    int sum;
    printf("Ilk tam sayiyi giriniz:\n");
    scanf("%d",&integer1);
    printf("Diger tam sayiyi giriniz;\n");
    scanf("%d",&integer2);
    sum=integer1+integer2;
    printf("Iki sayinin toplami:\n%d",sum);
    return 0;
}
