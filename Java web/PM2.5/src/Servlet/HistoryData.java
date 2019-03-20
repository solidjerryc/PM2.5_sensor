package Servlet;

import com.Database;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

@WebServlet(name = "HistoryData", urlPatterns = "/historydata")
public class HistoryData extends HttpServlet {
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        String point=request.getParameter("poi");
        String ret="";
        try{
            Database db=new Database();
            ret=db.dbSelectHistoryData(point);
        }catch(Exception e){
            response.getWriter().append("error");
        }
        response.getWriter().append(ret);

    }
}
