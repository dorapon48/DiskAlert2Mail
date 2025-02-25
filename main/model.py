from pydantic import BaseModel, Field, model_validator
from typing import Optional, List

class DiskConfig(BaseModel):
    diskName: str
    mountPath: str
    diskUsageMaxLimit: Optional[int] = Field(None, ge=0, le=100)

class GlobalConfig(BaseModel):
    toMail: str
    fromMail: str
    smtpServerAdress: str
    smtpServerUser: str
    smtpServerPassword: str
    diskUsageMaxLimit: int = Field(..., ge=0, le=100)

class Config(BaseModel):
    config: GlobalConfig
    disks: List[DiskConfig]

    @model_validator(mode="after")
    def apply_global_disk_limit(self):
        for disk in self.disks:
            if disk.diskUsageMaxLimit is None:  # diskUsageMaxLimit が指定されていない場合
                disk.diskUsageMaxLimit = self.config.diskUsageMaxLimit  # グローバル値を代入
        return self