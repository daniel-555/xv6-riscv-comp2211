#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int upTime = uptime();

    fprintf(1, "The uptime is %d ticks\n", upTime);

    return 0;
}