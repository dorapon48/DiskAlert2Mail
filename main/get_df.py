import subprocess
from typing import List

from model import DiskUseage

def get_df():
    """
    dfの結果をDiskUseageのリストで返す
    """
    result = subprocess.run(["df"], capture_output=True, text=True)
    lines = result.stdout.split("\n")
    # 最初の行（ヘッダー）を除外
    disk_usage_info: List[DiskUseage] = []
    for line in lines[1:]:
        parts = line.split()
        if len(parts) < 6:
            continue  # 不完全な行は無視
        disk_usage_info.append(DiskUseage(
            filesystem=parts[0],
            size=parts[1],
            used=parts[2],
            available=parts[3],
            mounted_on=parts[5]
        ))
    
    return disk_usage_info
