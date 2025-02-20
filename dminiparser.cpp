#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

// 最大の探知diskは100個
#define MAX_DISKS 100

/**
 * @brief DiskAlert2Mailのiniファイルを読み込む
 */
class DMini {

    struct DiskConfig {
        string sectionName;
        string mountPath;
        int diskUsageMaxLimit;
    };

    struct IniConfig {
        int diskUsageMaxLimit;
        string toMail;
        string fromMail;
        DiskConfig disks[MAX_DISKS];
        int diskCount = 0;
    };

    IniConfig config;
public:
    // コンストラクタ
    DMini(const string& filename) {
        read_ini(filename);
    }
    DMini() {}

    // INIファイルの読み込み
    bool read_ini(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Failed to open INI file: " << filename << endl;
            return false;
        }

        string line, section;

        while (getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t")); // 先頭の空白を削除
            line.erase(line.find_last_not_of(" \t") + 1); // 末尾の空白を削除
            if (line.empty() || line[0] == ';' || line[0] == '#') continue; // コメント行をスキップ

            if (line[0] == '[' && line.back() == ']') { // セクション開始
                section = line.substr(1, line.size() - 2);
            } else { // キーと値
                size_t pos = line.find('=');
                if (pos != string::npos) {
                    string key = line.substr(0, pos);
                    string value = line.substr(pos + 1);
                    key.erase(key.find_last_not_of(" \t") + 1); // 末尾の空白削除
                    value.erase(0, value.find_first_not_of(" \t")); // 先頭の空白削除

                    // `set_config` で設定を適用
                    set_config(section, key, value);
                }
            }
        }
        return true;
    }

    /**
     * @brief 入力された値をconfigに登録する
     * @param section セクション
     */
    bool set_config(string section, string key, string value) {
        if (section == "global") {
            if (key == "diskUsageMaxLimit") config.diskUsageMaxLimit = stoi(value);
            else if (key == "toMail") config.toMail = value;
            else if (key == "fromMail") config.fromMail = value;
            return true;
        }

        // ディスク設定の更新
        for (int i = 0; i < config.diskCount; i++) {
            if (config.disks[i].sectionName == section) {
                if (key == "mountPath") config.disks[i].mountPath = value;
                else if (key == "diskUsageMaxLimit") config.disks[i].diskUsageMaxLimit = stoi(value);
                return true;
            }
        }

        // 新しいディスクセクションの追加
        if (config.diskCount < MAX_DISKS) {
            config.disks[config.diskCount].sectionName = section;
            if (key == "mountPath") config.disks[config.diskCount].mountPath = value;
            else if (key == "diskUsageMaxLimit") config.disks[config.diskCount].diskUsageMaxLimit = stoi(value);
            config.diskCount++;
            return true;
        }

        return false;
    }

    // 設定の表示（デバッグ用）
    void print_config() const {
        cout << "[global]" << endl;
        cout << "diskUsageMaxLimit: " << config.diskUsageMaxLimit << endl;
        cout << "toMail: " << config.toMail << endl;
        cout << "fromMail: " << config.fromMail << endl;

        for (int i = 0; i < config.diskCount; i++) {
            cout << "\n[" << config.disks[i].sectionName << "]" << endl;
            cout << "mountPath: " << config.disks[i].mountPath << endl;
            cout << "diskUsageMaxLimit: " << config.disks[i].diskUsageMaxLimit << endl;
        }
    }
};

int main() {
    DMini dmini("diskalert2mail.ini");

    dmini.print_config();

    return 0;
}