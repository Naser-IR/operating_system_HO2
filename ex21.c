#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
//#include <sys/fcntl.h>

int main(int argc, char**argv ) {
    if (argc !=3 ) {
        printf("not enough argument\n");
        return 0;
    }
    char a1;
    char a2;
    int f1;
    int f2;
    int sum1 = 0;
    int sum2 = 0;
    f1 = open(argv[1], O_RDONLY); //closeeee
    f2 = open(argv[2],O_RDONLY);
    while (1){
        ssize_t x1 = read(f1, &a1, 1);
        ssize_t x2 = read(f2, &a2, 1);
        if(a1 != a2)
            break;
        if(x1 == 0 && x2==0) {
            // printf("k");
            return 1;
        }
    }
    close(f1);
    close(f2);
    f1 = open(argv[1], O_RDONLY);
    f2 = open(argv[2],O_RDONLY);
    int cur1;
    int cur2;
    while (1){
        ssize_t x1 = read(f1, &a1, 1);
        ssize_t x2 = read(f2, &a2, 1);
        //turn all the letter to capital


        while ((a1 == ' ' || a1 == '\n') && x1 !=0) {
            x1 = read(f1, &a1, 1);
        }
       if ((a1 != ' ' && a1 != '\n'))
            cur1 = (int) a1;
        while ((a2 == ' ' || a2 == '\n') && x2 !=0) {
            x2 = read(f2, &a2, 1);
        }
        if ((a2 != ' ' && a2 != '\n'))
            cur2 = (int) a2;
//        printf("%c\n",a1);
//        printf("%c\n",a2);
        if (97 <= cur1 && cur1 <= 122) {
            cur1 = cur1 - 32;
        }
        if (97 <= cur2 && cur2 <= 122) {
            cur2 = cur2 - 32;
        }
        if (cur2 != cur1){
            break;
        }

        if(x1 == 0 && x2==0) {
            return 3;
        }
    }
    //  printf("diff");
    return 2;
}
