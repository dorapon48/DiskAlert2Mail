#ifndef DISK_ALERT_2_MAIL_ENT_H
#define DISK_ALERT_2_MAIL_ENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISKS 100

typedef struct {
    char *mountPath;
    int diskUsageMaxLimit;
} DiskConfig;

typedef struct {
    int diskUsageMaxLimit;
    char *toMail;
    char *fromMail;
    DiskConfig disks[MAX_DISKS];
    int diskCount;
} IniConfig;

/**
 * @brief INIファイルを解析して設定を読み取る
 * 
 * @param file INIファイルのパス
 * @return FILE
 */
FILE set_dm_inifile(const char *file) FILE {
    FILE *f;
    f = fopen(file, "r");
    return f;
}

#endif // INI_PARSER_H
