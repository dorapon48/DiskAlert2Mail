import tomllib
import os
from pydantic import ValidationError
from model import Config

def read_toml():
    """
    d2m.tomlを読み込む, 読み込む場所はこのファイルがある場所
    """
    # スクリプトがあるディレクトリのパスを取得
    script_dir = os.path.dirname(os.path.abspath(__file__))  # スクリプトの場所を取得
    toml_path = os.path.join(script_dir, "d2m.toml")  # TOML のパスを設定

    with open(toml_path, "rb") as file:  # "rb" でバイナリ読み込み
        config = tomllib.load(file)

    # print(config)  # 読み込んだ TOML データを表示
    # tomlが正しい型をしているか確認
    try:
        config = Config.model_validate(config)
        print(config)
        print("Valid TOML!")
        return config
    except ValidationError as e:
        print("Invalid TOML:", e)
        return None
    except ValueError as e:
        print("Validation Error:", e)
        return None

# read_toml()