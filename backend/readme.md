在config.py中添加
```Python
MONGO_URI = "mongodb://ip:port/sensor"
LASTED_PROTOCOLVERSION = 2
```
ip, port修改为目标mongodb的配置.  
之后  
```Python
python main.py
```
即可运行