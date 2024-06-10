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
//#include <BRepLib.hxx>
//#include <BRepOffsetAPI_ThruSections.hxx>
//#include <BRepBuilderAPI_MakeWire.hxx>
//#include <BRepBuilderAPI_Transform.hxx>
//#include <BRepBuilderAPI_MakeFace.hxx>
//#include <BRepBuilderAPI_MakeEdge.hxx>
//#include <BRepPrimAPI_MakePrism.hxx>
//#include <BRepPrimAPI_MakeSphere.hxx>
//#include <BRepPrimAPI_MakeCone.hxx>
//#include <BRepPrimAPI_MakeTorus.hxx>
//#include <BRepPrimAPI_MakeBox.hxx>
//#include <BRepFilletAPI_MakeFillet.hxx>
//#include <BRepOffsetAPI_MakeThickSolid.hxx>
//#include <BRepPrimAPI_MakeCylinder.hxx>
//#include <BRepAlgoAPI_Fuse.hxx>
//#include <BRepTools_ReShape.hxx>
//#include <BRepTools.hxx>
//#include <BRepBuilderAPI_MakeVertex.hxx>
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

#include <vector>
#include <iostream>
#include <Dense>
#include <Geometry>

// using namespace Eigen;

class draw_primitives
{
public:
    draw_primitives();

    // Load drawing:
    std::vector<Handle(AIS_Shape)> load_stepfile(std::string filename);

    // Snap to closest endpoint.
    bool snap_to_closest_endpoint(std::vector<Handle(AIS_Shape)> shapevec, gp_Pnt mousepos, double dist, gp_Pnt &snappos);
    bool snap_to_closest_midpoint(std::vector<Handle(AIS_Shape)> shapevec, gp_Pnt mousepos, double dist, gp_Pnt &snappos);

    // Downcast interactive object to ais_shape.
    Handle(AIS_Shape) downcast(Handle(AIS_InteractiveObject) interactiveObject);

    // Draw 2d primitives:
    Handle(AIS_Shape) draw_2d_circle(gp_Pnt center,double radius);
    Handle(AIS_Shape) draw_cp_2d_arc(gp_Pnt center, gp_Pnt point1, gp_Pnt point2);
    Handle(AIS_Shape) draw_2d_acad_arc(gp_Pnt center, double radius, const Standard_Real alpha1, const Standard_Real alpha2);
    Handle(AIS_Shape) draw_2d_ellipse(gp_Pnt center, gp_Pnt secpoint, double alpha_start, double alpha_end, double ratio);
    Handle(AIS_Shape) draw_2d_text(std::string str, int textheight, gp_Pnt point, double rot_deg);

    // Draw 3d primitives:
    Handle(AIS_Shape) draw_3d_point(gp_Pnt point);
    Handle(AIS_Shape) draw_3d_line(gp_Pnt point1, gp_Pnt point2);
    Handle(AIS_Shape) draw_3d_line_wire(std::vector<gp_Pnt> points);
    Handle(AIS_Shape) draw_3p_3d_arc(gp_Pnt point1, gp_Pnt point2, gp_Pnt point3);
    Handle(AIS_Shape) draw_3d_acad_arc(gp_Pnt center, double radius, double alpha1, double alpha2, gp_Dir dir);
    Handle(AIS_Shape) draw_3d_pc_arc_closest(gp_Pnt point1,gp_Pnt point2,gp_Pnt center,gp_Dir dir ,gp_Pnt closest);
    Handle(AIS_Shape) draw_3d_pc_arc(gp_Pnt point1, gp_Pnt point2, gp_Pnt center, double dir_Xv, double dir_Yv, double dir_Zv);
    Handle(AIS_Shape) draw_3p_3d_circle(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3);
    Handle(AIS_Shape) draw_3d_pc_circle(gp_Pnt center, gp_Dir dir, double radius);

    // Draw 3d surface:
    Handle(AIS_Shape) draw_3d_surface(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4);

    // Draw 3d solids:
    Handle(AIS_Shape) draw_3d_cone(gp_Pnt centerpoint, double bottomdiameter, double topdiameter, double height);
    Handle(AIS_Shape) draw_3d_tcp_cone(gp_Pnt centerpoint, double bottomdiameter, double topdiameter, double height);
    Handle(AIS_Shape) draw_3d_cilinder(double radius, double height);
    Handle(AIS_Shape) draw_3d_sphere(double radius, gp_Pnt center);
    Handle(AIS_Shape) draw_3d_box(double dx, double dy, double dz);

    // Draw 3d tools:
    std::vector<Handle(AIS_Shape)> draw_3d_arc_lenght(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3);
    double get_3d_arc_lenght(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3, int divisions);
    gp_Pnt get_3d_arc_point_given_arclenght_fromstartpoint(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3, double arclenght_from_startpoint);
    Handle(AIS_Shape) rotate_3d(Handle(AIS_Shape) shape, gp_Pnt center, double euler_z, double euler_y, double euler_x);
    Handle(AIS_Shape) rotate_translate_3d_quaternion(Handle(AIS_Shape) shape, gp_Pnt translation, double euler_z, double euler_y, double euler_x);
    Handle(AIS_Shape) translate_3d(Handle(AIS_Shape) shape, gp_Pnt current, gp_Pnt target);
    Handle(AIS_Shape) colorize(Handle(AIS_Shape) shape, Quantity_Color color, double transparancy);
    gp_Pnt midpoint(gp_Pnt point1, gp_Pnt point2);

    // Draw 3d sets:
    Handle(AIS_Shape) draw_3d_origin(gp_Pnt origin, double linelenght);
    Handle(AIS_Shape) draw_3d_point_origin_cone(gp_Pnt point, gp_Pnt euler);
    Handle(AIS_Shape) draw_3d_point_origin_cone_text(gp_Pnt point, gp_Pnt euler, std::string text, int textheight, int textrotation);
    Handle(AIS_Shape) draw_3d_line_origin_cone_text(gp_Pnt point1, gp_Pnt point2, gp_Pnt euler1, gp_Pnt euler2, std::string text, int textheight);
    Handle(AIS_Shape) draw_3d_wire_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight);
    Handle(AIS_Shape) draw_3d_arc_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight);
    Handle(AIS_Shape) draw_3d_circle_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight);

    // Intersection functions:
    bool intersect_3d_sphere_sphere(gp_Pnt pc1, double radius1, gp_Pnt pc2, double radius2, gp_Pnt &pi);
    bool intersect_3d_line_sphere(gp_Pnt cp, gp_Pnt p0, gp_Pnt p1, double radius, gp_Pnt &pi);

    //! Returns true if a sphere touches a line and a arc at the same time.
    bool intersect_line_arc_sphere(Handle(AIS_Shape) lineShape, Handle(AIS_Shape) arcShape, gp_Pnt sphere_pc, double sphere_radius);

    // Offset functions:
    void offset_3d_point_on_line(Eigen::Vector3d A, Eigen::Vector3d B, double offset_a, Eigen::Vector3d &C);
    void offset_3d_point_on_line(gp_Pnt A, gp_Pnt B, double offset_a, gp_Pnt &C);
    void offset_line_on_a_plane(gp_Pnt line_p0, gp_Pnt line_p1, gp_Pnt plane_p0, double offset, gp_Pnt &offset_line_p0, gp_Pnt &offset_line_p1);

    // Rotate functions:
    gp_Pnt rotate_3d_point_around_line(gp_Pnt thePointToRotate, double theta, gp_Pnt theLineP1, gp_Pnt theLineP2);

    // Plane functions:
    bool create_3d_plane(gp_Pnt p_origin, gp_Pnt p2, gp_Pnt p3, gp_Pln &plane);
    gp_Pnt projectPointOnPlane(gp_Pnt point, gp_Pnt planeOrigin, gp_Vec planeNormal);

    // Transformation functions:
    //! Create coordinate system from 3 points on a plane, where p0-p1 is x-axis.
    gp_Trsf create_transformation_matrix(gp_Pnt p0, gp_Pnt p1, gp_Pnt point_on_plane);

    // Info request functions:
    void get_closest_point_to_line(gp_Pnt point, Handle(AIS_Shape) shape, gp_Pnt &intersection);
    void get_closest_point_to_line(gp_Pnt point, gp_Pnt line_start, gp_Pnt line_end, gp_Pnt &intersection);
    int get_shapetype(Handle(AIS_Shape) shape);
    gp_Pnt get_point(Handle(AIS_Shape) shape);

    gp_Pnt get_arc_centerpoint(Handle(AIS_Shape) shapeArc);
    double get_arc_radius(Handle(AIS_Shape) shapeArc);
    void get_line_points(Handle(AIS_Shape) shapeLine, gp_Pnt &p1, gp_Pnt &p2);
    void get_arc_points(Handle(AIS_Shape) shapeArc, gp_Pnt &p1, gp_Pnt &p2, gp_Pnt &pw, gp_Pnt &pc);
    bool is_point_on_line(gp_Pnt p1, gp_Pnt p2, gp_Pnt pi);
    bool is_point_on_line(Handle(AIS_Shape) shapeLine, gp_Pnt pi);
    bool is_point_on_arc(Handle(AIS_Shape) shapeLine, gp_Pnt pi);
    bool is_point_on_plane(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt pi); //! is pi on plane p1,p2,p3?

    // Geometric functions:
    gp_Dir create_normal_from_3_points_on_plane(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3);
    void organize_line_arc_points(gp_Pnt &line_p0, gp_Pnt &line_p1, gp_Pnt &arc_p0, gp_Pnt &arc_p1); //! Ensure a correct point flow.

    std::vector<Handle(AIS_Shape)> create_line_arc_intersections(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset);

    // Create fillets:
    bool draw_3d_line_line_fillet(Handle(AIS_Shape) shapeLineA, Handle(AIS_Shape) shapeLineB, double radius, Handle(AIS_Shape) &shape);
    bool draw_3d_line_arc_fillet(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double radius, Handle(AIS_Shape) &shape);
    bool draw_3d_arc_line_fillet(Handle(AIS_Shape) shapeArc, Handle(AIS_Shape) shapeLine, double radius, Handle(AIS_Shape) &shape);
    //! Left for info, function using eigen to create plane.
    bool draw_3d_line_line_fillet_conventional(Handle(AIS_Shape) shapeA, Handle(AIS_Shape) shapeB, double radius, Handle(AIS_Shape) &shape);
    bool draw_3d_line_arc_fillet_conventional(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double radius, Handle(AIS_Shape) &shape);
    bool draw_3d_arc_line_fillet_conventional(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double radius, Handle(AIS_Shape) &shape);

    void create_3d_line_arc_offset_lines(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset, gp_Pnt &p0, gp_Pnt &p1, gp_Pnt &p2, gp_Pnt &p3);

    //! Test functions.
    bool draw_3d_line_arc_offset_lines( Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset, Handle(AIS_Shape) &shapeA, Handle(AIS_Shape) &shapeB);


    // Print functions:
    void print_gp_Pnt(gp_Pnt pnt);
    void print_gp_Pnt(std::string text, gp_Pnt pnt);

private:
    Handle(XCAFDoc_ColorTool) aColorTool;
    Handle(XCAFDoc_ShapeTool) aShapeTool;
    std::vector<Handle(AIS_Shape)> Ais_ShapeVec;
    void Visit(const TDF_Label& theLabel);
};

#endif // DRAW_PRIMITIVES_H




















