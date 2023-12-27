from flask import Flask
from flask import request, render_template
import sqlite3
import os
import time

app = Flask(__name__)
PROJECT_ROOT = app.root_path
DATABASE = os.path.join(PROJECT_ROOT, 'morsenet.db')  # The database file for this project
PASSWORD_KEY = "****"


@app.route("/", methods=['POST', 'GET'])
def main():
    if request.method == "POST":
        print(request.json)
        data = request.json
        if data["password"] != PASSWORD_KEY:
            return "Wrong Password", 423, {'ContentType':'application/json'} 

        query = "INSERT INTO data (username, morse, time) VALUES (?, ?, ?)"

        connection = sqlite3.connect(DATABASE)
        cur = connection.cursor()
        cur.execute(query, (data["username"], data["morse"], time.time()))
        connection.commit()
        connection.close()
        
        return "Success", 200, {'ContentType':'application/json'} 
    else:
        connection = sqlite3.connect(DATABASE)
        cur = connection.cursor()
        cur.execute("SELECT * FROM data", ())
        data = cur.fetchall()
        connection.close()

        return render_template('main.html', data=data)
    
@app.template_filter('timesince')
def timesince(dt, default="now"):
    diff = time.time() - dt
    periods = (
        (diff / (3600 * 24 * 7 * 30 * 365), "year", "years"),
        (diff / (3600 * 24 * 7 * 30), "month", "months"),
        (diff / (3600 * 24 * 7), "week", "weeks"),
        (diff / (3600 * 24), "day", "days"),
        (diff / 3600, "hour", "hours"),
        (diff / 60, "minute", "minutes"),
        (diff, "second", "seconds"),
    )
    for period, singular, plural in periods:
        if period >= 1:
            return "%d %s ago" % (period, singular if int(period) == 1 else plural)
    return default