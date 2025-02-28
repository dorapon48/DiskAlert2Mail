# DiskAlert2Mail
ディスクの残り容量を監視し，ディスクが規定量に達したら通知するアプリ

# 使用方法
## 1. このページをgit cloneでコピー
```command
git clone https://github.com/dorapon48/DiskAlert2Mail.git
```

## 2. d2m.toml.exampleをコピーし，d2m.tomlに変更
`d2m.toml.example`は`main`ディレクトリに存在します．
```command
cd DiskAlert2Mail/main
cp d2m.toml.example d2m.toml
```

## 3. d2m.tomlの設定
設定の詳細は後述．

## 4. crontabの設定
`crontab`ファイルで起動頻度を設定できます．<br>
初期設定では毎朝8:00にディスクの状態を確認します．

## 5. dockerを起動
`docker-compose.yml`は開発用です．<br>
本番環境では`docker-compose.prod.yml`を起動してください．
```command
docker compose -f docker-compose.prod.yml build
docker compose -f docker-compose.prod.yml up
```

# `config.toml` の設定項目説明
このファイルはサーバーやディスクの設定，およびメール送信の設定を管理するためのTOML形式の設定ファイルです．

## **設定項目一覧**

### **全体設定 `[config]`**
| 変数名                | 型       | デフォルト値 | 説明 |
|----------------------|---------|-------------|-----------------------------------|
| `diskUsageMaxLimit` | `int` | `80`        | ディスク使用率通知の下限 (%)       |
| `serverName`        | `string`  | `""`        | サーバーの名前                    |
| `serverIP`         | `string`  | `""`        | サーバーのIPアドレス               |

### **メール設定 `[mail]`**
| 変数名         | 型       | デフォルト値       | 説明 |
|---------------|---------|------------------|--------------------------------------|
| `toAddress`   | `string`  | `""`             | メールの送り先                        |
| `fromAddress` | `string`  | `""`             | メールの送り主                        |
| `smtpAddress` | `string`  | `"smtp.gmail.com"` | SMTPサーバーのアドレス                |
| `smtpPort`    | `int` | `587`            | SMTPサーバーのポート番号              |
| `smtpUser`    | `string`  | `""`             | SMTPサーバーのユーザー名              |
| `smtpPassword`| `string`  | `""`             | SMTPサーバーのパスワード              |

### **ディスク設定 `[[disks]]`**
このセクションはリストとして複数のディスクを設定できます．

| 変数名               | 型       | デフォルト値      | 説明 |
|---------------------|---------|-----------------|----------------------------------------|
| `diskName`         | `string`  | `"disk1"`       | ディスクの表示名                         |
| `mountPath`        | `string`  | `"/path/to/disk"` | `df` コマンドで表示されるマウントパス   |
| `diskUsageMaxLimit`| `int` | `50`           | ディスク使用率通知の下限 (%)（この設定が `[config]` の `diskUsageMaxLimit` より優先） |

## **詳細説明**

### `[config]`
- サーバー全体の基本設定を記述します．
- `diskUsageMaxLimit` は，ディスクの使用率がこの値を超えた場合に通知を送るための閾値です．
- `serverName`，`serverIP`はメールの表示に使用します．

### `[mail]`
- メール送信の設定を記述します．
- `smtpAddress` にはSMTPサーバーのアドレスを指定します（例: Gmailなら `"smtp.gmail.com"`）．
- `smtpPort` は通常，`587`（TLS）または `465`（SSL）を使用します．
- `smtpUser` と `smtpPassword` には認証情報を設定します．

### `[[disks]]`
- 設定ファイル内で複数の `[[disks]]` セクションを定義することで，複数のディスクを管理できます．
- `diskUsageMaxLimit` は `[config]` の設定よりも個別の `[[disks]]` セクションに設定した値が優先されます．
