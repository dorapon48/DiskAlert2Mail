#ifndef DM_INI_H
#define DM_INI_H

#include <string>
#include <limits>
#include "iniconfig.hpp"
#define MAX_DISKS 100

class DMini {
    private:
    IniConfig config;

    // method
    std::string trim(const std::string& str);
    bool set_config(std::string section, std::string key, std::string value);
    void set_config_global(std::string key, std::string value);
    void set_config_section(std::string section, std::string key, std::string value);
    void set_diskconfig_byid(int disk_id, std::string key, std::string value);
    bool is_valid();

    public:
    // コンストラクタ
    DMini(std::string& filename);
    DMini();

    // method
    bool read_ini(const std::string& filename);

    // getter
    IniConfig get_config();

    // debug
    void print_config() const;

};

#endif // DM_INI_H
