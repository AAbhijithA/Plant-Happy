from sklearn.ensemble import RandomForestClassifier
import pickle
import time
import cv2
import urllib.request
import numpy as np
import requests
from dotenv import load_dotenv
import os

load_dotenv()

api_key = os.getenv("write_api_key")

loaded_model = None
with open('RF_Model_Trained.pkl', 'rb') as f:
    loaded_model = pickle.load(f)

link_for_image = "Enter the url from the ESP32_Program for the camera image"

while(True):
    rtimg = urllib.request.urlopen(link_for_image)
    rtimgnp = np.array(bytearray(rtimg.read()),dtype=np.uint8)
    rtimgFINAL = cv2.imdecode(rtimgnp,-1)
    rtimgFINAL = cv2.cvtColor(rtimgFINAL, cv2.COLOR_BGR2GRAY)
    resizeIMG = cv2.resize(rtimgFINAL,(224,224), interpolation = cv2.INTER_LINEAR)
    flattened_img = resizeIMG.flatten()
    predict_img = [flattened_img]
    ans = loaded_model.predict(predict_img)
    give = None
    if ans[0] == "cordana":
        give = 1
    elif ans[0] == "pestalotiopsis":
        give = 2
    elif ans[0] == "sigatoka":
        give = 3
    else:
        give = 4
    print(give)
    print(ans)
    requests.get('https://api.thingspeak.com/update?api_key='+ api_key +'&field4='+str(give))
    time.sleep(5)



