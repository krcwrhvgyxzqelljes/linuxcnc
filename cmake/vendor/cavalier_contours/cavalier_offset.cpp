#include "cavalier_offset.h"

cavalier_offset::cavalier_offset()
{

}

gp_Pnt cavalier_offset::rotate_3d(double x_to_rotate,double y_to_rotate, double z_to_rotate, double rotate_degrees_x, double rotate_degrees_y, double rotate_degrees_z){

    double cx=cos(rotate_degrees_x*M_PI/180.0); //cos of rotate_degrees
    double sx=sin(rotate_degrees_x*M_PI/180.0);
    double matrix_x[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                              { 1, 0,  0,  0 },
                              { 0, cx,-sx, 0 },
                              { 0, sx, cx, 0 },
                              { 0, 0,  0,  1 }
                            };

    double cy=cos(rotate_degrees_y*M_PI/180.0); //cos of rotate_degrees
    double sy=sin(rotate_degrees_y*M_PI/180.0);
    double matrix_y[4][4] = {
        { cy, 0, sy, 0 },
        { 0,  1, 0,  0 },
        {-sy, 0, cy, 0 },
        { 0,  0, 0,  1 },
    };

    double cz=cos(rotate_degrees_z*M_PI/180.0); //cos of rotate_degrees
    double sz=sin(rotate_degrees_z*M_PI/180.0);
    double matrix_z[4][4] = {
        { cz,-sz, 0, 0 },
        { sz, cz, 0, 0 },
        { 0,   0, 1, 0 },
        { 0,   0, 0, 1 }
    };

    double vector[4] = {0, 0, 0, 1}; //[x,y,z,w] //w=1 for translate, w=0 for direction
    vector[0]=x_to_rotate;
    vector[1]=y_to_rotate;
    vector[2]=z_to_rotate;

    double x=(matrix_x[0][0]*vector[0])+(matrix_x[0][1]*vector[1])+(matrix_x[0][2]*vector[2])+(matrix_x[0][3]*vector[3]);
    double y=(matrix_x[1][0]*vector[0])+(matrix_x[1][1]*vector[1])+(matrix_x[1][2]*vector[2])+(matrix_x[1][3]*vector[3]);
    double z=(matrix_x[2][0]*vector[0])+(matrix_x[2][1]*vector[1])+(matrix_x[2][2]*vector[2])+(matrix_x[2][3]*vector[3]);
    double w=(matrix_x[3][0]*vector[0])+(matrix_x[3][1]*vector[1])+(matrix_x[3][2]*vector[2])+(matrix_x[3][3]*vector[3]);

    //rotate around y axis, take the result of the x axis rotation above :
    vector[0]=x;
    vector[1]=y;
    vector[2]=z;
    x=(matrix_y[0][0]*vector[0])+(matrix_y[0][1]*vector[1])+(matrix_y[0][2]*vector[2])+(matrix_y[0][3]*vector[3]);
    y=(matrix_y[1][0]*vector[0])+(matrix_y[1][1]*vector[1])+(matrix_y[1][2]*vector[2])+(matrix_y[1][3]*vector[3]);
    z=(matrix_y[2][0]*vector[0])+(matrix_y[2][1]*vector[1])+(matrix_y[2][2]*vector[2])+(matrix_y[2][3]*vector[3]);
    w=(matrix_y[3][0]*vector[0])+(matrix_y[3][1]*vector[1])+(matrix_y[3][2]*vector[2])+(matrix_y[3][3]*vector[3]);

    //rotate around z axis, take the result of the y axis rotation above :
    vector[0]=x;
    vector[1]=y;
    vector[2]=z;
    x=(matrix_z[0][0]*vector[0])+(matrix_z[0][1]*vector[1])+(matrix_z[0][2]*vector[2])+(matrix_z[0][3]*vector[3]);
    y=(matrix_z[1][0]*vector[0])+(matrix_z[1][1]*vector[1])+(matrix_z[1][2]*vector[2])+(matrix_z[1][3]*vector[3]);
    z=(matrix_z[2][0]*vector[0])+(matrix_z[2][1]*vector[1])+(matrix_z[2][2]*vector[2])+(matrix_z[2][3]*vector[3]);
    w=(matrix_z[3][0]*vector[0])+(matrix_z[3][1]*vector[1])+(matrix_z[3][2]*vector[2])+(matrix_z[3][3]*vector[3]);

    //    cout<<" x:"<<x<<endl;
    //    cout<<" y:"<<y<<endl;
    //    cout<<" z:"<<z<<endl;
    //    cout<<" w:"<<w<<endl;

    gp_Pnt result;
    result.SetX(x);
    result.SetY(y);
    result.SetZ(z);
    return result;
}

gp_Pnt cavalier_offset::offset_point_on_line(double xs, double ys, double xe, double ye, double offset_from_xs_ys){
    //  cross calculation
    //    A-----------B------------C
    // (Xa,Ya)     (Xb,Yb)      (Xc,Yc)
    //    AB = sqrt( (Xb - Xa)² + (Yb - Ya)² )
    //    AC = 1000
    //    Xc = Xa + (AC * (Xb - Xa) / AB)
    //    Yc = Ya + (AC * (Yb - Ya) / AB)
    gp_Pnt p;
    double AB=sqrt(pow(xe-xs,2)+pow(ye-ys,2));
    double AC=offset_from_xs_ys;
    p.SetX(xs+(AC*(xe-xs)/AB));
    p.SetY(ys+(AC*(ye-ys)/AB));
    return p;
}

gp_Pnt cavalier_offset::bulge_to_arc_waypoint(gp_Pnt p1, gp_Pnt p2, double bulge) {
    // Calculate the distance between p1 and p2
    double dist = sqrt(pow(p1.X() - p2.X(), 2) + pow(p1.Y() - p2.Y(), 2));

    // Calculate the midpoint between p1 and p2
    gp_Pnt m = {(p1.X() + p2.X()) / 2, (p1.Y() + p2.Y()) / 2, 0};

    // Calculate the height from the midpoint to the arc control point
    double d1 = fabs(bulge) * (0.5 * dist);

    // Normalize p1 with respect to m and rotate it based on the bulge direction
    double p1x_nor = p1.X() - m.X();
    double p1y_nor = p1.Y() - m.Y();
    gp_Pnt p1_rot;

    if (bulge < 0) { // Clockwise arc
        p1_rot = rotate_3d(p1x_nor, p1y_nor, 0, 0, 0, -90);
    } else { // Counterclockwise arc or straight line (positive bulge)
        p1_rot = rotate_3d(p1x_nor, p1y_nor, 0, 0, 0, 90);
    }

    // Bring the rotated p1 back to the original coordinates
    p1_rot.SetX(p1_rot.X() + m.X());
    p1_rot.SetY(p1_rot.Y() + m.Y());

    // Calculate the arc control point based on the offset from the midpoint
    gp_Pnt q = offset_point_on_line(m.X(), m.Y(), p1_rot.X(), p1_rot.Y(), d1);

    return q; // Arc control point
}

double waypoint_to_bulge(gp_Pnt p1, gp_Pnt pw, gp_Pnt p2){
    gp_Pnt pm=draw_primitives().get_line_midpoint(p1,p2);
    double d1=draw_primitives().get_line_lenght(pm,pw);
    double d2=draw_primitives().get_line_lenght(pm,p1);
    return d1/d2;
}

// Function to draw the original polyline
void cavalier_offset::drawOriginalPolyline(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> & context) {
    // Start with the first vertex
    const auto& firstVertex = polyline.vertexes().front();
    gp_Pnt startPoint(firstVertex.x(), firstVertex.y(), 0);

    // Iterate over each subsequent vertex
    for (size_t i = 1; i < polyline.vertexes().size(); ++i) {
        const auto& vertex = polyline.vertexes()[i];
        gp_Pnt endPoint(vertex.x(), vertex.y(), 0);

        // Draw the line segment
        Handle(AIS_Shape) aShape=draw_primitives().draw_3d_line(startPoint,endPoint);
        aShape=draw_primitives().colorize(aShape,color,0);
        context->Display(aShape,false);

        // Update the start point for the next segment
        startPoint = endPoint;
    }
}

// Function to draw only arc segments from a polyline
void cavalier_offset::drawPolylineArcsOnly(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> &context) {
    for (size_t i = 0; i < polyline.vertexes().size(); ++i) {
        const auto& startVertex = polyline.vertexes()[i];
        const auto& endVertex = polyline.vertexes()[(i + 1) % polyline.vertexes().size()];

        // Draw the arc segment only if the bulge is not 0
        if (startVertex.bulge() != 0) {
            gp_Pnt startPoint(startVertex.x(), startVertex.y(), 0);
            gp_Pnt endPoint(endVertex.x(), endVertex.y(), 0);

            gp_Pnt waypoint= bulge_to_arc_waypoint(startPoint,endPoint,startVertex.bulge());

            // Draw the arc using the waypoint, start, and end Point
            Handle(AIS_Shape) aShape=draw_primitives().draw_3p_3d_arc(startPoint,waypoint,endPoint);
            aShape=draw_primitives().colorize(aShape,color,0);
            context->Display(aShape,false);
        }
    }
}

// Function to draw only straight line segments from a polyline
void cavalier_offset::drawPolylineLinesOnly(const cavc::Polyline<double>& polyline, Quantity_Color color , opencascade::handle<AIS_InteractiveContext> & context) {
    for (size_t i = 0; i < polyline.vertexes().size()-1; ++i) {
        const auto& startVertex = polyline.vertexes()[i];
        const auto& endVertex = polyline.vertexes()[(i + 1) % polyline.vertexes().size()];

        // Draw the line segment only if the bulge is 0
        if (startVertex.bulge() == 0) {
            gp_Pnt startPoint(startVertex.x(), startVertex.y(), 0);
            gp_Pnt endPoint(endVertex.x(), endVertex.y(), 0);

            // Draw the line
            Handle(AIS_Shape) aShape=draw_primitives().draw_3d_line(startPoint, endPoint);
            aShape=draw_primitives().colorize(aShape,color,0);
            context->Display(aShape,false);
        }
    }
}

// Function to draw both line and arc segments from a polyline
void cavalier_offset::drawPolyline(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> & context) {
    for (size_t i = 0; i < polyline.vertexes().size()-1; ++i) { // -1 for open contour.
        const auto& startVertex = polyline.vertexes()[i];
        const auto& endVertex = polyline.vertexes()[(i + 1) % polyline.vertexes().size()];

        gp_Pnt startPoint(startVertex.x(), startVertex.y(), 0);
        gp_Pnt endPoint(endVertex.x(), endVertex.y(), 0);

        if (startVertex.bulge() == 0) {
            // Draw the line segment
            Handle(AIS_Shape) aShape = draw_primitives().draw_3d_line(startPoint, endPoint);
            aShape = draw_primitives().colorize(aShape, color, 0);
            context->Display(aShape,false);
        } else {
            // Draw the arc segment
            gp_Pnt waypoint = bulge_to_arc_waypoint(startPoint, endPoint, startVertex.bulge());
            Handle(AIS_Shape) aShape = draw_primitives().draw_3p_3d_arc(startPoint, waypoint, endPoint);
            aShape = draw_primitives().colorize(aShape, color, 0);
            context->Display(aShape,false);
        }
    }
}

void cavalier_offset::run_offset_example(opencascade::handle<AIS_InteractiveContext> & context){
    // Input polyline
    cavc::Polyline<double> input;
    // Add vertices as (x, y, bulge)
    input.addVertex(0, 0, 0);
    input.addVertex(0, 50, 0);
    input.addVertex(50, 50, 0);
    input.isClosed() = false;
    double offset=3;

    // Draw the original polyline
    // drawOriginalPolyline(input,Quantity_NOC_GRAY32);

    // Outside offset:
    std::vector<cavc::Polyline<double>> outside = cavc::parallelOffset(input, offset);
    // Draw both line and arc segments from each resulting polyline
    for (const auto& polyline : outside) {
        // drawPolyline(polyline, Quantity_NOC_BEET);
    }

    // Outside offset back:
    std::vector<cavc::Polyline<double>> outside_back;
    for (const cavc::Polyline<double>& polyline : outside) {
        auto offsetResults = cavc::parallelOffset(polyline, -offset);
        outside_back.insert(outside_back.end(), offsetResults.begin(), offsetResults.end());
    }

    // Inside offset:
    std::vector<cavc::Polyline<double>> inside = cavc::parallelOffset(input, -offset);
    // Draw both line and arc segments from each resulting polyline
    for (const cavc::Polyline<double>& polyline : inside) {
        // drawPolyline(polyline, Quantity_NOC_CHOCOLATE);
    }

    // Inside offset back:
    std::vector<cavc::Polyline<double>> inside_back;
    for (const cavc::Polyline<double>& polyline : inside) {
        auto offsetResults = cavc::parallelOffset(polyline, offset);
        inside_back.insert(inside_back.end(), offsetResults.begin(), offsetResults.end());
    }

    // If you want to print the total number of vertices across all polylines
    std::size_t totalVertices_inside= 0;
    for (const cavc::Polyline<double>& polyline : inside_back) {
        totalVertices_inside += polyline.vertexes().size();
    }
    totalVertices_inside--;
    // std::cout << "Total number of vertices in all polylines inside back: " << totalVertices_inside << std::endl;

    std::size_t totalVertices_outside = 0;
    for (const cavc::Polyline<double>& polyline : outside_back) {
        totalVertices_outside += polyline.vertexes().size();
    }
    totalVertices_outside--;
    // std::cout << "Total number of vertices in all polylines outside back: " << totalVertices_outside << std::endl;

    if(totalVertices_outside>totalVertices_inside){
        // Draw the new offset polylines
        for (const cavc::Polyline<double>& polyline : outside_back) {
            drawPolyline(polyline, Quantity_NOC_CYAN3, context); // Use any desired color for the new offset
        }
    } else {
        // Draw the new offset polylines
        for (const cavc::Polyline<double>&polyline : inside_back) {
            drawPolyline(polyline, Quantity_NOC_BLUE1, context); // Use any desired color for the new offset
        }
    }
}
