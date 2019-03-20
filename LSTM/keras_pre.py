# -*- coding: utf-8 -*-
"""
Created on Thu Apr 19 20:29:06 2018

@author: pro3
"""

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import pymysql
import pandas as pd
import numpy as np
from keras.models import load_model
import time

try:
    sql='''select * from(select date_format(data_time,'%Y-%m-%d %H:%i:00') data_time1,truncate(avg(pm25),2) pm25,avg(pm10) pm10,avg(temperature) temperature,avg(pressure) pressure,avg(humidity) humidity from data where data.poi_no='001' group by data_time1 order by data_time1 desc limit 240) a order by a.data_time1 ;'''
    
    mysql_cn= pymysql.connect(host='mysql', port=3306,user='java', passwd='Underground123', db='air_pollution')
    df = pd.read_sql(sql, con=mysql_cn)    
    mysql_cn.close()
    
    x_min=np.array([3.10000002,3.8866667 ,13.54999971,1007.179993,17.63000011])
    x_range=np.array([120.51999768,180.71667091,24.84333373,15.906697,72.72333495])
    pm_last=df.iloc[-1,1]
    
    model = load_model('model.h5')
    data2=np.array((df.iloc[:,1:]-x_min)/x_range)
    data2=data2.reshape((1,5*240))
    y_train_predict = model.predict(data2, batch_size=1, verbose=0)
    
    y_pre=y_train_predict*120.51999768+3.10+3*np.random.rand(30)
    a=y_pre[0]
    y_pre=y_pre+(pm_last-y_pre[0,0]+3*np.random.rand()) 

    pd.date_range(df.iloc[-1,0],periods=31,freq='min')[1:]
    
    s='['
    for i in df.iloc[:,0]:
        s+='"'+i+'"'
        s+=','
    for i in pd.date_range(df.iloc[-1,0],periods=31,freq='min')[1:]:
        s+='"'+str(i)+'"'
        s+=','
        
    s=s[:-1]+'];'
    s+='['
    for i in df.iloc[:,1]:
        s+=str(i)
        s+=','
        
    for i in y_pre[0]:
        s+=str(round(i, 2))
        s+=','
        
    s=s[:-1]+']'
    
    with open('/usr/local/tomcat80/webapps/ROOT/data/predict/predict_001','w') as f:
        f.write(s)
    with open('/root/python_script/log_keras','w') as f:
        f.write(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))+'\n')
except Exception as e:
    with open('/root/python_script/log_keras','w') as f:
        f.write(e)
