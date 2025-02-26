from read_toml import read_toml




def check_disks(disk: DiskConfig, df_list: List[DiskUseage]):
    """
    ディスクの判定をする
    """
    for df in df_list:
        if (df.mounted_on == "/mnt" + disk.mountPath):
            useage = df.used / df.available
            if (useage >= disk.diskUsageMaxLimit):
                return True
    return False