#include "cavalier_3d.h"

cavalier_3d::cavalier_3d()
{

}

// Function to create a new plane aligned with global XYZ axes and positioned at the origin
gp_Pln cavalier_3d::AlignPlaneToOrigin(const gp_Pln& originalPlane) {
    // Get the coordinate system of the original plane
    gp_Ax3 axis = originalPlane.Position();

    // Create a new coordinate system aligned with the global XYZ axes at the origin
    gp_Ax3 globalAxes(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1), gp_Dir(1, 0, 0));

    // Create a transformation to align the plane with the global axes
    gp_Trsf transformation;
    transformation.SetTransformation(axis, globalAxes);

    // Apply the transformation to the original plane
    gp_Pln alignedPlane = originalPlane;
    alignedPlane.Transform(transformation);

    return alignedPlane;
}

// Function to port points from one plane to another
void cavalier_3d::PortCavLineBetweenPlanes(const gp_Pln& sourcePlane, const gp_Pln& targetPlane, cavline& line) {
    // Get the transformation from sourcePlane to targetPlane
    gp_Trsf transformation;
    transformation.SetTransformation(sourcePlane.Position(), targetPlane.Position());

    // Apply the transformation to the point
    line.p0.Transform(transformation);
    line.p1.Transform(transformation);
    line.pw.Transform(transformation);
    line.pc.Transform(transformation);
}

gp_Pnt cavalier_3d::get_2d_arc_center(gp_Pnt p0, gp_Pnt pw, gp_Pnt p1) {
    gp_Pnt center={0,0,0};

    double x1 = (p0.X() + pw.X()) / 2;
    double y1 = (p0.Y() + pw.Y()) / 2;
    double dy1 = pw.X() - p0.X();
    double dx1 = -(pw.Y() - p0.Y());

    // Get the perpendicular bisector of (x2, y2) and (x3, y3).
    double x2 = (pw.X() + p1.X()) / 2;
    double y2 = (pw.Y() + p1.Y()) / 2;
    double dy2 = p1.X() - pw.X();
    double dx2 = -(p1.Y() - pw.Y());

    double endpoint_x0 = x1 + dx1;
    double endpoint_y0 = y1 + dy1;
    double endpoint_x1 = x2 + dx2;
    double endpoint_y1 = y2 + dy2;

    // line 1
    double delta_y0 = endpoint_y0 - y1;
    double delta_x0 = x1 - endpoint_x0;
    double c0 = delta_y0 * x1 + delta_x0 * y1;
    // line 2
    double delta_y1 = endpoint_y1 - y2;
    double delta_x1 = x2 - endpoint_x1;
    double c1 = delta_y1 * x2 + delta_x1 * y2;

    double determinant = delta_y0 * delta_x1 - delta_y1 * delta_x0;

    if (determinant == 0) // The lines are parallel.
    {
        // lines are parallel
        // std::cout << "Error in arc_center function, class offset.cpp" << std::endl;
    }
    else {
        center.SetX( (delta_x1 * c0 - delta_x0 * c1) / determinant ); // std::cout<<"function offset.ccp, arc center x: "<<center.x<<std::endl;
        center.SetY( (delta_y0 * c1 - delta_y1 * c0) / determinant ); // std::cout<<"function offset.ccp, arc center y: "<<center.y<<std::endl;
        center.SetZ( p0.Z() );
    }
    return center;
}

// Function to calculate the angle between two vectors originating from the center
double cavalier_3d::get_angle_between_vectors(const gp_Pnt& pc, const gp_Pnt& p0, const gp_Pnt& p1) {
    gp_Vec v1(pc, p0);
    gp_Vec v2(pc, p1);
    double dotProduct = v1.Dot(v2);
    double len1 = v1.Magnitude();
    double len2 = v2.Magnitude();
    double cosineAngle = dotProduct / (len1 * len2);
    // Clamp the value to the range [-1, 1] to avoid numerical issues with acos
    cosineAngle = std::min(1.0, std::max(-1.0, cosineAngle));
    return std::acos(cosineAngle);
}

// Main function to calculate arc length
double cavalier_3d::get_2d_arc_length(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1, const gp_Pnt& pc) {
    // Calculate the radius of the circle
    double radius = pc.Distance(p0);

    // Calculate the angle subtended by the arc
    double angleStartToEnd = get_angle_between_vectors(pc, p0, p1);

    // Calculate the angle subtended by the waypoint to ensure the arc is valid
    double angleStartToWaypoint = get_angle_between_vectors(pc, p0, pw);
    double angleWaypointToEnd = get_angle_between_vectors(pc, pw, p1);

    // Ensure the waypoint lies on the arc segment between p0 and p1
    if (angleStartToWaypoint + angleWaypointToEnd > angleStartToEnd + 1e-6) {
        // Adjust the angle for the longer arc segment
        angleStartToEnd = 2 * M_PI - angleStartToEnd;
    }

    // Calculate the arc length
    double arcLength = radius * angleStartToEnd;
    return arcLength;
}

std::vector<cavline> cavalier_3d::convertToCavLines(const std::vector<cavc::Polyline<double>>& outside) {
    std::vector<cavline> result;
    for (const auto& polyline : outside) {
        const auto& vertices = polyline.vertexes();
        cavline line;
        for (size_t i = 1; i <= vertices.size(); ++i) {
            const auto& startVertex = vertices[i - 1];
            const auto& endVertex = vertices[i];
            if (i < vertices.size()) {

                line.p0 = {startVertex.x(), startVertex.y(), 0}; // Assuming Z-coordinate is 0
                line.p1 = {endVertex.x(), endVertex.y(), 0};     // Assuming Z-coordinate is 0
                line.bulge = startVertex.bulge();                // Assuming bulge is constant for each line segment
                line.pw = cavalier_offset().bulge_to_arc_waypoint(line.p0, line.p1, line.bulge);

                if(line.bulge==0){
                    line.lenght=draw_primitives().get_line_lenght(line.p0,line.p1);
                }
                if(line.bulge!=0){
                    line.pc=get_2d_arc_center(line.p0,line.pw,line.p1);
                    line.lenght=get_2d_arc_length(line.p0,line.pw,line.p1,line.pc);
                }
                if(line.bulge==1 || line.bulge==-1){ // Full circle.
                    double radius=line.p0.Distance(line.pc);
                    line.lenght=2*M_PI*radius;
                }
                result.push_back(line);
                // std::cout<<" bulge:"<<line.bulge<<std::endl;
            } else { // Last vertex, handle separately
                // Ensure that the last vertex is treated as a line segment, not a circle
                line.p0 = {startVertex.x(), startVertex.y(), 0}; // Assuming Z-coordinate is 0
                line.p1 = {endVertex.x(), endVertex.y(), 0}; // Assuming Z-coordinate is 0
                line.bulge =  startVertex.bulge();
                line.pw = cavalier_offset().bulge_to_arc_waypoint(line.p0, line.p1, line.bulge);
                if(line.bulge==0){
                    line.lenght=draw_primitives().get_line_lenght(line.p0,line.p1);
                }
                if(line.bulge!=0){
                    line.pc=get_2d_arc_center(line.p0,line.pw,line.p1);
                    line.lenght=get_2d_arc_length(line.p0,line.pw,line.p1,line.pc);
                }
                if(line.bulge==1 || line.bulge==-1){ // Full circle.
                    double radius=line.p0.Distance(line.pc);
                    line.lenght=2*M_PI*radius;
                }
                // std::cout<<"last bulge:"<<line.bulge<<std::endl;
                result.push_back(line);
            }
        }
    }
    return result;
}


// Function to draw both line and arc segments from a polyline
void cavalier_3d::drawPolyline_(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> & context) {
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
            gp_Pnt waypoint = cavalier_offset().bulge_to_arc_waypoint(startPoint, endPoint, startVertex.bulge());
            Handle(AIS_Shape) aShape = draw_primitives().draw_3p_3d_arc(startPoint, waypoint, endPoint);
            aShape = draw_primitives().colorize(aShape, color, 0);
            context->Display(aShape,false);
        }
    }
}

Handle(AIS_Shape) cavalier_3d::cavline_to_shape(cavline line){

    Handle(AIS_Shape) aShape;
    if(draw_primitives().is_point_on_line(line.p0,line.p1,line.pw)){
        // std::cout<<"draw as line"<<std::endl;
        aShape = draw_primitives().draw_3d_line(line.p0,line.p1);

    } else if(line.p0.Distance(line.p1)>0){ // Draw the arc segment
        // std::cout<<"draw as arc"<<std::endl;
        aShape = draw_primitives().draw_3p_3d_arc(line.p0, line.pw, line.p1);
    } else { // Draw as cicle.
        // std::cout<<"draw as circle"<<std::endl;
        double radius=line.pc.Distance(line.p0);
        // std::cout<<"circle radius:"<<radius<<std::endl;
        aShape = draw_primitives().draw_2d_circle(line.pc,radius);
    }
    return aShape;
}

// Function to draw cavline
void cavalier_3d::drawCavLine(const cavline& line, Quantity_Color color, opencascade::handle<AIS_InteractiveContext>& context) {

    if(draw_primitives().is_point_on_line(line.p0,line.p1,line.pw)){
        // std::cout<<"draw as line"<<std::endl;
        Handle(AIS_Shape) aShape = draw_primitives().draw_3d_line(line.p0,line.p1);
        aShape = draw_primitives().colorize(aShape, color, 0);
        context->Display(aShape,false);
    } else if(line.p0.Distance(line.p1)>0){ // Draw the arc segment
        // std::cout<<"draw as arc"<<std::endl;
        Handle(AIS_Shape) aShape = draw_primitives().draw_3p_3d_arc(line.p0, line.pw, line.p1);
        aShape = draw_primitives().colorize(aShape, color, 0);
        context->Display(aShape,false);

        // Draw the center.
        aShape = draw_primitives().draw_3d_point(line.pc);
        aShape = draw_primitives().colorize(aShape, color, 0);
        context->Display(aShape,false);

    } else { // Draw as cicle.
        // std::cout<<"draw as circle"<<std::endl;
        double radius=line.pc.Distance(line.p0);
        // std::cout<<"circle radius:"<<radius<<std::endl;
        Handle(AIS_Shape) aShape = draw_primitives().draw_2d_circle(line.pc,radius);
        aShape = draw_primitives().colorize(aShape, color, 0);
        context->Display(aShape,false);

        // Draw the center.
        aShape = draw_primitives().draw_3d_point(line.pc);
        aShape = draw_primitives().colorize(aShape, color, 0);
        context->Display(aShape,false);
    }
}

// Minimal cavline input : p0,pw.p1.
// If its a circle, original input is returned.
std::vector<cavline> cavalier_3d::get_2d_fillet_pathvec(std::vector<cavline> cvec, double radius){

    radius=abs(radius);
    if(radius==0){ // No offset.
        return cvec;
    }

    for(uint i=0; i<cvec.size(); i++){
        cvec[i].bulge=draw_primitives().waypoint_to_bulge(cvec[i].p0, cvec[i].pw, cvec[i].p1);
    }

    // Run the cavaliercode at the zero plane.
    cavc::Polyline<double> input;
    // input.addVertex(l0.p0.X(), l0.p0.Y(), l0.bulge);
    // input.addVertex(l1.p0.X(), l1.p0.Y(), l1.bulge);
    // input.addVertex(l1.p1.X(), l1.p1.Y(), 0);

    for(uint i=0; i<cvec.size(); i++){

        if(cvec[i].p0.Distance(cvec[i].p1)==0){ // It's a full circle.
            return cvec;
        }

        input.addVertex(cvec[i].p0.X(), cvec[i].p0.Y(), cvec[i].bulge);
        if(i==cvec.size()-1){
            input.addVertex(cvec[i].p1.X(), cvec[i].p1.Y(), 0);
        }
    }
    input.isClosed() = false;


    std::vector<cavc::Polyline<double>> outside = cavc::parallelOffset(input, radius);
    std::vector<cavline> cavvec_outside_back;
    std::vector<cavc::Polyline<double>> outside_back ;
    if(outside.size()>0){
        outside_back = cavc::parallelOffset(outside[0], -radius);
        cavvec_outside_back = convertToCavLines(outside_back);
    }

    std::vector<cavc::Polyline<double>> inside = cavc::parallelOffset(input, -radius);
    std::vector<cavline> cavvec_inside_back;
    std::vector<cavc::Polyline<double>> inside_back;
    if(inside.size()>0){
        inside_back = cavc::parallelOffset(inside[0], radius);
        cavvec_inside_back = convertToCavLines(inside_back);
    }

    if(outside.size()>0 || inside.size()>0){
        // Compare the vector results of offset back.
        if(outside_back.size()>=inside_back.size()){
            // for (size_t i = 0; i < cavvec_outside_back.size() - 1; ++i) {
            //     drawCavLine(cavvec_outside_back[i], Quantity_NOC_PURPLE, context);
            // }
            cavvec_outside_back.pop_back();
            return cavvec_outside_back;
        } else {
            // for (size_t i = 0; i < cavvec_inside_back.size() - 1; ++i) {
            //     drawCavLine(cavvec_inside_back[i], Quantity_NOC_BLUE, context);
            // }
            cavvec_inside_back.pop_back();
            return cavvec_inside_back;
        }
    }
    // Return original input as a vector.
    return cvec;
}


// Minimal cavline input : p0,pw.p1.
// If its a circle, original input is returned.
std::vector<cavline> cavalier_3d::get_2d_fillet_path_2_segments(cavline l0, cavline l1, double radius){

    radius=abs(radius);

    l0.bulge = draw_primitives().waypoint_to_bulge(l0.p0, l0.pw, l0.p1);
    l1.bulge = draw_primitives().waypoint_to_bulge(l1.p0, l1.pw, l1.p1);

    // drawCavLine(l0, Quantity_NOC_YELLOW, context);
    // drawCavLine(l1, Quantity_NOC_YELLOW, context);

    // Run the cavaliercode at the zero plane.
    cavc::Polyline<double> input;
    input.addVertex(l0.p0.X(), l0.p0.Y(), l0.bulge);
    input.addVertex(l1.p0.X(), l1.p0.Y(), l1.bulge);
    input.addVertex(l1.p1.X(), l1.p1.Y(), 0);
    input.isClosed() = false;

    if(radius==0){ // No offset.
        return {l0,l1};
    }
    if(l0.p0.Distance(l0.p1)==0){ // It's a full circle.
        return {l0,l1};
    }
    if(l1.p0.Distance(l1.p1)==0){ // It's a full circle.
        return {l0,l1};
    }

    std::vector<cavc::Polyline<double>> outside = cavc::parallelOffset(input, radius);
    std::vector<cavline> cavvec_outside_back;
    std::vector<cavc::Polyline<double>> outside_back ;
    if(outside.size()>0){
        outside_back = cavc::parallelOffset(outside[0], -radius);
        cavvec_outside_back = convertToCavLines(outside_back);
    }

    std::vector<cavc::Polyline<double>> inside = cavc::parallelOffset(input, -radius);
    std::vector<cavline> cavvec_inside_back;
    std::vector<cavc::Polyline<double>> inside_back;
    if(inside.size()>0){
        inside_back = cavc::parallelOffset(inside[0], radius);
        cavvec_inside_back = convertToCavLines(inside_back);
    }

    if(outside.size()>0 || inside.size()>0){
        // Compare the vector results of offset back.
        if(outside_back.size()>=inside_back.size()){
            // for (size_t i = 0; i < cavvec_outside_back.size() - 1; ++i) {
            //     drawCavLine(cavvec_outside_back[i], Quantity_NOC_PURPLE, context);
            // }
            cavvec_outside_back.pop_back();
            return cavvec_outside_back;
        } else {
            // for (size_t i = 0; i < cavvec_inside_back.size() - 1; ++i) {
            //     drawCavLine(cavvec_inside_back[i], Quantity_NOC_BLUE, context);
            // }
            cavvec_inside_back.pop_back();
            return cavvec_inside_back;
        }
    }
    // Return original input as a vector.
    return {l0,l1};
}

// Minimal cavline input : p0,pw.p1
int cavalier_3d::get_3d_fillet_pathvec(std::vector<cavline> cavvec, double radius, std::vector<cavline> &result_vec) {

//    if(l0.p0.Distance(l0.p1)<1e-6){ // Is a circle.
//        return 0;
//    }
//    if(l1.p0.Distance(l1.p1)<1e-6){ // Is a circle.
//        return 0;
//    }

//    // Create the original plane
//    gp_Pln originalPlane;

//    int r=draw_primitives().create_3d_plane(l0.p0, l0.p1, l1.pw, originalPlane);
//    if(!r){
//        std::cerr << "Can not create plane, segments are colinair, aborting fillet." << std::endl;
//        return 0;
//    }


//    // Align plane to the origin
//    gp_Pln zeroPlane = AlignPlaneToOrigin(originalPlane);

//    // Port points from the original plane to the zero plane
//    PortCavLineBetweenPlanes(originalPlane, zeroPlane, l0);
//    PortCavLineBetweenPlanes(originalPlane, zeroPlane, l1);

//    // Attempt to get the 2D fillet path
//    result_vec = get_2d_fillet_path_2_segments(l0, l1, radius);

//    // Port back the lines of result_vec to the original plane
//    for (uint i = 0; i < result_vec.size(); i++) {
//        PortCavLineBetweenPlanes(zeroPlane, originalPlane, result_vec[i]);
//    }

//    return 1;
    return 0;
}

// Minimal cavline input : p0,pw.p1
int cavalier_3d::get_3d_fillet_path_2_segments(cavline l0, cavline l1, double radius, std::vector<cavline> &result_vec) {

    if(l0.p0.Distance(l0.p1)<1e-6){ // Is a circle.
        return 0;
    }
    if(l1.p0.Distance(l1.p1)<1e-6){ // Is a circle.
        return 0;
    }

    // Create the original plane
    gp_Pln originalPlane;

    int r=draw_primitives().create_3d_plane(l0.p0, l0.p1, l1.pw, originalPlane);
    if(!r){
        std::cerr << "Can not create plane, segments are colinair, aborting fillet." << std::endl;
        return 0;
    }


    // Align plane to the origin
    gp_Pln zeroPlane = AlignPlaneToOrigin(originalPlane);

    // Port points from the original plane to the zero plane
    PortCavLineBetweenPlanes(originalPlane, zeroPlane, l0);
    PortCavLineBetweenPlanes(originalPlane, zeroPlane, l1);

    // Attempt to get the 2D fillet path
    result_vec = get_2d_fillet_path_2_segments(l0, l1, radius);

    // Port back the lines of result_vec to the original plane
    for (uint i = 0; i < result_vec.size(); i++) {
        PortCavLineBetweenPlanes(zeroPlane, originalPlane, result_vec[i]);
    }

    return 1;
}

void cavalier_3d::run_2d_example(opencascade::handle<AIS_InteractiveContext> & context, double offset) {

    cavline l0;
    l0.p0 = {0, 0, 0};
    l0.pw = {10, 50, 0};
    l0.p1 = {0, 100, 0};
    l0.bulge = draw_primitives().waypoint_to_bulge(l0.p0, l0.pw, l0.p1);

    cavline l1;
    l1.p0 = {0, 100, 0};
    l1.pw = {50, 110, 0};
    l1.p1 = {100, 100, 0};
    l1.bulge = draw_primitives().waypoint_to_bulge(l1.p0, l1.pw, l1.p1);

    drawCavLine(l0, Quantity_NOC_YELLOW, context);
    drawCavLine(l1, Quantity_NOC_YELLOW, context);

    // Run the cavaliercode at the zero plane.
    cavc::Polyline<double> input;
    input.addVertex(l0.p0.X(), l0.p0.Y(), l0.bulge);
    input.addVertex(l1.p0.X(), l1.p0.Y(), l1.bulge);
    input.addVertex(l1.p1.X(), l1.p1.Y(), 0);
    input.isClosed() = false;

    if(offset==0){ // No offset.
        return;
    }
    if(l0.p0.Distance(l0.p1)==0){ // It's a full circle.
        return;
    }
    if(l1.p0.Distance(l1.p1)==0){ // It's a full circle.
        return;
    }

    std::vector<cavc::Polyline<double>> outside = cavc::parallelOffset(input, offset);
    std::cout<<"result size:"<<outside.size()<<std::endl;
    std::vector<cavline> cavvec_outside = convertToCavLines(outside);

    if(outside.size()>0){
        // Update the waypoints.
        for (auto& line : cavvec_outside) {
            line.pw = cavalier_offset().bulge_to_arc_waypoint(line.p0, line.p1, line.bulge);
        }

        // Drawing cavvec offset lines on the zero plane, excluding the last item.
        // for (size_t i = 0; i < cavvec_outside.size() - 1; ++i) {
        //   drawCavLine(cavvec_outside[i], Quantity_NOC_GREEN, context);
        // }
    }

    std::vector<cavc::Polyline<double>> outside_back = cavc::parallelOffset(outside[0], -offset);
    // std::cout<<"result size:"<<outside_back.size()<<std::endl;
    std::vector<cavline> cavvec_outside_back = convertToCavLines(outside_back);
    if(outside_back.size()>0){
        // Update the waypoints.
        for (auto& line : cavvec_outside_back) {
            line.pw = cavalier_offset().bulge_to_arc_waypoint(line.p0, line.p1, line.bulge);
        }

        // Drawing cavvec offset lines on the zero plane, excluding the last item.
        // for (size_t i = 0; i < cavvec_outside_back.size() - 1; ++i) {
        //     drawCavLine(cavvec_outside_back[i], Quantity_NOC_PURPLE, context);
        // }
    }


    std::vector<cavc::Polyline<double>> inside = cavc::parallelOffset(input, -offset);
    // std::cout<<"result size:"<<inside.size()<<std::endl;
    std::vector<cavline> cavvec_inside = convertToCavLines(inside);
    if(inside.size()>0){
        // Update the waypoints.
        for (auto& line : cavvec_inside) {
            line.pw = cavalier_offset().bulge_to_arc_waypoint(line.p0, line.p1, line.bulge);
        }

        // Drawing cavvec offset lines on the zero plane, excluding the last item.
        // for (size_t i = 0; i < cavvec_inside.size() - 1; ++i) {
        //     drawCavLine(cavvec_inside[i], Quantity_NOC_RED, context);
        // }
    }

    std::vector<cavc::Polyline<double>> inside_back = cavc::parallelOffset(inside[0], offset);
    // std::cout<<"result size:"<<inside_back.size()<<std::endl;
    std::vector<cavline> cavvec_inside_back = convertToCavLines(inside_back);
    if(inside_back.size()>0){
        // Update the waypoints.
        for (auto& line : cavvec_inside_back) {
            line.pw = cavalier_offset().bulge_to_arc_waypoint(line.p0, line.p1, line.bulge);
        }

        // Drawing cavvec offset lines on the zero plane, excluding the last item.
        // for (size_t i = 0; i < cavvec_inside_back.size() - 1; ++i) {
        //     drawCavLine(cavvec_inside_back[i], Quantity_NOC_BLUE, context);
        // }
    }

    // Compare the vector results of offset back.
    if(outside_back.size()>inside_back.size()){
        for (size_t i = 0; i < cavvec_outside_back.size() - 1; ++i) {
            drawCavLine(cavvec_outside_back[i], Quantity_NOC_PURPLE, context);
        }
    } else {
        for (size_t i = 0; i < cavvec_inside_back.size() - 1; ++i) {
            drawCavLine(cavvec_inside_back[i], Quantity_NOC_BLUE, context);
        }
    }
}

void cavalier_3d::run_2d_plus_example(opencascade::handle<AIS_InteractiveContext> & context, double offset) {

    cavline l0;
    l0.p0 = {0, 0, 0};
    l0.pw = {10, 50, 0};
    l0.p1 = {0, 100, 0};
    cavline l1;
    l1.p0 = {0, 100, 0};
    l1.pw = {50, 110, 0};
    l1.p1 = {100, 100, 0};
    drawCavLine(l0, Quantity_NOC_YELLOW, context);
    drawCavLine(l1, Quantity_NOC_YELLOW, context);

    std::vector<cavline> cvec=get_2d_fillet_path_2_segments(l0,l1,offset);

    for(uint i=0; i<cvec.size(); i++){
        drawCavLine(cvec.at(i), Quantity_NOC_BLUE, context);
    }
}

void cavalier_3d::run_3d_example(opencascade::handle<AIS_InteractiveContext> & context, double offset) {

    cavline l0;
    l0.p0 = {0, 0, 0};
    l0.pw = {10, 50, 0};
    l0.p1 = {0, 100, 0};
    // l0.pw = draw_primitives().get_line_midpoint(l0.p0,l0.p1);
    cavline l1;
    l1.p0 = {0, 100, 0};
    l1.pw = {50, 80, 0};
    l1.p1 = {100, 100, 0};


    // JUst addint a rotation to the input here.
    // Define the rotation angles in radians for X and Y axes (45 degrees)
    double angleX = M_PI / 4.0; // 45 degrees in radians
    double angleY = M_PI / 4.0; // 45 degrees in radians

    // Define the rotation centers (you can choose any point as the center)
    gp_Pnt rotationCenter(0, 0, 0); // For example, using the origin as the center

    // Define the rotation matrices for X and Y axes
    gp_Trsf rotationTransformX, rotationTransformY;
    rotationTransformX.SetRotation(gp_Ax1(rotationCenter, gp_Dir(1, 0, 0)), angleX); // Rotation around X-axis
    rotationTransformY.SetRotation(gp_Ax1(rotationCenter, gp_Dir(0, 1, 0)), angleY); // Rotation around Y-axis

    // Apply the rotations to each point
    l0.p0.Transform(rotationTransformX);
    l0.pw.Transform(rotationTransformX);
    l0.p1.Transform(rotationTransformX);

    l1.p0.Transform(rotationTransformX);
    l1.pw.Transform(rotationTransformX);
    l1.p1.Transform(rotationTransformX);

    l0.p0.Transform(rotationTransformY);
    l0.pw.Transform(rotationTransformY);
    l0.p1.Transform(rotationTransformY);

    l1.p0.Transform(rotationTransformY);
    l1.pw.Transform(rotationTransformY);
    l1.p1.Transform(rotationTransformY);
    // End of rotation input.


    // l1.pw = draw_primitives().get_line_midpoint(l1.p0,l1.p1);
    drawCavLine(l0, Quantity_NOC_YELLOW, context);
    drawCavLine(l1, Quantity_NOC_YELLOW, context);

    gp_Pln originalPlane;
    draw_primitives().create_3d_plane(l0.p0,l0.p1,l1.p1,originalPlane);

    // Align plane to the origin
    gp_Pln zeroPlane = AlignPlaneToOrigin(originalPlane);

    // Port points from the original plane to the zero plane
    PortCavLineBetweenPlanes(originalPlane, zeroPlane, l0);
    PortCavLineBetweenPlanes(originalPlane, zeroPlane, l1);

    std::vector<cavline> cvec=get_2d_fillet_path_2_segments(l0,l1,offset);

    // Port back the lines of cavvec to the original plane
    for (auto& line : cvec) {
        PortCavLineBetweenPlanes(zeroPlane, originalPlane, line);
        drawCavLine(line, Quantity_NOC_RED, context);
    }
}

void cavalier_3d::run_3d_plus_example(opencascade::handle<AIS_InteractiveContext> & context, double offset) {

    cavline l0;
    l0.p0 = {0, 0, 0};
    l0.pw = {50, 0, 0};
    l0.p1 = {100, 0, 0};

    cavline l1;
    l1.p0 = {100, 0, 0};
    l1.pw = {150, 50, 0};
    l1.p1 = {200, 0, 0};

    //    cavline l2;
    //    l2.p0 = {100, 0, 100};
    //    l2.pw = {150, 0, 100};
    //    l2.p1 = {200, 0, 100};

    //    cavline l3;
    //    l3.p0 = {200, 0, 100};
    //    l3.pw = {220, 50, 100};
    //    l3.p1 = {200, 100, 100};

    std::vector<cavline> cvec;

    get_3d_fillet_path_2_segments(l0,l1,offset,cvec);
    // Show all.
    for (auto& line : cvec) {
        drawCavLine(line, Quantity_NOC_RED, context);
        std::cout<<"bulge:"<<line.bulge<<" lenght:"<<line.lenght<<std::endl;
    }

    //    cvec=get_3d_fillet_path_2_segments(l1,l2,offset);
    //    // Show all.
    //    for (auto& line : cvec) {
    //        drawCavLine(line, Quantity_NOC_RED, context);
    //        std::cout<<"bulge:"<<line.bulge<<" lenght:"<<line.lenght<<std::endl;
    //    }

    //    cvec=get_3d_fillet_path_2_segments(l2,l3,offset);
    //    // Show all.
    //    for (auto& line : cvec) {
    //        drawCavLine(line, Quantity_NOC_RED, context);
    //        std::cout<<"bulge:"<<line.bulge<<" lenght:"<<line.lenght<<std::endl;
    //    }
}








