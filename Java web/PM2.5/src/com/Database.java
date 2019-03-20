package com;

import java.sql.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Database {
    //设置系统hosts使得mysql指向服务器ip：39.108.108.117
    private String url="jdbc:mysql://mysql:3306/air_pollution";//数据库连接URL
    private String username="java";//数据库连接用户名
    private String psd="Underground123";//数据库连接密钥

    public Database(){}

    /**
     * Set MySQL connection string.
     * @param url Example:url="jdbc:mysql://mysql:3306/air_pollution"
     * @param username MySQL user name
     * @param psd Password
     */
    public Database(String url,String username,String psd){
        this.url=url;
        this.username=username;
        this.psd=psd;
    }


    /**
     *
     * @return Return true for false indicated if the connection succeeded.
     */
    public boolean dbTestConnect()// throws ClassNotFoundException, SQLException
    {
        try // throws ClassNotFoundException, SQLException
        {
            Class.forName("com.mysql.jdbc.Driver");
            Connection conn = DriverManager.getConnection(url, username, psd);
            if(!conn.isClosed())
                System.out.println("Succeeded connecting to the Database!");
            conn.close();
            return true;
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        } catch (SQLException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
    }

    public String showDatabases()
    {
        String dat="";
        try{
            Class.forName("com.mysql.jdbc.Driver");
            Connection conn = DriverManager.getConnection(url, username, psd);
            Statement stmt=conn.createStatement();
            String sql = "show tables;";
            ResultSet rs = stmt.executeQuery(sql);
            while(rs.next()){
                dat+=rs.getString("Tables_in_air_pollution");
                dat+='\n';
            }
            rs.close();
            conn.close();
            //stmt.execute("SET SQL_SAFE_UPDATES = 0");//允许使用update语句。调优可以删除

            conn.close();
        }catch(Exception e)
        {
            System.out.println(e);
        }

        return dat;
    }

    /**
     * 对静止POI插入数据
     * @param time 时间字符串，格式 2017-05-15 12:15:00.0
     * @param poiNo poi点 格式 003
     * @param pm25 PM2.5数值
     * @param pm10 PM10数值
     * @param temperature 气温
     * @param pressure 气压
     * @param humidity 湿度
     */
    public void poiInsert(String time,String poiNo,String pm25,String pm10,String temperature,String pressure,String humidity){
        try{
            Class.forName("com.mysql.jdbc.Driver");
            Connection conn = DriverManager.getConnection(url, username, psd);
            Statement stmt=conn.createStatement();
            String sql;
            sql=String.format("insert into `data`(data_time,poi_no,pm25,pm10,temperature,pressure,humidity) values('%s','%s',%s,%s,%s,%s,%s);",time,poiNo,pm25,pm10,temperature,pressure,humidity);
            stmt.execute(sql);
            //stmt.execute("SET SQL_SAFE_UPDATES = 0");//允许使用update语句。调优可以删除
            conn.close();
            //System.out.println("insert suceeded!");
        }catch(Exception e)
        {
            System.out.println(e);
        }
    }

    /**
     * 返回poi数据，格式为json
     * @return
     */
    public String dbSelectPOI()
    {
        String dat="";
        try{
            Class.forName("com.mysql.jdbc.Driver");
            Connection conn = DriverManager.getConnection(url, username, psd);
            Statement stmt=conn.createStatement();
            String sql = "select * from poi;";
            ResultSet rs = stmt.executeQuery(sql);
            String name="";
            String id="";
            double lng;
            double lat;
            int alt;
            while(rs.next()){
                name = rs.getString("poi_name");
                id = rs.getString("poi_no");
                lng=rs.getDouble("poi_lng");
                lat=rs.getDouble("poi_lat");
                alt=rs.getInt("poi_alt");
                dat+="{ \"type\": \"Feature\", \"properties\": { \"Id\": \""+id+"\", \"Name\": \""+name+"\" }, \"geometry\": { \"type\": \"Point\", \"coordinates\": [ "+lng+", "+lat+" ] } },";
                //dat+=(name + "\t" + id+"\t" + lng+"\t" + lat+"\t" + alt);
                //System.out.println(dat.substring(0, dat.length()-1));
            }
            rs.close();
            conn.close();
            //stmt.execute("SET SQL_SAFE_UPDATES = 0");//允许使用update语句。调优可以删除

            conn.close();
        }catch(Exception e)
        {
            System.out.println(e);
        }

        return dat.substring(0, dat.length()-1);
    }

    /**
     * 返回几个点最新的数据，结果为GeoJson格式，前端可以直接解析
     * @return
     */
    public String dbSelectRecentData()
    {
        String dat="";
        try{
            Class.forName("com.mysql.jdbc.Driver");
            Connection conn = DriverManager.getConnection(url, username, psd);
            Statement stmt=conn.createStatement();
            String sql = "select a.data_id,a.data_time,a.poi_no,a.pm25,a.pm10,a.temperature,a.pressure,a.humidity,poi.poi_name,poi.poi_lng,poi.poi_lat,poi.poi_alt " +
                    "from data a " +
                    "right join ( " +
                    "select poi_no, max(data_time) max_time " +
                    "from data " +
                    "group by data.poi_no) b1 " +
                    "on a.poi_no=b1.poi_no and b1.max_time=a.data_time " +
                    "left join poi " +
                    "on poi.poi_no=b1.poi_no" +
                    ";";
            ResultSet rs = stmt.executeQuery(sql);
            String name="";
            String id="";
            double lng;
            double lat;
            String date="";
            double pm25,pm10,pressure,temperature,humidity;
            //int alt;
            while(rs.next()){
                name = rs.getString("poi_name");
                id = rs.getString("poi_no");
                lng=rs.getDouble("poi_lng");
                lat=rs.getDouble("poi_lat");
                //alt=rs.getInt("poi_alt");
                date=rs.getString("data_time");
                pm25=rs.getDouble("pm25");
                pm10=rs.getDouble("pm10");
                temperature=rs.getDouble("temperature");
                pressure=rs.getDouble("pressure");
                humidity=rs.getDouble("humidity");
                dat+="{ \"type\": \"Feature\", \"properties\": { \"Id\": \""+id+"\", \"Name\": \""+name+"\",\""+"date\":\""+date+"\",\"pm25\":\""+pm25+"\",\"pm10\":\""+pm10+"\",\"temperature\":\""+temperature+"\",\"pressure\":\""+pressure+"\",\"humidity\":\""+humidity+"\" }, \"geometry\": { \"type\": \"Point\", \"coordinates\": [ "+lng+", "+lat+" ] } },";
                //dat+=(name + "\t" + id+"\t" + lng+"\t" + lat+"\t" + alt);
                //System.out.println(dat.substring(0, dat.length()-1));
            }
            rs.close();
            conn.close();
            //stmt.execute("SET SQL_SAFE_UPDATES = 0");//允许使用update语句。调优可以删除

            conn.close();
        }catch(Exception e)
        {
            System.out.println(e);
        }

        return dat.substring(0, dat.length()-1);
    }

    /**
     *
     * 查询某点的历史数据，结果以字符串形式返回
     * @param point
     * @return
     */
    public String dbSelectHistoryData(String point)
    {
        String date="[";
        String pm25="[";
        String pm10="[";
        String pressure="[";
        String temperature="[";
        String humidity="[";
        DateFormat sdf2 = new SimpleDateFormat("YYYY/MM/d HH:mm");
        try{
            Class.forName("com.mysql.jdbc.Driver");
            Connection conn = DriverManager.getConnection(url, username, psd);
            Statement stmt=conn.createStatement();
            String sql ="select * " +
                    "from( " +
                    "select date_format(data_time,'%Y-%m-%d %H:%i:00') data_time1,truncate(avg(pm25),2) pm25,truncate(avg(pm10),2) pm10,truncate(avg(temperature),2) temperature,truncate(avg(pressure),2) pressure,truncate(avg(humidity),2) humidity " +
                    "from data " +
                    "where data.poi_no='"+point+"'" +
                    "group by data_time1 " +
                    "order by data_time1 desc " +
                    "limit 480) a " +
                    "order by a.data_time1" +
                    ";";
            ResultSet rs = stmt.executeQuery(sql);

            //int alt;
            while(rs.next()){
                //date+=rs.getTimestamp("data_time").toString();
                date+="\""+sdf2.format(rs.getTimestamp("data_time1"))+"\""+',';
                pm25+=rs.getString("pm25")+',';
                pm10+=rs.getString("pm10")+',';
                temperature+=rs.getString("temperature")+',';
                pressure+=rs.getString("pressure")+',';
                humidity+=rs.getString("humidity")+',';
                //System.out.println(date+'\t'+value);
            }
            rs.close();
            conn.close();
            //stmt.execute("SET SQL_SAFE_UPDATES = 0");//允许使用update语句。调优可以删除

            conn.close();
        }catch(Exception e)
        {
            System.out.println(e);
        }
        return date.substring(0, date.length()-1)+"];"
                +pm25.substring(0, pm25.length()-1)+"];"
                +pm10.substring(0, pm10.length()-1)+"];"
                +pressure.substring(0, pressure.length()-1)+"];"
                +temperature.substring(0, temperature.length()-1)+"];"
                +humidity.substring(0, humidity.length()-1)+"]";
    }

    /**
     * 主函数，测试
     * @param args
     */
    public static void main(String[] args) {
        Database db=new Database();
        db.dbTestConnect();

        System.out.println(db.dbSelectHistoryData("001"));
    }

}
