#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "dmini.hpp"

// 最大の探知diskは100個
#define MAX_DISKS 100

/**
 * @brief DiskAlert2Mailのiniファイルを読み込む
 */
IniConfig config;
// コンストラクタ
DMini::DMini(std::string &filename) {
    read_ini(filename);
}
DMini::DMini() {}

/**
 * @brief 入力されたiniファイルの読み取り
 * @param iniファイルのパス
 * @return success true
 */
bool DMini::read_ini(std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open INI file: " << filename << std::endl;
        return false;
    }

    std::string line, section;

    while (getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t")); // 先頭の空白を削除
        line.erase(line.find_last_not_of(" \t") + 1); // 末尾の空白を削除
        if (line.empty() || line[0] == ';' || line[0] == '#') continue; // コメント行をスキップ

        if (line[0] == '[' && line.back() == ']') { // セクション開始
            section = line.substr(1, line.size() - 2);
        } else { // キーと値
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                key.erase(key.find_last_not_of(" \t") + 1); // 末尾の空白削除
                value.erase(0, value.find_first_not_of(" \t")); // 先頭の空白削除

                set_config(section, key, value);
            }
        }
    }
    if (!is_valid()){
        return false;
    }
    return true;
}

/**
 * @brief 入力された値をconfigに登録する
 * @param section セクション
 * @param key キー
 * @param value 値
 * @return success true
 */
bool DMini::set_config(std::string section, std::string key, std::string value) {
    if (section == "global") {
        if (key == "diskUsageMaxLimit") config.disk_usage_max_limit = std::stoi(value);
        else if (key == "toMail") config.to_mail = value;
        else if (key == "fromMail") config.from_mail = value;
        return true;
    }

    // ディスク設定の更新
    for (int i = 0; i < config.disk_count; i++) {
        if (config.disks[i].section_name == section) {
            if (key == "mountPath") config.disks[i].mount_path = value;
            else if (key == "diskUsageMaxLimit") config.disks[i].disk_usage_max_limit = stoi(value);
            return true;
        }
    }

    // 新しいディスクセクションの追加
    if (config.disk_count < MAX_DISKS) {
        config.disks[config.disk_count].section_name = section;
        if (key == "mountPath") config.disks[config.disk_count].mount_path = value;
        else if (key == "diskUsageMaxLimit") config.disks[config.disk_count].disk_usage_max_limit = stoi(value);
        config.disk_count++;
        return true;
    }

    return false;
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