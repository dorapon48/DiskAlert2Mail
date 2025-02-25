import os
import sys

# 相対パスの解決
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from send_mail import send_mail
from read_toml import read_toml


if __name__ == '__main__':
    conf = read_toml()
    bodytext = "test"
    subject = "test"
    send_mail(conf.config, bodytext, subject)


    
    
    