#include <stdio.h>

typedef struct s_test
{
    char *name;
} t_test;

void    test(char **save)
{
    *save = "hello";
}

int main(){

    t_test t;

    printf("%s\n", t.name);

    printf("\n");
    test(&(t.name));

    printf("%s\n", t.name);

}