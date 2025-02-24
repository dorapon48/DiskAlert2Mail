import tomllib
import subprocess

with open("d2m.toml", "rb") as file:  # "rb" でバイナリ読み込み
    config = tomllib.load(file)

print(config)  # 読み込んだ TOML データを表示

result = subprocess.run(["df", "-h", "/mnt"], capture_output=True, text=True)
lines = result.stdout.split("\n")
for i in lines:
    print(i)