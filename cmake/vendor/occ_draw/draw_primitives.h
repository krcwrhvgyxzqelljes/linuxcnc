#ifndef DRAW_PRIMITIVES_H
#define DRAW_PRIMITIVES_H

#include "AIS_Line.hxx"
#include <AIS_Shape.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Selection.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_ColoredDrawer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Standard_DefineHandle.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeArcOfEllipse.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeEllipse.hxx>
#include <GC_MakeSegment.hxx>
#include <gce_MakeRotation.hxx>
#include <TopExp.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
#include <TDF_Label.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopTools.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include "Geom_Axis2Placement.hxx"
#include <GCE2d_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <XCAFDoc_Area.hxx>
#include <XCAFDoc_Centroid.hxx>
#include <XCAFDoc_Datum.hxx>
#include <XCAFDoc_Dimension.hxx>
#include <XCAFDoc_Location.hxx>
#include <XCAFDoc_Volume.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>

#include "XCAFPrs_DocumentExplorer.hxx"
#include <TDataStd_Name.hxx>
#include <XCAFDoc_AssemblyItemId.hxx>
#include <XCAFDoc_AssemblyItemRef.hxx>

#include <Font_BRepFont.hxx>
#include <Font_BRepTextBuilder.hxx>
#include <Bnd_Box.hxx>

#include "gp_Elips.hxx"


#include <NCollection_Mat4.hxx>
#include <gp_Quaternion.hxx>

#include <gce_MakeCirc.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_UniformAbscissa.hxx>

#ifdef Success
#undef Success
#endif

#include <math.h>
#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>

class draw_primitives
{
public:
    draw_primitives();

    enum ShapeType {
        point = 1,
        line = 2,
        arc = 3,
        circle = 4,
        unknown = 5
    };

    static int get_shape_type(const Handle(AIS_Shape)& shape);
    static int get_shape_type(const gp_Pnt p0, const gp_Pnt p1, const gp_Pnt p2);

    // Load drawing:
    std::vector<Handle(AIS_Shape)> load_stepfile(std::string filename);

    // Snap to closest endpoint.
    static bool snap_to_closest_endpoint(std::vector<Handle(AIS_Shape)> shapevec, gp_Pnt mousepos, double dist, gp_Pnt &snappos);
    static bool snap_to_closest_midpoint(std::vector<Handle(AIS_Shape)> shapevec, gp_Pnt mousepos, double dist, gp_Pnt &snappos);

    // Downcast interactive object to ais_shape.
    static Handle(AIS_Shape) downcast(Handle(AIS_InteractiveObject) interactiveObject);

    // Draw 2d primitives:
    static Handle(AIS_Shape) draw_2d_circle(gp_Pnt center,double radius);
    static Handle(AIS_Shape) draw_cp_2d_arc(gp_Pnt center, gp_Pnt point1, gp_Pnt point2);
    static Handle(AIS_Shape) draw_2d_acad_arc(gp_Pnt center, double radius, const Standard_Real alpha1, const Standard_Real alpha2);
    static Handle(AIS_Shape) draw_2d_ellipse(gp_Pnt center, gp_Pnt secpoint, double alpha_start, double alpha_end, double ratio);
    static Handle(AIS_Shape) draw_2d_text(std::string str, int textheight, gp_Pnt point, double rot_deg);

    // Draw 3d primitives:
    static Handle(AIS_Shape) draw_3d_point(gp_Pnt point);
    static Handle(AIS_Shape) draw_3d_line(const gp_Pnt &p0, const gp_Pnt &p1);
    static Handle(AIS_Shape) draw_3d_line_wire(std::vector<gp_Pnt> pvec);
    static Handle(AIS_Shape) draw_3p_3d_arc(const gp_Pnt &p0, const gp_Pnt &pw, const gp_Pnt &p1);
    static Handle(AIS_Shape) draw_3d_acad_arc(gp_Pnt center, double radius, double alpha1, double alpha2, gp_Dir dir);
    static Handle(AIS_Shape) draw_3d_pc_arc_closest(gp_Pnt point1,gp_Pnt point2,gp_Pnt center,gp_Dir dir ,gp_Pnt closest);
    static Handle(AIS_Shape) draw_3d_pc_arc(gp_Pnt point1, gp_Pnt point2, gp_Pnt center, double dir_Xv, double dir_Yv, double dir_Zv, gp_Pnt &pw);
    static Handle(AIS_Shape) draw_3d_pc_arc(gp_Pnt point1,gp_Pnt point2,gp_Pnt center, double dir_Xv, double dir_Yv, double dir_Zv);
    static Handle(AIS_Shape) draw_3p_3d_circle(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3);
    static Handle(AIS_Shape) draw_3d_pc_circle(gp_Pnt center, gp_Dir dir, double radius);
    static Handle(AIS_Shape) draw_3d_pc_circle(gp_Pnt center, double dir_Xv, double dir_Yv, double dir_Zv, double radius);

    // Draw 3d surface:
    static Handle(AIS_Shape) draw_3d_surface(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4);

    // Draw 3d solids:
    static Handle(AIS_Shape) draw_3d_cone(gp_Pnt centerpoint, double bottomdiameter, double topdiameter, double height);
    static Handle(AIS_Shape) draw_3d_tcp_cone(gp_Pnt centerpoint, double bottomdiameter, double topdiameter, double height);
    static Handle(AIS_Shape) draw_3d_cilinder(double radius, double height);
    static Handle(AIS_Shape) draw_3d_sphere(double radius, gp_Pnt center);
    static Handle(AIS_Shape) draw_3d_box(double dx, double dy, double dz);

    // Draw 3d tools:
    static std::vector<Handle(AIS_Shape)> draw_3d_arc_lenght(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3);
    static Handle(AIS_Shape) rotate_3d(Handle(AIS_Shape) shape, gp_Pnt center, double euler_z, double euler_y, double euler_x);
    static Handle(AIS_Shape) rotate_translate_3d_quaternion(Handle(AIS_Shape) shape, gp_Pnt translation, double euler_z, double euler_y, double euler_x);
    static Handle(AIS_Shape) translate_3d(Handle(AIS_Shape) shape, gp_Pnt current, gp_Pnt target);
    static Handle(AIS_Shape) colorize(Handle(AIS_Shape) shape, Quantity_Color color, double transparancy);
    static double get_line_lenght(gp_Pnt p1, gp_Pnt p2);
    static double waypoint_to_bulge(gp_Pnt p1, gp_Pnt pw, gp_Pnt p2);

    // Draw 3d sets:
    static Handle(AIS_Shape) draw_3d_origin(gp_Pnt origin, double linelenght);
    static Handle(AIS_Shape) draw_3d_point_origin_cone(gp_Pnt point, gp_Pnt euler);
    static Handle(AIS_Shape) draw_3d_point_origin_cone_text(gp_Pnt point, gp_Pnt euler, std::string text, int textheight, int textrotation);
    static Handle(AIS_Shape) draw_3d_line_origin_cone_text(gp_Pnt point1, gp_Pnt point2, gp_Pnt euler1, gp_Pnt euler2, std::string text, int textheight);
    static Handle(AIS_Shape) draw_3d_wire_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight);
    static Handle(AIS_Shape) draw_3d_arc_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight);
    static Handle(AIS_Shape) draw_3d_circle_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight);

    // Intersection functions:
    static bool intersect_3d_sphere_sphere(gp_Pnt pc1, double radius1, gp_Pnt pc2, double radius2, gp_Pnt &pi);
    static bool intersect_3d_line_sphere(gp_Pnt cp, gp_Pnt p0, gp_Pnt p1, double radius, gp_Pnt &pi);

    //! Returns true if a sphere touches a line and a arc at the same time.
    static bool intersect_line_arc_sphere(Handle(AIS_Shape) lineShape, Handle(AIS_Shape) arcShape, gp_Pnt sphere_pc, double sphere_radius);

    // Offset functions:
    //void offset_3d_point_on_line(Eigen::Vector3d A, Eigen::Vector3d B, double offset_a, Eigen::Vector3d &C);
    static void offset_3d_point_on_line(gp_Pnt A, gp_Pnt B, double offset_a, gp_Pnt &C);
    static void offset_line_on_a_plane(gp_Pnt line_p0, gp_Pnt line_p1, gp_Pnt plane_p0, double offset, gp_Pnt &offset_line_p0, gp_Pnt &offset_line_p1);

    // Plane functions:
    static bool create_3d_plane(gp_Pnt p_origin, gp_Pnt p2, gp_Pnt p3, gp_Pln &plane);
    static gp_Pnt projectPointOnPlane(gp_Pnt point, gp_Pnt planeOrigin, gp_Vec planeNormal);

    // Transformation functions:
    //! Create coordinate system from 3 points on a plane, where p0-p1 is x-axis.
    static gp_Trsf create_transformation_matrix(gp_Pnt p0, gp_Pnt p1, gp_Pnt point_on_plane);

    // Info request functions:
    static void get_closest_point_to_line(gp_Pnt point, Handle(AIS_Shape) shape, gp_Pnt &intersection);
    static void get_closest_point_to_line(gp_Pnt point, gp_Pnt line_start, gp_Pnt line_end, gp_Pnt &intersection);

    static gp_Pnt get_point(Handle(AIS_Shape) shape);
    static gp_Pnt get_line_midpoint(gp_Pnt point1, gp_Pnt point2);
    static void get_line_points(Handle(AIS_Shape) shapeLine, gp_Pnt &p1, gp_Pnt &p2);
    static void get_arc_points(Handle(AIS_Shape) shapeArc, gp_Pnt &p1, gp_Pnt &p2, gp_Pnt &pw, gp_Pnt &pc);

    static gp_Pnt get_arc_centerpoint(Handle(AIS_Shape) shapeArc);
    static double get_arc_radius(Handle(AIS_Shape) shapeArc);

    static bool is_point_on_line(gp_Pnt p1, gp_Pnt p2, gp_Pnt pi);
    static bool is_point_on_line(Handle(AIS_Shape) shapeLine, gp_Pnt pi);
    static bool is_point_on_arc(Handle(AIS_Shape) shapeLine, gp_Pnt pi);
    static bool is_point_on_plane(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt pi); //! is pi on plane p1,p2,p3?

    // Geometric functions:
    static gp_Dir create_normal_from_3_points_on_plane(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3);
    static void organize_line_arc_points(gp_Pnt &line_p0, gp_Pnt &line_p1, gp_Pnt &arc_p0, gp_Pnt &arc_p1); //! Ensure a correct point flow.

    static std::vector<Handle(AIS_Shape)> create_line_arc_intersections(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset);

    static void create_3d_line_arc_offset_lines(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset, gp_Pnt &p0, gp_Pnt &p1, gp_Pnt &p2, gp_Pnt &p3);

    //! Test functions.
    static bool draw_3d_line_arc_offset_lines( Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset, Handle(AIS_Shape) &shapeA, Handle(AIS_Shape) &shapeB);

    // Print functions:
    static void print_gp_Pnt(gp_Pnt pnt);
    static void print_gp_Pnt(std::string text, gp_Pnt pnt);

    // 2d operations for clothoids.
    static double get_2d_line_angle(const gp_Pnt& p1, const gp_Pnt& p2);
    static double get_2d_arc_end_angle(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1);
    static double get_2d_arc_start_angle(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1);
    static int get_2d_arc_direction(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1);
    static double get_2d_arc_radius(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1);
    static int get_lines_colinear(const gp_Pnt& p0, const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& p3);

    static gp_Pln create_top_view_plane();
    static void port_points_between_planes(const gp_Pln& sourcePlane, const gp_Pln& targetPlane, std::vector<gp_Pnt> &pvec);
    static void port_point_between_planes(const gp_Pln& sourcePlane, const gp_Pln& targetPlane, gp_Pnt &p);
    static gp_Trsf get_transformation_2_planes(gp_Pln a, gp_Pln b);
    static gp_Pln align_plane_to_origin(const gp_Pln& originalPlane);

    static void get_3d_arc_radius_and_center(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1, double& radius, gp_Pnt& center);
    static void get_3d_arc_angle_radians(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1, double &theta);

    static void interpolate_point_on_line(const gp_Pnt& p0, const gp_Pnt& p1, double progress, gp_Pnt& pi);
    // Has errors. static void interpolate_point_on_arc(gp_Pnt p0, gp_Pnt pw, gp_Pnt p1, double progress, gp_Pnt &pi);
    static void interpolate_point_on_arc(gp_Pnt p0, gp_Pnt pw, gp_Pnt p1, double progress, gp_Pnt &pi);
    static gp_Pnt rotate_point_around_line(const gp_Pnt& p_to_rotate, double theta, const gp_Pnt& p0, const gp_Pnt& p1);
    static int get_3d_arc_angle_rad_center(const gp_Pnt &p0, const gp_Pnt &pw, const gp_Pnt &p1, double &angle_rad, gp_Pnt &pc);
    static double get_3d_arc_lenght(const gp_Pnt &p0, const gp_Pnt &pw, const gp_Pnt &p1);

    static int get_3d_fillet_start_end_point(const gp_Pnt &p0, gp_Pnt &p1, gp_Pnt &p2, gp_Pnt &p3, gp_Pnt &p4, const gp_Pnt &p5, double offset);
    static bool is_valid_point(const gp_Pnt &p);
    static Handle(AIS_Shape) draw_universal_3d_shape(gp_Pnt p0, gp_Pnt p1, gp_Pnt p2);

private:
    Handle(XCAFDoc_ColorTool) aColorTool;
    Handle(XCAFDoc_ShapeTool) aShapeTool;
    std::vector<Handle(AIS_Shape)> Ais_ShapeVec;
    void Visit(const TDF_Label& theLabel);
};

#endif // DRAW_PRIMITIVES_H




















