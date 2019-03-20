create database air_pollution;
use air_pollution;

CREATE TABLE `poi` (
    `poi_name` varchar(50) DEFAULT NULL,
    `poi_no` char(4) NOT NULL,
    `poi_lng` double DEFAULT NULL,
    `poi_lat` double DEFAULT NULL,
    `poi_alt` int(11) DEFAULT NULL,
    PRIMARY KEY (`poi_no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `data` (
    `data_id` int(11) NOT NULL AUTO_INCREMENT,
    `data_time` datetime DEFAULT NULL,
    `poi_no` varchar(4) DEFAULT NULL,
    `pm25` float DEFAULT NULL,
    `pm10` float DEFAULT NULL,
    `temperature` float DEFAULT NULL,
    `pressure` float DEFAULT NULL,
    `humidity` float DEFAULT NULL,
    PRIMARY KEY (`data_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into poi value('地理科学学院', '001', 119.204516, 26.032325, 20);
insert into poi value('李二楼顶', '002', 119.204461, 26.037912, 20);
insert into poi value('桂一楼顶', '003', 119.201912, 26.038198, 20);
insert into poi value('桃十楼顶', '004', 119.20269, 26.041477, 20);
insert into poi value('李八楼顶', '005', 119.203355, 26.039878, 20);
insert into poi value('桂十一楼顶', '006', 119.202197, 26.035776, 20);
insert into poi value('西区田径场', '007', 119.200349, 26.032107, 20);
insert into poi value('知名楼', '008', 119.203817, 26.02999, 20);
insert into poi value('行政楼', '009', 119.209053, 26.030948, 20);
insert into poi value('立诚楼', '010', 119.204907, 26.027998, 20);
insert into poi value('体育馆', '011', 119.210273, 26.028299, 20);

insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','001',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','002',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','003',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','004',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','005',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','006',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','007',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','008',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','009',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','010',24.5,36.5,13.54,1013.12,60);
insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('2017-05-15 12:15:00.0','011',24.5,36.5,13.54,1013.12,60);