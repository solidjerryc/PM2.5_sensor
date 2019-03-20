package Servlet;

import com.Database;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

@WebServlet(name = "GetData", urlPatterns = {"/test"})
public class GetData extends HttpServlet {
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doGet(request,response);
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        Database db = new Database();
        if( db.dbTestConnect()) {
            response.setContentType("text/html");
            PrintWriter out = response.getWriter();
            out.println("Connect succeeded!");
            out.println(db.showDatabases());
        }
    }
}
