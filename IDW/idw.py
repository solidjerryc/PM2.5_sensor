# -*- coding: utf-8 -*-
"""
Created on Mon Apr  9 17:10:27 2018

@author: Administrator
"""

import pymysql
import pandas as pd
import numpy as np
import matplotlib 
matplotlib.use("Agg") 
import matplotlib.pyplot as plt
import time
    
path='/usr/local/tomcat80/webapps/ROOT/data/idw/'

def idw(point,point_org,p=2):
    p_xy=point_org[:,[0,1]]
    z=point_org[:,2]
    d=[]
    for i in p_xy:
        d.append(((i[0]-point[0])**2+(i[1]-point[1])**2)**0.5)
    d=np.array(d)**(-p)
    w=d/np.sum(d)
    result=np.dot(z,w.T)
    return result

sql="select a.data_id,a.data_time,a.poi_no,a.pm25,a.pm10,a.temperature,a.pressure,a.humidity,poi.poi_name,poi.poi_lng,poi.poi_lat,poi.poi_alt from data a right join (select poi_no, max(data_time) max_time from data group by data.poi_no) b1 on a.poi_no=b1.poi_no and b1.max_time=a.data_time left join poi on poi.poi_no=b1.poi_no;"
mysql_cn= pymysql.connect(host='mysql', port=3306,user='java', passwd='Underground123', db='air_pollution')
df = pd.read_sql(sql, con=mysql_cn)    
mysql_cn.close()


grid_size=0.0001

names=['pm25','pm10','temperature','pressure','humidity']

for index,name in enumerate(names):
    points=np.array(df.iloc[:,[-3,-2,3+index]])
    xmax,xmin,ymax,ymin=max(points[:,0])+0.003,min(points[:,0])-0.003,max(points[:,1])+0.003,min(points[:,1])-0.003
    
    z=[]
    for x in np.arange(xmin,xmax,grid_size):
        for y in np.arange(ymin,ymax,grid_size):
            z.append(idw([x,y],points))
        
    z=np.array(z).reshape(len(np.arange(xmin,xmax,grid_size)),-1)
    z=z.T#np.rot90(z,3)
    
    rasterOrigin = (xmin,ymin)
    
    pixelWidth = grid_size
    pixelHeight = grid_size
    
#    plt.imshow(np.flipud(z))
    plt.imsave(path+name+'.png',np.flipud(z))
    
with open('/root/python_script/log','w') as f:
    f.write(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))+'\n')
