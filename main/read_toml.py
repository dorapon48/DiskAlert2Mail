import tomllib
from pydantic import ValidationError
from model import Config

with open("d2m.toml", "rb") as file:  # "rb" でバイナリ読み込み
    config = tomllib.load(file)

# print(config)  # 読み込んだ TOML データを表示

try:
    config = Config.model_validate(config)
    print(config)
    print("Valid TOML!")
except ValidationError as e:
    print("Invalid TOML:", e)
except ValueError as e:
    print("Validation Error:", e)