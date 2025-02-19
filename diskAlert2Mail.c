#include <stdio.h>
#include <stdlib.h>
#include <mntent.h>

int main() {
    FILE *fp;
    FILE *conf;
    struct mntent *mnt;
    
    // /etc/mtab を開く（または /proc/mounts を使用することも可能）
    fp = setmntent("/etc/mtab", "r");
    if (fp == NULL) {
        perror("setmntent");
        return EXIT_FAILURE;
    }

    conf = fopen("diskAlert2Mail.conf", "r");
    if (conf == NULL) {
        perror("");
        return EXIT_FAILURE;
    }
    
    printf("Mounted File Systems:\n");
    while ((mnt = getmntent(fp)) != NULL) {
        printf("Device: %s\t Mount Point: %s\t Filesystem Type: %s\n",
               mnt->mnt_fsname, mnt->mnt_dir, mnt->mnt_type);
    }
    
    // ファイルを閉じる
    endmntent(fp);
    return EXIT_SUCCESS;
}
