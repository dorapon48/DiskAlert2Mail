#include <stdio.h>
#include <stdlib.h>
#include <mntent.h>
#include <sys/statvfs.h>
#include <string.h>

const char *excluded_devices[] = {"none", "tmpfs", "devtmpfs", "sysfs", "proc", "cgroup", "snapfuse", NULL};


int is_excluded_device(const char *device) {
    for (int i = 0; excluded_devices[i] != NULL; i++) {
        if (strcmp(device, excluded_devices[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE *fp;
    struct mntent *mnt;
    struct statvfs stat;
    
    // /etc/mtab を開く（または /proc/mounts を使用することも可能）
    fp = setmntent("/etc/mtab", "r");
    if (fp == NULL) {
        perror("setmntent");
        return EXIT_FAILURE;
    }
    
    printf("Mounted File Systems:\n");
    while ((mnt = getmntent(fp)) != NULL) {
        if (is_excluded_device(mnt->mnt_fsname)) {
            continue;
        }    
        if (statvfs(mnt->mnt_dir, &stat) == 0) {
            unsigned long total = stat.f_blocks * stat.f_frsize;
            unsigned long free = stat.f_bfree * stat.f_frsize;
            unsigned long used = total - free;
            printf("Device: %s\t Mount Point: %s\t Filesystem Type: %s\t Total: %lu bytes\t Used: %lu bytes\t Free: %lu bytes\n",
                   mnt->mnt_fsname, mnt->mnt_dir, mnt->mnt_type, total, used, free);
        } else {
            perror("statvfs");
        }
    }
    
    // ファイルを閉じる
    endmntent(fp);
    return EXIT_SUCCESS;
}
