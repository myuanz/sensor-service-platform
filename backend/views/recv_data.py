from flask import Blueprint, request, jsonify
from app import mongo
from flask_pymongo.wrappers import Database
from datetime import datetime

db: Database = mongo.db
view = Blueprint("recv_data", __name__)


@view.route("/", methods = ['POST'])
def recv_data():
    assert request.json != None
    recv: dict = request.json
    ProtocolVersion = recv.get('ProtocolVersion')
    if not (ProtocolVersion and ProtocolVersion >= 2):
        return jsonify({'ErrorCode': 1, 'Data': '不支持的版本'})
    try:

        record = {
            'device_id': recv['DeviceID'],
            'CreateTime': recv.get('CreateTime', datetime.now()),
            'ProjectID': recv['ProjectID'],
            'Data': recv['Data'],
        }
    except Exception as e:
        return jsonify({'ErrorCode': 1, 'Data': str(e)})
    else:
        db.data_record.insert(record)
        return jsonify({'ErrorCode': 0, 'Data': datetime.now()})