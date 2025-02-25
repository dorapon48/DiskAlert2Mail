import smtplib
from email.mime.text import MIMEText
from email.utils import formatdate
from model import MailConfig

def send_mail(config: MailConfig, bodyText: str, subject: str):
    # メールをMIME形式で作成
    msg = MIMEText(bodyText)
    msg['Subject'] = subject
    msg['From'] = config.fromAddress
    msg['To'] = config.toAddress
    msg['Date'] = formatdate()

    # 接続
    connection = smtplib.SMTP(config.smtpAddress, config.smtpPort)
    connection.set_debuglevel(True) # enable debug output
    connection.starttls()
    connection.login(config.smtpUser, config.smtpPassword)
    # 送信
    connection.sendmail(config.fromAddress, config.toAddress, msg.as_string())
    connection.close()