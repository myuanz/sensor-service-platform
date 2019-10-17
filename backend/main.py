from app import app, mongo
from flask import url_for

from views import recv_data

from gevent.pywsgi import WSGIServer
# from gevent import monkey
# monkey.patch_all()

app.register_blueprint(recv_data.view)


if __name__ == '__main__':
    # app.run(host="0.0.0.0", debug=True)
    http_server = WSGIServer(('0.0.0.0', 9996), app)
    http_server.serve_forever()
