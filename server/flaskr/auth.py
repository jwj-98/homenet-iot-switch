import functools
from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for
)
from werkzeug.security import check_password_hash, generate_password_hash
from flaskr.db import get_db

bp = Blueprint('auth', __name__, url_prefix='/auth')


# 로그인 여부 확인
def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('auth.login'))
        return view(**kwargs)
    return wrapped_view


# 접속한 사용자 정보
@bp.before_app_request
def load_logged_in_user():
    username = session.get('username')

    # 사용자 정보 없음
    if username is None:
        g.user = None

    # 사용자 정보 있음
    else:
        g.user = get_db().execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()


# /register 요청시 실행 post 요청시 쿼리실행
@bp.route('/register', methods=('GET', 'POST'))
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None

        if not username:
            error = '아이디는 필수입니다.'
        elif not password:
            error = '비밀번호는 필수입니다.'
        elif db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is not None:
            error = '아이디 {} 는 이미 등록되어 있습니다.'.format(username)

        if error is None:
            db.execute(
                'INSERT INTO user (username, password) VALUES (?, ?)',
                (username, generate_password_hash(password))
            )
            db.commit()
            return 'ok'

        flash(error)

    return render_template('auth/register.html')


# /registerSwitch 요청시 실행 post 요청시 쿼리실행: 스위치 기기를 등록합니다.
@bp.route('/registerSwitch', methods=('GET', 'POST'))
def registerSwitch():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        mac = request.form['mac']
        type = request.form['type']
        db = get_db()
        error = None

        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            error = '아이디를 확인해주세요.'
        elif not check_password_hash(user['password'], password):
            error = '비밀번호를 확인해주세요.'
        elif db.execute(
            'SELECT id FROM switch WHERE mac = ?', (mac,)
        ).fetchone() is not None:
            db.execute(
                'DELETE FROM switch WHERE mac = ?', (mac,)
            )
            db.commit()
        if error is None:
            db.execute(
                'INSERT INTO switch (username, mac, type, r1, r2, r3) VALUES (?, ?, ?, ?, ?, ?)',
                (username, mac, type, 0, 0, 0)
            )
            db.commit()
            return 'ok'
    return error


# /registerMoodLight 요청시 실행 post 요청시 쿼리실행: 무드등 기기를 등록합니다.
@bp.route('/registerMoodLight', methods=('GET', 'POST'))
def registerMoodLight():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        mac = request.form['mac']
        r1 = request.form['r1']
        db = get_db()
        error = None

        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            error = '아이디를 확인해주세요.'
        elif not check_password_hash(user['password'], password):
            error = '비밀번호를 확인해주세요.'
        if error is None:
            db.execute(
                'INSERT INTO user (username, mac, r1) VALUES (?, ?, ?)',
                (username, mac, r1)
            )
            db.commit()
            return 'ok'
    return error


@bp.route('/login', methods=('GET', 'POST'))
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            error = '아이디를 확인해주세요.'
        elif not check_password_hash(user['password'], password):
            error = '비밀번호를 확인해주세요.'

        if error is None:
            session.clear()
            session['username'] = user['username']
            return 'ok', redirect(url_for('board.lists'))

        flash(error)

    return render_template('auth/login.html')


@bp.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('index'))
