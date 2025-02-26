from typing import List
from model import DiskConfig, DiskUseage, MailBody

from read_toml import read_toml
from get_df import get_df


def create_mail_body_text(mail_body_list: List[MailBody]):
    """
    メールの本文を作成する
    """
    msg = """
Disk Alert To Mail
以下のディスクの使用率が設定された値を上回りました．

"""
    for m in mail_body_list:
        c = m.config
        d = m.df
        if (c.mountPath == ""): mount_p = "/"
        else : mount_p = c.mountPath
        re_amount = round((d.available - d.used) / 1024 ** 2, 2)
        msg += "ディスク名: " + c.diskName + "\n"
        msg += "マウント先: " + mount_p + "\n"
        msg += "設定使用率: " + str(c.diskUsageMaxLimit) + "\n"
        msg += "使用率　　: " + "{:.2f}".format((d.used / d.available) * 100) + "\n"
        msg += "残り容量　: " + str(re_amount) + "GB" + "\n"
        msg += "\n"
    return msg
        

def check_disk(disk: DiskConfig, df_list: List[DiskUseage]):
    """
    ディスクの判定をする
    diskUsageMaxLimitを越えているならindexが返る
    範囲内ならば-1
    """
    for index, df in enumerate(df_list):
        if (df.mounted_on == "/mnt" + disk.mountPath):
            useage = (df.used / df.available) * 100
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
            overd_disks.append(MailBody(config=d, df=df[df_index]))
    msg = create_mail_body_text(overd_disks)
    print(msg)

    

