import network, time
import dht
import machine
import socket
import ujson
import urequests

SSID = "N6-506-ARM"
PASSWORD = "armn6-506qwg"
ADDRESS = "http://10.18.52.137:9997"

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(SSID, PASSWORD)
print(station)

d = dht.DHT11(machine.Pin(4))
d.measure()

while not station.isconnected():
    station.connect(SSID, PASSWORD)
    print('连接中')
    time.sleep(1)
while True:
    json = {
        'ProtocolVersion': 2,
        'DeviceID': 2,
        'CreateTime': '',
        'ProjectID': 0,
        'Data': 0,
    }
    for Data, ProjectID in zip([d.temperature(), d.humidity()], [2, 3]):
        # 数据分别是d.temperature(), d.humidity(), 项目ID分别是2, 3
        
        json['Data'] = Data
        json['ProjectID'] = ProjectID
        try:
            response = urequests.post(ADDRESS, json=json, headers={'Content-Type': 'application/json'})
            # 本来headers是不用写的, 但是我这个版本有点老了, 新版本的已经支持不直接写headers自动添加了.
        except Exception as e:
            print(e)
        else:
            print(response.json())
    time.sleep(5) # 休眠5s
