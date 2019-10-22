from flask import Blueprint, request, jsonify
from app import mongo, app
from flask_pymongo.wrappers import Database
from datetime import datetime
import json

db: Database = mongo.db
view = Blueprint("recv_data", __name__)


@view.route("/", methods = ['POST'])
def recv_data():
    # print(request.data, request.headers, request.json)
    if request.json == None:
        return jsonify(request.data)

    recv: dict = request.json
    ProtocolVersion = recv.get('ProtocolVersion')
    if ProtocolVersion not in ProtocolFun:
        return jsonify({'ErrorCode': 1, 'Data': '不支持的版本'})
    try:
        ProtocolFun[ProtocolVersion](recv)
    except Exception as e:
        print(e)
        return jsonify({'ErrorCode': 1, 'Data': str(e)})
    else:
        return jsonify({'ErrorCode': 0, 'Data': datetime.now()})


def parse2(recv):
    CreateTime = recv.get('CreateTime')
    record = {
        'DeviceID': recv['DeviceID'],
        'CreateTime': CreateTime,
        'RecvTime': datetime.now(),
        'ProjectID': recv['ProjectID'],
        'Data': recv['Data'],
    }
    db.data_record.insert(record)
    return record

def parse3(recv):
    CreateTime = recv.get('CreateTime')
    projects = recv.get('ProjectID')
    data = recv.get('Data')

    if len(projects) != len(data):
        raise Exception("Project length is not equal to Data length")
    records = []

    for projectID, datum in zip(projects, data):
        record = {
            'DeviceID': recv['DeviceID'],
            'CreateTime': CreateTime,
            'RecvTime': datetime.now(),
            'ProjectID': projectID,
            'Data': datum,
        }

        records.append(record)
        print(datum)
    db.data_record.insert_many(records)
    return records


ProtocolFun = {
    2: parse2,
    3: parse3,
}
