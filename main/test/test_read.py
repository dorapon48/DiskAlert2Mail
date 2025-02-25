import tomllib
from pydantic import BaseModel, Field, ValidationError, model_validator
from typing import Optional, List


class DiskConfig(BaseModel):
    diskName: str
    mountPath: str
    diskUsageMaxLimit: Optional[int] = Field(None, ge=0, le=100)


class MailConfig(BaseModel):
    toAddress: str
    fromAddress: str
    smtpAddress: str
    smtpPort: int
    smtpUser: str
    smtpPassword: str


class GlobalConfig(BaseModel):
    diskUsageMaxLimit: int = Field(..., ge=0, le=100)
    executionCycle: int = Field(..., ge=1)
    mailconfig: MailConfig


class Config(BaseModel):
    config: GlobalConfig
    disks: List[DiskConfig]

    @model_validator(mode="before")
    @classmethod
    def apply_global_disk_limit(cls, values):
        """ 各ディスクの `diskUsageMaxLimit` が None の場合、グローバル設定を適用 """
        global_limit = values["config"]["diskUsageMaxLimit"]
        for disk in values.get("disks", []):
            disk.setdefault("diskUsageMaxLimit", global_limit)
        return values


def load_config_from_toml(toml_s: str) -> Optional[Config]:
    """ TOML ファイルを読み込み、Config にバリデーションを適用 """
    try:
        data = tomllib.loads(toml_s)
        return Config.model_validate(data)
    except ValidationError as e:
        print("バリデーションエラー:")
        print(e.json(indent=2))
        return None
    except KeyError as e:
        print(f"設定ファイルのキーが不足しています: {e}")
        return None


# テスト用 TOML データ
toml_data = """
[config]
diskUsageMaxLimit = 80
executionCycle = 24
toAddress = "admin@example.com"
fromAddress = "noreply@example.com"
smtpAddress = "smtp.example.com"
smtpPort = 587
smtpUser = "user"
smtpPassword = "password"

[[disks]]
diskName = "test disk"
mountPath = "/path/to/disk"

[[disks]]
diskName = "test disk 2"
mountPath = "/path/to/disk2"
diskUsageMaxLimit = 10
"""

# バリデーションを実行
config = load_config_from_toml(toml_data)
if config:
    print("バリデーション成功:")
    print(config.model_dump_json(indent=2))