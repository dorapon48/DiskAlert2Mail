#ifndef INI_CONFIG_H
#define INI_CONFIG_H

#include <string>
#include <limits>
#include <vector>

struct DiskConfig {
    std::string section_name;
    std::string mount_path;
    int disk_usage_max_limit = INT32_MIN;
};

struct IniConfig {
    int disk_usage_max_limit = 80;
    std::string to_mail;
    std::string from_mail;
    std::vector<DiskConfig> disks;
    int disk_count = 0;
};

#endif // INI_CONFIG_H