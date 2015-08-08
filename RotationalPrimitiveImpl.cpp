// Copyright Hugh Perkins 2005
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// You can contact me at hughperkins@gmail.com for more information.
//

//! \file
//! \brief RotationalPrimitiveImpl class function definitions

#define GLVECTOR_IOSTREAM

#include "RotationalPrimitiveImpl.h"

#include <iostream>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>

namespace FractalSpline
{

    void RotationalPrimitiveImpl::UpdateTransforms()
    {
        RotationalExtrusionPath.UpdatePath();
        BuildFaces();
    }

    void RotationalPrimitiveImpl::SetLevelOfDetail( const int iNewDetail )
    {
        RotationalExtrusionPath.SetLevelOfDetail( iNewDetail );
    }

    void RotationalPrimitiveImpl::SetTwist( const int iNewTwist )
    {
        RotationalExtrusionPath.SetTwist( iNewTwist );
    }

    void RotationalPrimitiveImpl::SetShear( const float fNewShear )
    {
        RotationalExtrusionPath.SetShear( fNewShear );
    }

    void RotationalPrimitiveImpl::SetTopSizeX( const float fNewShear )
    {
        RotationalExtrusionPath.SetTopSizeX( fNewShear );
    }

    void RotationalPrimitiveImpl::SetTopSizeY( const float fNewTopSizeY )
    {
        RotationalExtrusionPath.SetTopSizeY( fNewTopSizeY );
    }

    void RotationalPrimitiveImpl::SetHoleSize( const int iNewHoleSize )
    {
        iHoleSize = iNewHoleSize;
        float fHoleSize = (float)iHoleSize / 200.0;
        RotationalExtrusionPath.SetRadialSectionScale( 0.5 - fHoleSize / 2.0 );
        RotationalExtrusionPath.SetRadius( 0.25 + fHoleSize / 4.0 );
    }

    void RotationalPrimitiveImpl::SetHollow( const int iNewHollow )
    {
        // cout << "SetHollow()" << endl;
        iHollow = iNewHollow;

        if( iHollow > 95 )
        {
            iHollow = 95;
        }
        else if( iHollow < 0 )
        {
            iHollow = 0;
        }

        if( iHollow == 0 )
        {
            bShowHollow = false;
            return;
        }
    }

    void RotationalPrimitiveImpl::SetTextureStartOffset( const float u, const float v )
    {
        fTextureStartU = u;
        fTextureStartV = v;
    }

    void RotationalPrimitiveImpl::SetTextureEndOffset( const float u, const float v )
    {
        fTextureEndU = u;
        fTextureEndV = v;
    }

    void RotationalPrimitiveImpl::SetCutStart( const int iNewCutStart )
    {
        iExtrusionStart = iNewCutStart;
        RotationalExtrusionPath.SetCutStartAngle( (float)iNewCutStart * 2 * 3.1415927 / 200.0 );
    }

    void RotationalPrimitiveImpl::SetCutEnd( const int iNewCutEnd )
    {
        iExtrusionEnd = iNewCutEnd;
        RotationalExtrusionPath.SetCutEndAngle( (float)iNewCutEnd * 2 * 3.1415927 / 200.0 );
    }

    void RotationalPrimitiveImpl::SetAdvancedCutStart( const int iNewCutStart )
    {
        iCutStart = iNewCutStart;
        if( iCutStart < 0 )
        {
            iCutStart = 0;
        }
        else if( iCutEnd >= iMaxCut )
        {
            iCutEnd = iMaxCut - 1;
        }

        if( iCutStart >= iCutEnd )
        {
            iCutEnd = iCutStart + 1;
        }
    }

    void RotationalPrimitiveImpl::SetAdvancedCutEnd( const int iNewCutEnd )
    {
        iCutEnd = iNewCutEnd;
        if( iCutEnd < 1 )
        {
            iCutEnd = 1;
        }
        else if( iCutEnd > iMaxCut )
        {
            iCutEnd = iMaxCut;
        }

        if( iCutEnd <= iCutStart )
        {
            iCutStart = iCutEnd - 1;
        }
    }

    //! BAsically we check the quadrant, then intersect it, then we just intersect the radius with the appropriate face
    void RotationalPrimitiveImpl::GetCutIntersect( GLVector3<float> &IntersectPoint, const int iCut, const float fCubeHalfWidth )
    {
        IntersectPoint.z = 0;

        int iCutQuadrant = GetCutQuadrant( iCut );
        float fCutRatio = (float)iCut / (float)iMaxCut;
        //  cout << " GetCutIntersect iCut = " << iCut << " CutQuadrant = " << iCutQuadrant << endl;

        // float fTanRatio = fCutRatio - 0.1666666666;

        GLVector3<float> linestart, lineend;
        linestart = ReferenceVertices[ iCutQuadrant] * fCubeHalfWidth / 0.5;
        //  cout << " iCutQuadrant " << iCutQuadrant << " iNumberFaces " << iNumberFaces << endl;
        if( iCutQuadrant < iNumberFaces - 1 )
        {
            lineend = ReferenceVertices[ iCutQuadrant + 1 ]  * fCubeHalfWidth / 0.5;
        }
        else
        {
            lineend = ReferenceVertices[ 0 ]  * fCubeHalfWidth / 0.5;
        }

        float fAngle = GetAngleWithXAxis( fCutRatio );

        //cout << "intersect fAngle = " << fAngle << " linestart " << linestart << " lineend " << lineend << " intersectpoint " << IntersectPoint << endl;

        GLVector3<float> CutVectorPerp;
        CutVectorPerp.z = 0;
        CutVectorPerp.x = - sin( fAngle );
        CutVectorPerp.y = cos( fAngle );

        // Grabbed this from http://softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
        //  cout << " CutVectorPerp.dot( lineend - linestart ) " << CutVectorPerp.dot( lineend - linestart ) << endl;
        IntersectPoint = linestart - ( lineend - linestart ) * CutVectorPerp.dot( linestart ) / CutVectorPerp.dot( lineend - linestart );

        //  Intersect_Lines(0.0, 0.0, cos( fAngle ), sin( fAngle ),
        //                  linestart.x, linestart.y,lineend.x, lineend.y,
        //                 &IntersectPoint.x,&IntersectPoint.y);
        //
        //   cout << "intersect fAngle = " << fAngle << " linestart " << linestart << " lineend " << lineend << " intersectpoint " << IntersectPoint << endl;

        /*
        static iCount = 0;
        if( iCount == 4 )
        {
          exit(0);
         }
         iCount++;
         */

        /*
        if( IntersectPoint.x < - fCubeHalfWidth )
        {
        IntersectPoint.x = - fCubeHalfWidth;
        }
        else if( IntersectPoint.x > fCubeHalfWidth )
        {
        IntersectPoint.x = fCubeHalfWidth;
        }

        if( IntersectPoint.y < - fCubeHalfWidth )
        {
        IntersectPoint.y = - fCubeHalfWidth;
        }
        else if( IntersectPoint.y > fCubeHalfWidth )
        {
        IntersectPoint.y = fCubeHalfWidth;
        }
        */
    }

    void RotationalPrimitiveImpl::PopulateSingleCutFacePositiveDirection( CrossSection &Face, float &fLength, GLVector3<float> &CutPoint, int iQuadrant, float fHalfCubeWidth, bool bOuter )
    {
        NormalizeQuadrant( iQuadrant );
        // cout << "PopulateSingleCutFacePositiveDirection, quadrant " << iQuadrant << endl;

        Face.RemoveAllPoints();

        GLVector3<float> StartPoint;
        GLVector3<float> EndPoint;

        StartPoint.z = EndPoint.z = 0;

        StartPoint = CutPoint;
        if( iQuadrant < iNumberFaces - 1 )
        {
            EndPoint = ReferenceVertices[ iQuadrant + 1 ] * fHalfCubeWidth / 0.5;
        }
        else
        {
            EndPoint = ReferenceVertices[ 0 ] * fHalfCubeWidth / 0.5;
        }

        fLength = ( EndPoint - StartPoint ).length();

        // cout << "startpoint " << StartPoint << " endpoint " << EndPoint << endl;

        if( bOuter )
        {
            Face.AddPoint( StartPoint.x, StartPoint.y, StartPoint.z );
            Face.AddPoint( EndPoint.x, EndPoint.y, EndPoint.z );
        }
        else
        {
            Face.AddPoint( EndPoint.x, EndPoint.y, EndPoint.z );
            Face.AddPoint( StartPoint.x, StartPoint.y, StartPoint.z );
        }
    }

    void RotationalPrimitiveImpl::PopulateSingleCutFaceNegativeDirection( CrossSection &Face, float &fLength, GLVector3<float> &CutPoint, int iQuadrant, float fHalfCubeWidth, bool bOuter )
    {
        //  cout << "PopulateSingleCutFaceNegativeDirection, quadrant " << iQuadrant << endl;
        NormalizeQuadrant( iQuadrant );

        Face.RemoveAllPoints();

        GLVector3<float> StartPoint;
        GLVector3<float> EndPoint;

        StartPoint.z = EndPoint.z = 0;

        EndPoint = CutPoint;

        StartPoint = ReferenceVertices[ iQuadrant ] * fHalfCubeWidth / 0.5;
        fLength = ( EndPoint - StartPoint ).length();

        //  cout << "startpoint " << StartPoint << " endpoint " << EndPoint << endl;

        if( bOuter )
        {
            Face.AddPoint( StartPoint.x, StartPoint.y, StartPoint.z );
            Face.AddPoint( EndPoint.x, EndPoint.y, EndPoint.z );
        }
        else
        {
            Face.AddPoint( EndPoint.x, EndPoint.y, EndPoint.z );
            Face.AddPoint( StartPoint.x, StartPoint.y, StartPoint.z );
        }

    }

    void RotationalPrimitiveImpl::PopulateCompleteSide( CrossSection &Face, float &fLength, int iQuadrant, float fHalfCubeWidth, bool bOuter )
    {
        NormalizeQuadrant( iQuadrant );

        //  cout << "Populatecompletesdie, quadrant " << iQuadrant << endl;

        Face.RemoveAllPoints();

        GLVector3<float> StartPoint;
        GLVector3<float> EndPoint;

        StartPoint.z = EndPoint.z = 0;

        StartPoint = ReferenceVertices[ iQuadrant ];
        if( iQuadrant < iNumberFaces - 1 )
        {
            EndPoint = ReferenceVertices[ iQuadrant + 1 ];
        }
        else
        {
            EndPoint = ReferenceVertices[ 0 ];
        }

        StartPoint = StartPoint * fHalfCubeWidth / 0.5;
        EndPoint = EndPoint * fHalfCubeWidth / 0.5;

        //  cout << "startpoint " << StartPoint << " endpoint " << EndPoint << endl;

        if( bOuter )
        {
            Face.AddPoint( StartPoint.x, StartPoint.y, StartPoint.z );
            Face.AddPoint( EndPoint.x, EndPoint.y, EndPoint.z );
        }
        else
        {
            Face.AddPoint( EndPoint.x, EndPoint.y, EndPoint.z );
            Face.AddPoint( StartPoint.x, StartPoint.y, StartPoint.z );
        }

        fLength = 2 * fHalfCubeWidth;
    }

    void RotationalPrimitiveImpl::RenderEndCapNoHollow( const int iBottomTextureID, const int iTopTextureID )
    {
        //cout << "Nocutnohollow" << endl;
        GLVector3<float> p1,p2,p3,p4;
        int i;

        GLVector3<float> o1, o2, rawcentre, transformedcentre;
        GLVector3<float> raw1, raw2;

        GLVector3<float> normal;

        rawcentre.x = 0;
        rawcentre.y = 0.0;
        rawcentre.z = 0.0;

        RotationalExtrusionPath.GetTransformedVertex( transformedcentre, rawcentre, 0 );

        CallbackRendererColor( iBottomTextureID );
        pRendererCallbacks->SetTextureID( iFaceTextures[ iBottomTextureID ] );

        for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
        {
            OuterFaces[i].GetVertex( o1, RotationalExtrusionPath, 0, 0 );
            OuterFaces[i].GetVertex( o2, RotationalExtrusionPath, 0, 1 );

            raw1 = OuterFaces[i].GetPoint( 0 );
            raw2 = OuterFaces[i].GetPoint( 1 );

            CalculateNormal( normal, o2,o1,transformedcentre );
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

            pRendererCallbacks->StartTriangle();
            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 ), TextureVCoordToOffsetVCoord( 0.5 ) );
            pRendererCallbacks->AddVertex( transformedcentre.x, transformedcentre.y, transformedcentre.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1- ( raw2.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( o2.x, o2.y, o2.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1- ( raw1.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( o1.x, o1.y, o1.z );

            pRendererCallbacks->EndTriangle();
        }
        //   glEnd();


        RotationalExtrusionPath.GetTransformedVertex( transformedcentre, rawcentre, RotationalExtrusionPath.GetNumberOfTransforms() - 1 );

        CallbackRendererColor( iTopTextureID );
        pRendererCallbacks->SetTextureID( iFaceTextures[ iTopTextureID ] );
        for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
        {
            OuterFaces[i].GetVertex( o1, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, 0 );
            OuterFaces[i].GetVertex( o2, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, 1 );

            raw1 = OuterFaces[i].GetPoint( 0 );
            raw2 = OuterFaces[i].GetPoint( 1 );

            CalculateNormal( normal, o1,o2,transformedcentre );
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

            pRendererCallbacks->StartTriangle();
            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 ), TextureVCoordToOffsetVCoord( 0.5 ) );
            pRendererCallbacks->AddVertex( transformedcentre.x, transformedcentre.y, transformedcentre.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw1.x + 0.5 ), TextureVCoordToOffsetVCoord( raw1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( o1.x, o1.y, o1.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw2.x + 0.5 ), TextureVCoordToOffsetVCoord( raw2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( o2.x, o2.y, o2.z );

            pRendererCallbacks->EndTriangle();
        }

    }


    void RotationalPrimitiveImpl::RenderEndCapHollow( const int iBottomTextureID, const int iTopTextureID )
    {
        int i;
        GLVector3<float> p1,p2,p3,p4;

        GLVector3<float> raw1, raw2, raw3, raw4;
        GLVector3<float> normal;

        for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
        {
            OuterFaces[i].GetVertex( p1, RotationalExtrusionPath, 0, 0 );
            OuterFaces[i].GetVertex( p2, RotationalExtrusionPath, 0, 1 );
            InnerFaces[i].GetVertex( p3, RotationalExtrusionPath, 0, 0 );
            InnerFaces[i].GetVertex( p4, RotationalExtrusionPath, 0, 1 );

            raw1 = OuterFaces[i].GetPoint( 0 );
            raw2 = OuterFaces[i].GetPoint( 1 );
            raw3 = InnerFaces[i].GetPoint( 0 );
            raw4 = InnerFaces[i].GetPoint( 1 );

            pRendererCallbacks->SetTextureID( iFaceTextures[ iBottomTextureID ] );
            CallbackRendererColor( iBottomTextureID );

            CalculateNormal2( normal, p4,p3,p2, p1 );
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( raw4.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw4.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p4.x, p4.y, p4.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( raw3.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw3.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( raw2.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->EndTriangle();

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( raw2.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( raw1.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( raw4.x + 0.5 ) ), TextureVCoordToOffsetVCoord( raw4.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p4.x, p4.y, p4.z );

            pRendererCallbacks->EndTriangle();

            OuterFaces[i].GetVertex( p1, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, 0 );
            OuterFaces[i].GetVertex( p2, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, 1 );
            InnerFaces[i].GetVertex( p3, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, 0 );
            InnerFaces[i].GetVertex( p4, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, 1 );

            pRendererCallbacks->SetTextureID( iFaceTextures[ iTopTextureID ] );
            CallbackRendererColor( iTopTextureID );

            CalculateNormal2( normal, p1,p2,p3, p4 );
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw1.x + 0.5 ), TextureVCoordToOffsetVCoord( raw1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw2.x + 0.5 ), TextureVCoordToOffsetVCoord( raw2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw3.x + 0.5 ), TextureVCoordToOffsetVCoord( raw3.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

            pRendererCallbacks->EndTriangle();

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw3.x + 0.5 ), TextureVCoordToOffsetVCoord( raw3.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw4.x + 0.5 ), TextureVCoordToOffsetVCoord( raw4.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p4.x, p4.y, p4.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( raw1.x + 0.5 ), TextureVCoordToOffsetVCoord( raw1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

            pRendererCallbacks->EndTriangle();
        }
    }

    void RotationalPrimitiveImpl::BuildCutFaces( const GLVector3<float> &cutstartouterface, const GLVector3<float> &cutstartinnerface, const GLVector3<float> &cutendouterface, const GLVector3<float> &cutendinnerface )
    {
        //  cout << "BuildCutFaces() showing cut..." << endl;
        CutFaces[0].RemoveAllPoints();
        if( bShowHollow )
        {
            CutFaces[0].AddPoint(cutstartinnerface.x,cutstartinnerface.y,cutstartinnerface.z );
        }
        else
        {
            CutFaces[0].AddPoint( 0, 0, 0 );
        }
        CutFaces[0].AddPoint(cutstartouterface.x,cutstartouterface.y,cutstartouterface.z );
        CutFaces[0].SetTextureStartOffset( fTextureStartU, fTextureStartV );
        CutFaces[0].SetTextureEndOffset( fTextureEndU, fTextureEndV );

        CutFaces[1].RemoveAllPoints();
        CutFaces[1].AddPoint(cutendouterface.x, cutendouterface.y, cutendouterface.z );
        if( bShowHollow )
        {
            CutFaces[1].AddPoint(cutendinnerface.x, cutendinnerface.y, cutendinnerface.z );
        }
        else
        {
            CutFaces[1].AddPoint( 0, 0, 0 );
        }

        CutFaces[1].SetTextureStartOffset( fTextureStartU, fTextureStartV );
        CutFaces[1].SetTextureEndOffset( fTextureEndU, fTextureEndV );
    }

    void RotationalPrimitiveImpl::SetupInnerFaceTextureOffsets( const float fStartSideInnerLength, const float fWholeSideLength, const float fTotalInnerLength )
    {
        int iQuadrant;

        InnerFaces[iFirstOuterFace].SetTextureStartOffset( TextureUCoordToOffsetUCoord( 1.0 - fStartSideInnerLength / fTotalInnerLength ), TextureVCoordToOffsetVCoord( 0.0 ) );
        InnerFaces[iFirstOuterFace].SetTextureEndOffset( TextureUCoordToOffsetUCoord( 1.0 ), TextureVCoordToOffsetVCoord( 1.0 ) );

        float fCumulativeInnerLength = fStartSideInnerLength;
        for( iQuadrant = iFirstOuterFace + 1; iQuadrant < iLastOuterFace; iQuadrant++ )
        {
            InnerFaces[ iQuadrant ].SetTextureEndOffset( TextureUCoordToOffsetUCoord( 1.0 - fCumulativeInnerLength / fTotalInnerLength ), TextureVCoordToOffsetVCoord( 1.0 ) );
            fCumulativeInnerLength += fWholeSideLength;
            InnerFaces[ iQuadrant ].SetTextureStartOffset( TextureUCoordToOffsetUCoord( 1.0 - fCumulativeInnerLength / fTotalInnerLength ), TextureVCoordToOffsetVCoord( 0.0 ) );
        }

        InnerFaces[iLastOuterFace].SetTextureStartOffset( TextureUCoordToOffsetUCoord( 0.0 ), TextureVCoordToOffsetVCoord( 0.0 ) );
        InnerFaces[iLastOuterFace].SetTextureEndOffset( TextureUCoordToOffsetUCoord( 1.0 - fCumulativeInnerLength / fTotalInnerLength ), TextureVCoordToOffsetVCoord( 1.0 ) );
    }

    void RotationalPrimitiveImpl::BuildFaces()
    {
        // cout << endl << endl << "BuildFaces() >>>" << endl;
        if( iCutStart == 0 && iCutEnd == iMaxCut )
        {
            bShowCut = false;
        }
        else
        {
            bShowCut = true;
        }

        if( iHollow == 0 )
        {
            bShowHollow = false;
        }
        else
        {
            bShowHollow = true;
        }

        float fHollowRatio = (float)iHollow / 100;

        GLVector3<float> cutstartinnerface, cutstartouterface;
        GLVector3<float> cutendinnerface, cutendouterface;

        int iCutStartDiagQuadrant, iCutEndDiagQuadrant; //!< quadrant 0 is -45 to +45 degrees, quad 2 is +45 to +135, etc

        //  float fCutStartRatio, fCutEndRatio;
        //  fCutStartRatio = (float)iCutStart / iMaxCut;
        //   fCutEndRatio = (float)iCutEnd / iMaxCut;

        //  cout << "startcut " << iCutStart << " endcut " << iCutEnd << endl;

        iCutStartDiagQuadrant = GetCutQuadrant( iCutStart );
        iCutEndDiagQuadrant = GetCutQuadrant( iCutEnd - 1 );

        GetCutIntersect( cutstartouterface, iCutStart, 0.5 );
        GetCutIntersect( cutendouterface, iCutEnd, 0.5 );

        //  cout << "cutstartouterface " << cutstartouterface << endl;
        //  cout << "cutendouterface " << cutendouterface << endl;

        if( bShowHollow )
        {
            GetCutIntersect( cutstartinnerface, iCutStart, fHollowRatio * 0.5 );
            GetCutIntersect( cutendinnerface, iCutEnd, fHollowRatio * 0.5 );
        }

        //   cout << "cutstartinnerface " << cutstartinnerface << endl;
        //    cout << "cutendinnerface " << cutendinnerface << endl;

        // cout << "DiagQuadrant: start: " << iCutStartDiagQuadrant << " end: " << iCutEndDiagQuadrant << endl;

        if( bShowCut )
        {
            BuildCutFaces( cutstartouterface, cutstartinnerface, cutendouterface, cutendinnerface );
        }

        //int iFaceIndex = 0;
        if( iCutStartDiagQuadrant == iCutEndDiagQuadrant )
        {
            iFirstOuterFace = iLastOuterFace = iCutStartDiagQuadrant;
            //  cout << "Same quad, easy" << endl;
            OuterFaces[0].RemoveAllPoints();
            OuterFaces[0].AddPoint( cutstartouterface.x, cutstartouterface.y, cutstartouterface.z );
            OuterFaces[0].AddPoint( cutendouterface.x, cutendouterface.y, cutendouterface.z );
            OuterFaces[0].SetTextureStartOffset( fTextureStartU, fTextureStartV );
            OuterFaces[0].SetTextureEndOffset( fTextureEndU, fTextureEndV );

            if( bShowHollow )
            {
                InnerFaces[0].RemoveAllPoints();
                InnerFaces[0].AddPoint( cutstartinnerface.x, cutstartinnerface.y, cutstartinnerface.z );
                InnerFaces[0].AddPoint( cutendinnerface.x, cutendinnerface.y, cutendinnerface.z );
                InnerFaces[0].SetTextureStartOffset( fTextureStartU, fTextureStartV );
                InnerFaces[0].SetTextureEndOffset( fTextureEndU, fTextureEndV );
            }
        }
        else
        {
            iFirstOuterFace = iCutStartDiagQuadrant;

            float fTotalInnerLength = 0;

            float fStartSideInnerLength = 0;
            float fWholeSideLength = 0;
            float fEndSideInnerLength = 0;

            float fDummyLength = 0;

            PopulateSingleCutFacePositiveDirection( OuterFaces[iFirstOuterFace], fDummyLength, cutstartouterface, iCutStartDiagQuadrant, 0.5, true );
            if( bShowHollow )
            {
                PopulateSingleCutFacePositiveDirection( InnerFaces[iFirstOuterFace], fStartSideInnerLength, cutstartinnerface, iCutStartDiagQuadrant, fHollowRatio * 0.5, false );
                fTotalInnerLength += fStartSideInnerLength;
            }
            //  iFaceIndex++;

            int iQuadrant = iCutStartDiagQuadrant + 1;
            while( iQuadrant < iCutEndDiagQuadrant )
            {
                //  cout << "iQuadrant = " << iQuadrant << " iCutEndDiagQuadrant = " << iCutEndDiagQuadrant <<  endl;
                PopulateCompleteSide( OuterFaces[ iQuadrant ], fDummyLength, iQuadrant, 0.5, true );
                if( bShowHollow )
                {
                    PopulateCompleteSide( InnerFaces[ iQuadrant ], fWholeSideLength, iQuadrant, fHollowRatio * 0.5, false );
                    fTotalInnerLength += fWholeSideLength;
                }
                //iFaceIndex++;
                iQuadrant++;
            }

            PopulateSingleCutFaceNegativeDirection( OuterFaces[iQuadrant], fDummyLength, cutendouterface, iCutEndDiagQuadrant, 0.5, true );
            if( bShowHollow )
            {
                PopulateSingleCutFaceNegativeDirection( InnerFaces[iQuadrant], fEndSideInnerLength, cutendinnerface, iCutEndDiagQuadrant, fHollowRatio * 0.5, false );
                fTotalInnerLength += fEndSideInnerLength;
            }
            //iFaceIndex++;

            iLastOuterFace = iQuadrant;

            if( bShowHollow )
            {
                SetupInnerFaceTextureOffsets( fStartSideInnerLength, fWholeSideLength, fTotalInnerLength );
            }
        }
        //cout << "BuildFaces() <<<" << endl;
    }

}
