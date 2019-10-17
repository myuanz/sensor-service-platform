import network, time
import dht
import machine
import socket
import ujson
import urequests

SSID = "myuan4"
PASSWORD = "506506506"
ADDRESS = "10.18.52.137:9997"

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(SSID, PASSWORD)

d = dht.DHT11(machine.Pin(4))

while True:
    response = urequests.post("http://jsonplaceholder.typicode.com/posts", data = "some dummy content")

