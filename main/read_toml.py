import tomllib

with open("d2m.toml", "rb") as file:  # "rb" でバイナリ読み込み
    config = tomllib.load(file)

print(config)  # 読み込んだ TOML データを表示

