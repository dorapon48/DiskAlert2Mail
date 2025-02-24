import tomllib
from pydantic import BaseModel, ValidationError, Field
from typing import Optional, List

class DiskConfig(BaseModel):
    diskName: str
    mountPath: str
    diskUsageMaxLimit: Optional[int] = Field(None, ge=0, le=100)

class GlobalConfig(BaseModel):
    diskUsageMaxLimit: int = Field(..., ge=0, le=100)
    toMail: str
    fromMail: str

class Config(BaseModel):
    config: GlobalConfig
    disks: List[DiskConfig]

with open("d2m.toml", "rb") as file:  # "rb" でバイナリ読み込み
    config = tomllib.load(file)

print(config)  # 読み込んだ TOML データを表示

try:
    config = Config.model_validate(config)
    print("Valid TOML!")
except ValidationError as e:
    print("Invalid TOML:", e)
except ValueError as e:
    print("Validation Error:", e)