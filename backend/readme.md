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
