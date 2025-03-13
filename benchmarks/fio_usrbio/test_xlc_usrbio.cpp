#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "hf3fs_usrbio.h"
#include <atomic>
#include <errno.h>
#include <vector>
#include <sys/stat.h>

extern "C" {
    #include "fio.h"
    #include "optgroup.h"
}

constexpr uint64_t NUM_IOS = 1024;
constexpr uint64_t XLC_BLOCK_SIZE = (32 << 20);

int main() {
    struct hf3fs_ior ior;
    hf3fs_iorcreate4(&ior, "/3fs/stage", NUM_IOS, true, 0, 0, -1, 0);
    
    struct hf3fs_iov iov;
    hf3fs_iovcreate(&iov, "/3fs/stage", NUM_IOS * XLC_BLOCK_SIZE, 0, -1);

    int fd = open("/3fs/stage/example.bin", O_RDONLY);
    hf3fs_reg_fd(fd, 0);

    for (int i = 0; i < NUM_IOS; i++) {
        hf3fs_prep_io(&ior, &iov, true, iov.base + i * XLC_BLOCK_SIZE, fd, i * XLC_BLOCK_SIZE, XLC_BLOCK_SIZE, nullptr);
    }
    hf3fs_submit_ios(&ior);

    hf3fs_cqe cqes[NUM_IOS];
    hf3fs_wait_for_ios(&ior, cqes, NUM_IOS, NUM_IOS, nullptr);

    hf3fs_dereg_fd(fd);
    close(fd);
    hf3fs_iovdestroy(&iov);
    hf3fs_iordestroy(&ior);

    return 0;
}