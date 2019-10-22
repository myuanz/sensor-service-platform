#include <Vector.h>
#include <Wire.h>

#define generate_quotation(value) "\"" + String(value) + "\"" 
#define generate_quotation_str(name, value) "\"" + String(name) + "\": " + value
#define generate_quotation_single(name_value) "\"" + String(#name_value) + "\": " + name_value

//#define _nmhe_I(name, version) name##version
//#define _nmhe(name, version) _nmhe_I(name, version)
//#define generate_data(...) _nmhe(##generate_data, PROTOCOLVERSION)##(__VA_ARGS__)

// #define WAIT_KEYWORD(KEYWORD, TIMES)                         \
//     for (int i = 0; i < TIMES && !Serial.find(KEYWORD); i++) \
//         delay(1);                                            \
//     ;

void WAIT_KEYWORD(char *KEYWORD, int TIMES)
{
    for (int i = 0; i < TIMES && !Serial.find(KEYWORD); i++)
    {
        delay(1);
        Serial.print(i);
    }
}

// #define WAIT_OK WAIT_KEYWORD("OK", 1000)
#define WAIT_TIME 20

void WAIT_OK()
{
    WAIT_KEYWORD("OK", WAIT_TIME);
}

void init_AT()
{
    // 初始化AT
    Serial.println("AT");
    WAIT_OK();
}

void connect_WiFi(String SSID, String PASS)
{
    // 连接WIFI
    Serial.println("AT+CWMODE=1"); // set client mode
    WAIT_OK();

    String cmd = "AT+CWJAP=\"";
    cmd += SSID;
    cmd += "\",\"";
    cmd += PASS;
    cmd += "\"";
    Serial.println(cmd);
    WAIT_OK();
}

void connect_TCP(String Host, String Port)
{
    // 开启TCP连接
    Serial.println("AT+CIPSTART=\"TCP\",\"" + Host + "\"," + Port);
    WAIT_OK();
}

void close_TCP()
{
    // 关闭TCP连接
    Serial.println("AT+CIPCLOSE");
    WAIT_OK();
}

void send_data(String Data)
{
    // 发送数据通过TCP, 必须已有一个TCP连接
    Serial.print("AT+CIPSEND=");
    Serial.println(Data.length());
    WAIT_KEYWORD(">", WAIT_TIME);
    Serial.println(Data);
    WAIT_KEYWORD("SEND OK", WAIT_TIME);
    WAIT_KEYWORD("+IPD", WAIT_TIME);
}

void send_data_buff(String Data)
{
    // 发送数据通过TCP, 必须已有一个TCP连接.
    // 数据移交到缓冲区, 无需等待send ok
    Serial.print("AT+CIPSENDBUF=");
    Serial.println(Data.length());
    WAIT_KEYWORD(">", WAIT_TIME);
    Serial.println(Data);
}

String generate_data_2(String Host, String ProjectID, String DeviceID, String Data, String CreateTime)
{
    // 生成第二版本的协议对应的数据
    /*
        POST / HTTP/1.0
        Host: 10.18.52.136
        Content-Type: application/json
        Content-Length: 82

        {
            "CreateTime": "", 
            "ProjectID": 0, 
            "ProtocolVersion": 2, 
            "DeviceID": 2, 
            "Data": 0
        }
    */
    const int ProtocolVersion = 2;
    
    String json = "{";
    json += generate_quotation_single(CreateTime) + ", "; // -> "CreateTime": CreateTime
    json += generate_quotation_single(ProjectID) + ", ";
    json += generate_quotation_single(DeviceID) + ", ";
    json += generate_quotation_single(ProtocolVersion) + ", ";
    json += generate_quotation_single(Data);
    json += "}";

    String data = "";
    data += "POST / HTTP/1.0\n";
    data += "Host: " + Host + "\n";
    data += "Content-Type: application/json\n";
    data += "Content-Length: " + String(json.length()) + "\n";
    data += "\r\n";
    data += json;
    return data;
}

template <typename T>
String generate_data_3(String Host, String DeviceID, Vector<int> ProjectID, Vector<T> Data, String CreateTime)
{
    // 生成第三版本的协议对应的数据, add attay to json;

    const int ProtocolVersion = 3;

    String json = "{";
    json += generate_quotation_single(CreateTime) + ", "; // -> "CreateTime": CreateTime
    
    String temp = generate_quotation("ProjectID") + ": [";
    int size = ProjectID.size();
    for (int i=0; i<size; i++){
        temp += String(ProjectID[i]) + (i==size-1?"":", ");
    }
    temp += "], ";
    json += temp;
    json += generate_quotation_single(DeviceID) + ", ";
    json += generate_quotation_single(ProtocolVersion) + ", ";


    temp = generate_quotation("Data") + ": [";
    for (int i=0; i<size; i++){
        temp += Data[i] + (i==size-1?"":", ");
    }
    temp += "]";
    json += temp;
    json += "}";

    String data = "";
    data += "POST / HTTP/1.0\n";
    data += "Host: " + Host + "\n";
    data += "Content-Type: application/json\n";
    data += "Content-Length: " + String(json.length()) + "\n";
    data += "\r\n";
    data += json;
    return data;
}
