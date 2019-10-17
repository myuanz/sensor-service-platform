在config.py中添加
```Python
MONGO_URI = "mongodb://ip:port/sensor"
LASTED_PROTOCOLVERSION = 2
```
ip, port修改为目标mongodb的配置.  
之后克隆到目标机器, 进入本目录
```bash
docker build -t sensor_service_backend . # 构造镜像
docker run -d -p 9997:9997 sensor_service_backend # 运行容器
```
---
部分数据库结构: 

```

db.users.insert({
    "STID": string,
    "name": string,
    "other": Any
})

db.devices.insert({
    "ID": 1, 
    "owner": "040717130",
    "use": "Arduino MKR1000例程演示, 将会发送自身已运行时间",
    "positon": "此人桌上",
    "note": ""
})

db.devices.insert({
    "ID": 2, 
    "owner": "040717130",
    "use": "ESP32 例程演示, 将会发送从DHT11读取到的温度和湿度",
    "positon": "此人桌上",
    "note": ""
})

db.data_record.insert({
    "device_id" : Number, 
    "CreateTime" : Date, 
    "RecvTime" : Date, 
    "ProjectID" : Number, 
    "Data" : Any
})

db.data_group.insert({
    "ID": 1, 
    "creator": "040717130",
    "description": "设备已启动时间",
})

db.data_group.insert({
    "ID": 2, 
    "creator": "040717130",
    "description": "温度",
})

db.data_group.insert({
    "ID": 3, 
    "creator": "040717130",
    "description": "湿度",
})
```