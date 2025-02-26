from pydantic import BaseModel, Field, model_validator
from typing import Optional, List

class DiskConfig(BaseModel):
    diskName: str
    mountPath: str
    diskUsageMaxLimit: Optional[int] = Field(None, ge=0, le=100)

    # mountPathが/のみの時, 空文字列を設定する
    @model_validator(mode="after")
    def mount_path_slash(self):
        if self.mountPath == "/":
            self.mountPath = ""
        return self

class MailConfig(BaseModel):
    toAddress: str
    fromAddress: str
    smtpAddress: str
    smtpPort: int
    smtpUser: str
    smtpPassword: str

class GlobalConfig(BaseModel):
    serverName: str
    serverIP: str
    diskUsageMaxLimit: int = Field(..., ge=0, le=100)

class Config(BaseModel):
    """
    configの形式指定
    """
    config: GlobalConfig
    mail: MailConfig
    disks: List[DiskConfig]

    @model_validator(mode="after")
    def apply_global_disk_limit(self):
        for disk in self.disks:
            if disk.diskUsageMaxLimit is None:  # diskUsageMaxLimit が指定されていない場合
                disk.diskUsageMaxLimit = self.config.diskUsageMaxLimit  # グローバル値を代入
        return self

class DiskUseage(BaseModel):
    filesystem: str
    size: int
    used: int
    available: int
    mounted_on: str

class MailBody(BaseModel):
    config: DiskConfig
    df: DiskUseage