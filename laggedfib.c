#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int 
max(int i, int j) {
    if (i > j) {
        return i;
    } else {
        return j;
    }
}

int 
main(int argc, char *argv[]) {
    int k = atoi(argv[1]);
    int j = atoi(argv[2]);
    int l = atoi(argv[3]);
    int m = max(k,j);
    if (l < m) {
        fprintf(stderr, "Length being less than max lag is unspported");
        exit(-1);
    }
    char seq[l];
    for (int i = 0; i < m; ++i) {
        seq[i] = (char) (i % 256);
    }
    for (int i = m; i < l; ++i) seq[i] = (seq[i-j]+seq[i-k]);
    int written = 0;
    while (written < l) {
        int res = write(1, &seq[written], l-written);
        if (res < 0) exit(-1);
        written+=res;
    }
}