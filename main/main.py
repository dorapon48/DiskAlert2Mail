from typing import List
from model import DiskConfig, DiskUseage, MailBody

from read_toml import read_toml
from get_df import get_df


def check_disk(disk: DiskConfig, df_list: List[DiskUseage]):
    """
    ディスクの判定をする
    diskUsageMaxLimitを越えているならindexが返る
    範囲内ならば-1
    """
    for index, df in enumerate(df_list):
        if (df.mounted_on == "/mnt" + disk.mountPath):
            useage = df.used / df.available
            if (useage >= disk.diskUsageMaxLimit):
                return index
            else:
                return -1
    print("waring: [" + disk.mountPath + "] disk does not exist")
    return -1

if __name__ == "__main__":
    config = read_toml()
    df = get_df()

    # 範囲外のディスクを抜き出す
    overd_disks: List[MailBody] = []
    for d in config.disks:
        df_index = check_disk(d, df)
        if df_index != -1:
            overd_disks.append(MailBody(config, df[df_index]))

    

