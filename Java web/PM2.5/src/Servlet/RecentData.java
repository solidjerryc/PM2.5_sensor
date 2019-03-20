package Servlet;

import com.Database;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

@WebServlet(name = "RecentData",urlPatterns = "/recentdata")
public class RecentData extends HttpServlet {
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setContentType("text/json;charset=utf-8");
        response.setCharacterEncoding("UTF-8");
        String aaa="{\"type\": \"FeatureCollection\",\"crs\": { \"type\": \"name\", \"properties\": { \"name\": \"urn:ogc:def:crs:OGC:1.3:CRS84\" } },\"features\": [";
        try{
            Database db=new Database();
            aaa+=db.dbSelectRecentData();
        }catch(Exception e){
            System.out.println(e);
            response.getWriter().append("<h1>error</h1>");
        }
        response.getWriter().append(aaa+"]}");
    }
}
