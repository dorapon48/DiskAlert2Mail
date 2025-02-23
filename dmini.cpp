#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "dmini.hpp"

/**
 * @brief DiskAlert2Mailのiniファイルを読み込む
 */
// コンストラクタ
DMini::DMini(std::string &filename) {
    read_ini(filename);
}
DMini::DMini() {}

/**
 * @brief 文字列の前後の空白を除去
 * @param str 文字列
 * @return 前後の空白を取り除いた文字列
 */
std::string DMini::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    std::string r = "";
    if (start != std::string::npos){
        r = str.substr(start, end - start + 1);
    }
    return r;
}

/**
 * @brief 入力されたiniファイルの読み取り
 * @param iniファイルのパス
 * @return success true
 */
bool DMini::read_ini(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open INI file: " << filename << std::endl;
        return false;
    }

    std::string line, section;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#') continue; // コメント行をスキップ

        if (line[0] == '[' && line.back() == ']') { // セクション開始
            section = line.substr(1, line.size() - 2);
        } else { // キーと値
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = trim(line.substr(0, pos));
                std::string value = trim(line.substr(pos + 1));

                if (section == "global"){
                    set_config_global(key, value);
                } else {
                    set_config_section(section, key, value);
                }
            }
        }
    }
    if (!is_valid()){
        return false;
    }
    return true;
}

/**
 * @brief globalセクションの設定
 * @param key キー
 * @param value 値
 */
void DMini::set_config_global(std::string key, std::string value){
    if (key == "diskUsageMaxLimit") config.disk_usage_max_limit = std::stoi(value);
    else if (key == "toMail") config.to_mail = value;
    else if (key == "fromMail") config.from_mail = value;
    else std::cout << "warning: section [global] The key is not recognized and will be ignored. key: "<< key << std::endl;
}

/**
 * @brief そのほかのセクションの設定
 * @param section セクション
 * @param key キー
 * @param value 値
 */
void DMini::set_config_section(std::string section, std::string key, std::string value){
    int index = -1;
    // すでに存在するセクションか確認
    for (int i = 0; i < config.disk_count; i++) {
        if (config.disks[i].section_name == section){
            index = i;
        }
    }
    if (index != -1){ // ディスク設定の更新
        set_diskconfig_byid(index, key, value);
    } else { // 新しいディスクセクションの追加
        DiskConfig disk;
        disk.section_name = section;
        config.disks.push_back(disk);
        set_diskconfig_byid(config.disk_count, key, value);
        config.disk_count++;
    }
}

/**
 * configのdisksのdiskconfigを設定する．
 * @param disk_id ディスクのインデックス
 * @param key キー
 * @param value 値
 */
void DMini::set_diskconfig_byid(int disk_id, std::string key, std::string value){
    if (key == "mountPath") config.disks[disk_id].mount_path = value;
    else if (key == "diskUsageMaxLimit") config.disks[disk_id].disk_usage_max_limit = stoi(value);
    else std::cout << "warning: section [" << config.disks[disk_id].section_name << "] The key is not recognized and will be ignored. key: "<< key << std::endl;
}

/**
 * @brief configの値が正しいか確認する
 * @return success true
 */
bool DMini::is_valid() {
    // globalセクションの条件
    // diskUsageMaxLimitは0 <= x <= 100
    if (config.disk_usage_max_limit < 0 || config.disk_usage_max_limit > 100){
        std::cerr << "section [global] invalid value(0 <= x <= 100) diskUsageMaxLimit: " << config.disk_usage_max_limit << std::endl;
        return false;
    }
    // globalセクション以外の条件
    for (int i = 0; i < config.disk_count; i++){
        DiskConfig d = config.disks[i];
        if (d.disk_usage_max_limit != INT32_MIN && (d.disk_usage_max_limit < 0 || d.disk_usage_max_limit > 100)){
            std::cerr << "section [" << d.section_name << "] invalid value(0 <= x <= 100) diskUsageMaxLimit: " << d.disk_usage_max_limit << std::endl;
            return false;
        }
    }
    return true;
}

// 設定の表示（デバッグ用）
void DMini::print_config() const {
    std::cout << "[global]" << std::endl;
    std::cout << "diskUsageMaxLimit: " << config.disk_usage_max_limit << std::endl;
    std::cout << "toMail: " << config.to_mail << std::endl;
    std::cout << "fromMail: " << config.from_mail << std::endl;

    for (int i = 0; i < config.disk_count; i++) {
        std::cout << "\n[" << config.disks[i].section_name << "]" << std::endl;
        std::cout << "mountPath: " << config.disks[i].mount_path << std::endl;
        std::cout << "diskUsageMaxLimit: " << config.disks[i].disk_usage_max_limit << std::endl;
    }
}

int main() {
    //DMini dmini("diskalert2mail.ini");

    DMini test = DMini();
    std::string tt = "diskalert2mail.ini";
    if (!test.read_ini(tt)) {
        return 1;
    }

    test.print_config();

    return 0;
}