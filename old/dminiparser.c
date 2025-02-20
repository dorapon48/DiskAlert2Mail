#include "dminiparser.h"

IniConfig read_dm_ini(const char *filename) {
    // init
    IniConfig config;
    config.diskCount = 0;
    config.diskUsageMaxLimit = 80; // デフォルト値
    strcpy(config.toMail, "");
    strcpy(config.fromMail, "");

    // ファイル読み込み
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open INI file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[256];
    char section[MAX_DISKS] = "";
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '[') { // セクション判定
            sscanf(line, "[%49[^]]]", section);
        } else if (strchr(line, '=')) { // key=value の行
            char *key, *value;
            sscanf(line, "%49[^=] = %255[^\n]", key, value);

            if (strcmp(section, "global") == 0) {
                if (strcmp(key, "diskUsageMaxLimit") == 0) {
                    config.diskUsageMaxLimit = atoi(value);
                } else if (strcmp(key, "toMail") == 0) {
                    strncpy(config.toMail, value, MAX_PATH_LENGTH);
                } else if (strcmp(key, "fromMail") == 0) {
                    strncpy(config.fromMail, value, MAX_PATH_LENGTH);
                }
            } else if (config.diskCount < MAX_DISKS) {
                DiskConfig *disk = &config.disks[config.diskCount];
                if (strcmp(key, "mountPath") == 0) {
                    strncpy(disk->mountPath, value, MAX_PATH_LENGTH);
                } else if (strcmp(key, "diskUsageMaxLimit") == 0) {
                    disk->diskUsageMaxLimit = atoi(value);
                }
                config.diskCount++;
            }
        }
    }
    fclose(file);
    return config;
}