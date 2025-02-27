# DiskAlert2Mail
ディスクの残り容量を監視し，ディスクが規定量に達したら通知するアプリ

## 使用方法
### 1. このページをgit cloneでコピー
```command
git clone https://github.com/dorapon48/DiskAlert2Mail.git
```

### 2. d2m.toml.exampleをコピーし，d2m.tomlに変更
`d2m.toml.example`は`main`ディレクトリに存在します．
```command
cd DiskAlert2Mail/main
cp d2m.toml.example d2m.toml
```

### 3. d2m.tomlの設定
設定の詳細は後述．

### 4. crontabの設定
`crontab`ファイルで起動頻度を設定できます．<br>
初期設定では毎朝8:00にディスクの状態を確認します．

### 5. dockerを起動
`docker-compose.yml`は開発用です．<br>
本番環境では`docker-compose.prod.yml`を起動してください．
```command
docker compose -f docker-compose.prod.yml build
docker compose -f docker-compose.prod.yml up
```

## d2m.toml
設定の詳細です．
