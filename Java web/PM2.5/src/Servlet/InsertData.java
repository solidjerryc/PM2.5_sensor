package Servlet;

import com.Database;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

@WebServlet(name = "InsertData",urlPatterns = "/insertdata")
public class InsertData extends HttpServlet {
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }

    /**
     * request URL sample: http://host:port/path/insertdata?poiNo=001&pm25=21.5&pm10=35.7&temperature=12.43&pressure=1013.4&humidity=61.4
     * @param request
     * @param response
     * @throws ServletException
     * @throws IOException
     */
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String poiNo=request.getParameter("poiNo");
        String pm25=request.getParameter("pm25");
        String pm10=request.getParameter("pm10");
        String temperature=request.getParameter("temperature");
        String pressure=request.getParameter("pressure");
        String humidity=request.getParameter("humidity");
        DateFormat sdf2 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Date date=new Date();
        String time=sdf2.format(date);
        Database db=new Database();
        db.poiInsert(time,poiNo,pm25,pm10,temperature,pressure,humidity);
    }
}
