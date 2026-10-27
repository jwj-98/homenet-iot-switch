from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for,Flask
)
from flask_mqtt import Mqtt
import os


from flaskr.auth import login_required
from flaskr.db import get_db

bp = Blueprint('main', __name__)


app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = os.environ.get('MQTT_BROKER_URL', '127.0.0.1')
app.config['MQTT_BROKER_PORT'] = int(os.environ.get('MQTT_BROKER_PORT', 1883))
app.config['MQTT_USERNAME'] = os.environ.get('MQTT_USERNAME', '')
app.config['MQTT_PASSWORD'] = os.environ.get('MQTT_PASSWORD', '')
app.config['MQTT_REFRESH_TIME'] = 1.0  # refresh time in seconds
mqtt = Mqtt(app)


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected OK")
    else:
        print("Bad connection Returned code=", rc)


def on_disconnect(client, userdata, flags, rc=0):
    print(str(rc))


# 메인페이지
@bp.route('/')
@login_required
def index():
    return redirect(url_for('main.control'))


# forbidden
@bp.route('/forbidden')
@login_required
def forbidden():
    return '권한이 없습니다.'


# iot 제어 페이지
@bp.route('/control', methods=('GET', 'POST'))
@login_required
def control():
    db = get_db()
    devices = db.execute(
        'SELECT mac, type, r1, r2, r3'
        ' FROM switch'
        ' WHERE username = ?', (session.get('username'),)
    ).fetchall()

    if request.method == 'GET':
        mac = request.args.get('mac')
        num = request.args.get('btnNum')
        if mac is not None:
            mqtt.publish('topic', 'mac=' + mac + '&btnNum=' + num)
            return redirect(url_for('main.control'))

    return render_template('main/control.html', title='IOT 제어', profile=g.user, list=True, devices=devices)


# 공지사항
@bp.route('/notice')
@login_required
def notice():
    return render_template('main/notice.html')


# 메뉴얼
@bp.route('/manual')
def manual():
    return render_template('main/ manual.html')


@bp.route('/java', methods=('GET', 'POST'))
def controlJava():
    if request.method == 'POST':
        username = request.form['username']
        db = get_db()
        devices = db.execute(
            'SELECT mac, type, r1, r2, r3'
            ' FROM switch'
            ' WHERE username = ?', (username,)
        ).fetchone()

    return devices[0] #+ '&' + devices[1] + '&' + devices[2] + '&' + devices[3] + '&' + devices[4]

