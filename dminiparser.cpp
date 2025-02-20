#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

map<string, string> ReadINI(const string& filename) {
    ifstream file(filename);
    map<string, string> config;
    string line, section;

    while (getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t")); // 先頭の空白を削除
        if (line.empty() || line[0] == ';' || line[0] == '#') continue; // コメント行をスキップ

        if (line[0] == '[') { // セクション開始
            section = line.substr(1, line.find(']') - 1);
        } else { // キーと値
            size_t pos = line.find('=');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                key.erase(key.find_last_not_of(" \t") + 1); // 末尾の空白削除
                value.erase(0, value.find_first_not_of(" \t")); // 先頭の空白削除
                config[section + "." + key] = value;
            }
        }
    }
    return config;
}

int main() {
    auto config = ReadINI("config.ini");

    cout << "Settings.KeyName: " << config["Settings.KeyName"] << endl;

    return 0;
}