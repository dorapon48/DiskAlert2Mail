from typing import List
from model import DiskConfig, DiskUseage, MailBody, GlobalConfig

from read_toml import read_toml
from get_df import get_df
from send_mail import send_mail


def create_mail_body_text(mail_body_list: List[MailBody], gconf: GlobalConfig):
    """
    メールの本文を作成する
    """
    msg = "Disk Alert To Mail\n"
    msg += "====サーバー情報====\n"
    msg += "サーバー名: " + gconf.serverName + "\n"
    msg += "サーバーIP: " + gconf.serverIP + "\n"
    msg += "\n"
    msg += "====お知らせ====\n"
    msg += "以下のディスクの使用率が設定された値を上回りました．\n"
    msg += "\n"
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

def main():
    """
    main
    """
    config = read_toml()
    df = get_df()

    if (config == None):
        print("An error occurred while loading d2m.toml")
        return 0

    # 範囲外のディスクを抜き出す
    overd_disks: List[MailBody] = []
    for d in config.disks:
        df_index = check_disk(d, df)
        if df_index != -1:
            overd_disks.append(MailBody(config=d, df=df[df_index]))
    # 範囲外のディスクがないなら終了
    if (overd_disks == []):
        return 0
    
    msg = create_mail_body_text(overd_disks, config.config)
    print(msg)
    send_mail(config.mail, msg, "Disk Alert to Mail")
    return 0


if __name__ == "__main__":
    main()
        
    

    

