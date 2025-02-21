#ifndef INI_CONFIG_H
#define INI_CONFIG_H

#include <string>
#include <limits>
#define MAX_DISKS 100

struct DiskConfig {
    std::string section_name = "";
    std::string mount_path = "";
    int disk_usage_max_limit = INT32_MIN;
};

struct IniConfig {
    int disk_usage_max_limit = INT32_MIN;
    std::string to_mail = "";
    std::string from_mail = "";
    DiskConfig disks[MAX_DISKS];
    int disk_count = 0;
};

#endif // INI_CONFIG_H