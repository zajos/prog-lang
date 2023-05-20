#include <stdio.h>


int main()
{
    double pi=3.14;
    double r;
    printf("Dairenin yaricapini giriniz:\n");
    scanf("%lf",&r);
    float alan=pi*r*r;
    printf("Dairenin alani:\n%f",alan);
    return 0;
}
