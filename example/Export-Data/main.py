#%%
import pymongo
client = pymongo.MongoClient("mongodb://10.18.52.137:27018/")
db = client['sensor']
data = db['data_record']
groups = db['data_group']
creators = db['users']
devices = db['devices']


projectID = int(input("输入项目ID: "))
print('-'*20)
#%%
group = groups.find_one({"ID": projectID})
creator = creators.find_one({"STID": group['creator']})
print('项目组描述: ', group.get('description'))
print('项目组创建人: ', creator.get('name'))

text = 'device_id, CreateTime, RecvTime, Data\n'

devices_id = []
for i in data.find({'ProjectID': projectID}):
    device_id = i.get('DeviceID')
    if not device_id:
        device_id = i.get('device_id') # 早些年酿成大错
    if device_id not in devices_id:
        device = devices.find_one({'ID': device_id})

        print('涉及设备ID: ', device_id)
        print('用途: ', device.get('use'))
        print('位置: ', device.get('positon'))
        owner_id = device.get('owner')
        if owner_id:
            owner = creators.find_one({"STID": owner_id})
            print('拥有者: ', owner.get('name'))
        else:
            print('拥有者: ', "未知")
        devices_id.append(device_id)
        print('-'*20)

    text += f"{i.get('device_id')}, {i.get('CreateTime')}, {i.get('RecvTime')}, {i.get('Data')}\n"

with open('output.csv', 'w') as f:
    f.write(text)
