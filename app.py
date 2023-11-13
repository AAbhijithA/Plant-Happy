from flask import Flask, render_template, url_for
import requests
from dotenv import load_dotenv
import os

load_dotenv()

app = Flask(__name__,template_folder='webapp_page')
api_key = os.getenv("read_api_key")
channel_id = os.getenv("channel_id")

@app.route('/')
def home():
    global api_key
    global channel_id
    temperature = requests.get('https://api.thingspeak.com/channels/'+ channel_id +'/fields/1/last.json?api_key='+ api_key +'&results=1').json()
    humidity = requests.get('https://api.thingspeak.com/channels/'+ channel_id +'/fields/2/last.json?api_key='+ api_key +'&results=1').json()
    moisture = requests.get('https://api.thingspeak.com/channels/'+ channel_id +'/fields/3/last.json?api_key='+ api_key +'&results=1').json()
    disease = requests.get('https://api.thingspeak.com/channels/'+ channel_id +'/fields/4/last.json?api_key='+ api_key +'&results=1').json()
    t = temperature['field1']
    h = humidity['field2']
    m = moisture['field3']
    ta = int(float(t))
    ha = int(float(h))
    ma = int(float(m))
    return render_template('home.html',temperature=ta,humidity=ha,moisture=ma,disease=disease['field4'])

@app.route('/guide')
def guide():
    return render_template('guide.html')

if __name__=="__main__":
    app.run(debug=True)

