本例程乃使用Arduino UNO外挂ESP8266, 我已封装好一个AT_HTTP.h的头文件, 在项目中引用之即可.  
共有函数:  
```C
void init_AT();
void connect_WiFi(String SSID, String PASS);
void connect_TCP(String Host, String Port);
void close_TCP();
void send_data(String Data);
void send_data_buff(String Data);
String generate_data(String Host, String ProjectID, String DeviceID, String Data, String CreateTime);
```
首先初始化, 之后连接WiFi.  
```C
init_AT();
connect_WiFi(SSID, PASS);
```

发送流程应当如下:   
1. 生成数据. generate_data
2. 打开TCP. connect_TCP
3. 发送数据. send_data
4. 关闭TCP. close_TCP  

```C
data = generate_data(HOST, String(4), String(5), String(millis() * 10), "0");
connect_TCP(HOST, String(PORT));
send_data(data);  
close_TCP();
```

HTTP请求不要求长连接, 发送完数据就可以关闭连接了.  
头文件阻塞等待相关输出, 无需再手动等待标识符.  


(目前一个TCP发送两个包会有bug发生, 所以发送一个包后就需要关闭连接.)