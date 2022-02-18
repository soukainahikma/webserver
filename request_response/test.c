#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *file = fopen("./test", "r");
    int a[4];
    int ret = fscanf(file, "%d %d %d\n", &a[0], &a[1], &a[2]);
    printf("%d\n", ret);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    ret = fscanf(file, "%d %d %d\n", &a[0], &a[1], &a[2]);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    fscanf(file, "%d %d %d\n", &a[0], &a[1], &a[2]);
    printf("%d\n", ret);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    fscanf(file, "%d %d %d\n", &a[0], &a[1], &a[2]);
    printf("%d\n", ret);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    return 0;
}
