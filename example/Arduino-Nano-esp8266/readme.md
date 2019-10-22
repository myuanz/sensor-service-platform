本例程乃使用Arduino UNO外挂ESP8266, 我已封装好一个AT_HTTP.h的头文件, 在项目中引用之即可.  
共有函数:  
```C
void init_AT();
void connect_WiFi(String SSID, String PASS);
void connect_TCP(String Host, String Port);
void close_TCP();
void send_data(String Data);
void send_data_buff(String Data);
String generate_data_2(String Host, String ProjectID, String DeviceID, String Data, String CreateTime);
String generate_data_3(String Host, String DeviceID, Vector<int> ProjectID, Vector<T> Data, String CreateTime);
```
首先初始化, 之后连接WiFi.  
```C++
init_AT();
connect_WiFi(SSID, PASS);
```

发送流程应当如下:   
1. 生成数据. generate_data_n
2. 打开TCP. connect_TCP
3. 发送数据. send_data
4. 关闭TCP. close_TCP  

第二版: 
```C++
data = generate_data_2(HOST, String(4), String(5), String(millis() * 10), "0");
connect_TCP(HOST, String(PORT));
send_data(data);  
close_TCP();
```

🎉第三版:
支持同时发送多个项目ID及其数据.
```C++
String data_array[4] = {Lig, temp, pre, alt}; // Here
Vector<String> data_vector(data_array, 4);

int project_array[4] = {4, 5, 6, 7};
Vector<int> project_vector(project_array, 4); // Here

data = generate_data_3(HOST, String(4), project_vector, data_vector, "0"); // 第三版
connect_TCP(HOST, String(PORT));
send_data(data);  
close_TCP();
```


HTTP请求不要求长连接, 发送完数据就可以关闭连接了.  
头文件阻塞等待相关输出, 无需再手动等待标识符.  


(目前一个TCP发送两个包会有bug发生, 所以发送一个包后就需要关闭连接.)