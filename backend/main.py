from app import app, mongo
from flask import url_for

from views import recv_data
app.register_blueprint(recv_data.view)


if __name__ == '__main__':
    app.run(host="0.0.0.0", debug=True)
