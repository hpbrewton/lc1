#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BUFFSIZE 2048

int nstates;
int nlength;
int *targets;
float *zprobs;
char buffer[BUFFSIZE];

void 
dump_state() {
    for (int i = 0; i < nstates; ++i) {
        printf("%d: %d(%f) %d(%f)\n", 
            i,
            targets[i*2+0],
            zprobs[i],
            targets[i*2+1],
            1-zprobs[i]);
    }
}

void
assign_states() {
    for (int i = 0; i < nstates; ++i) {
        targets[i*2+0] = rand()%nstates;
        targets[i*2+1] = rand()%nstates;
        zprobs[i] = (float)rand()/(float)(RAND_MAX);
    }
}

void 
produce() {
    int curr = 0;
    for (int i = 0; i < nlength; ++i) {
        for (int j = 0; j < BUFFSIZE; ++j) {
            char b = 0;
            for (int k = 0; k < 8; ++k) {
                float r = (float)rand()/(float)(RAND_MAX);
                if (r < zprobs[i]) {
                    curr = targets[curr*2+0];
                } else {
                    curr = targets[curr*2+1];
                    b |= 1;
                }
                b = b << 1;
            }
            buffer[j] = b;
        }
        size_t written=0;
        while (written < BUFFSIZE) {
            size_t res = write(1, &buffer[written], BUFFSIZE-written);
            if (res < 0) {
                //panic
                exit(-1);
            }
            written += res;
        }
    } 
}

int
main(int argc, char *argv[]) {
    time_t t;
    srand((unsigned) time(&t));

    nstates = atoi(argv[1]);
    nlength = atoi(argv[2]);
    nlength = BUFFSIZE-(nlength%2048)+nlength;
    nlength = nlength/BUFFSIZE;
    int i_targets[2*nstates];
    targets = (int *) i_targets;
    float i_zprobs[nstates];
    zprobs  = (float*) i_zprobs;
    
    assign_states();
    produce();
}