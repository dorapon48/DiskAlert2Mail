import smtplib
import os
import sys

# 相対パスの解決
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from read_toml import read_toml


if __name__ == '__main__':
    config = read_toml().config

    connection = smtplib.SMTP(config.smtpServerAdress, 587)
    connection.set_debuglevel(True) # enable debug output
    connection.starttls()
    connection.login(config.smtpServerUser, config.smtpServerPassword)
    
    connection.sendmail(config.fromMail, config.toMail, "TEST Mail from Python!")
    