// Created on: 1995-07-19
// Created by: Modelistation
// Copyright (c) 1995-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.


#include <Adaptor3d_Curve.hxx>
#include <Extrema_CurveTool.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <GCPnts_TangentialDeflection.hxx>

//=======================================================================
//function : IsPeriodic
//purpose  : 
//=======================================================================
Standard_Boolean Extrema_CurveTool::IsPeriodic(const Adaptor3d_Curve& C)
{
  GeomAbs_CurveType aType = GetType(C);
  if (aType == GeomAbs_Circle ||
      aType == GeomAbs_Ellipse)
    return Standard_True;
  else
    return C.IsPeriodic();
}

//=======================================================================
//function : DeflCurvIntervals
//purpose  : 
//=======================================================================
Handle(TColStd_HArray1OfReal) 
Extrema_CurveTool::DeflCurvIntervals(const Adaptor3d_Curve& C)
{
  const Standard_Real epsd = 1.e-3;
  const Standard_Real maxdefl = 1.e3;
  const Standard_Real mindefl = 1.e-3;
  Handle(TColStd_HArray1OfReal) Intervals;
  Standard_Integer nbpnts = 23, i;
  Standard_Real L = 0.;
  Standard_Real tf = C.FirstParameter(), tl = C.LastParameter();
  gp_Pnt aP = C.Value(tf);
  for (i = 2; i <= nbpnts; ++i)
  {
    Standard_Real t = (tf * (nbpnts - i) + (i - 1) * tl) / (nbpnts - 1);
    gp_Pnt aP1 = C.Value(t);
    L += aP.Distance(aP1);
  }
  //
  Standard_Real dLdt = L / (tl - tf);
  if (L <= Precision::Confusion() || dLdt < epsd || (tl - tf) > 10000.) // To avoid problemwith GCPnts
  {
    nbpnts = 2;
    Intervals = new TColStd_HArray1OfReal(1, nbpnts);
    Intervals->SetValue(1, tf);
    Intervals->SetValue(nbpnts, tl);
    return Intervals;
  }
  //
  Standard_Real aDefl = Max(0.01 * L / (2. * M_PI), mindefl);
  if (aDefl > maxdefl)
  {
    nbpnts = 2;
    Intervals = new TColStd_HArray1OfReal(1, nbpnts);
    Intervals->SetValue(1, tf);
    Intervals->SetValue(nbpnts, tl);
    return Intervals;
  }
  //
  Standard_Real aMinLen = Max(.00001*L, Precision::Confusion());
  Standard_Real aTol = Max(0.00001*(tl - tf), Precision::PConfusion());
  //
  GCPnts_TangentialDeflection aPntGen(C, M_PI / 6, aDefl, 2, aTol, aMinLen);
  nbpnts = aPntGen.NbPoints();
  Intervals = new TColStd_HArray1OfReal(1, nbpnts);
  for (i = 1; i <= nbpnts; ++i)
  {
    Intervals->SetValue(i, aPntGen.Parameter(i));
  }
  return Intervals;
}
