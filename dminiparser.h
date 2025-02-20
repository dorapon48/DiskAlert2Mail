#ifndef DM_INI_PARSER_H
#define DM_INI_PARSER_H

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
 * @return INiConfig
 */
IniConfig read_dm_ini(const char *__file);

#endif // INI_PARSER_H
