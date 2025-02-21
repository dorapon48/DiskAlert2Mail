#ifndef DM_INI_H
#define DM_INI_H

#include <string>
#include <limits>
#define MAX_DISKS 100

class DMini {
    private:
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

    IniConfig config;

    bool read_ini(std::string& filename);
    bool set_config(std::string section, std::string key, std::string value);
    bool is_valid();

    public:
    // コンストラクタ
    DMini(std::string& filename);
    DMini();

    // getter
    int get_global_disk_usage_max_limit();
    std::string get_to_mail();
    std::string get_from_mail();
    int get_disk_count();
    std::string get_section_name(int disk_id);
    std::string get_mount_path(int disk_id);
    int get_other_disk_usage_max_limit(int disk_id);

    // debug
    void print_config() const;

};

#endif // DM_INI_H
