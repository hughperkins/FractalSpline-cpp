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
// You can contact me at hughperkins at gmail for more information.
//

//! \file
//! \brief LinearPrimitiveImpl class function definitions

#define GLVECTOR_IOSTREAM

#include "LinearPrimitiveImpl.h"

#include <iostream>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>

namespace FractalSpline
{

    void LinearPrimitiveImpl::UpdateTransforms()
    {
        LinearExtrusionPath.UpdatePath();
        BuildFaces();
    }

    void LinearPrimitiveImpl::SetLevelOfDetail( const int iNewDetail )
    {
        iLevelOfDetail = iNewDetail;
        LinearExtrusionPath.SetLevelOfDetail( iNewDetail );
    }

    void LinearPrimitiveImpl::SetTwist( const int iNewTwist )
    {
        //  cout << "LinearPrimitiveImpl::SetTwist iNewTwist " << iNewTwist << endl;
        LinearExtrusionPath.SetTwist( iNewTwist );
    }

    void LinearPrimitiveImpl::SetShear( const float fNewShear )
    {
        LinearExtrusionPath.SetShear( fNewShear );
    }

    void LinearPrimitiveImpl::SetTopSizeX( const float fNewSheer )
    {
        LinearExtrusionPath.SetTopSizeX( fNewSheer );
    }

    void LinearPrimitiveImpl::SetTopSizeY( const float fNewTopSizeY )
    {
        LinearExtrusionPath.SetTopSizeY( fNewTopSizeY );
    }

    void LinearPrimitiveImpl::SetHollow( const int iNewHollow )
    {
        // cout << "LinearPrimitiveImpl::SetHollow iNewHollow " << iNewHollow << endl;
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

    void LinearPrimitiveImpl::SetTextureStartOffset( const float u, const float v )
    {
        fTextureStartU = u;
        fTextureStartV = v;
    }

    void LinearPrimitiveImpl::SetTextureEndOffset( const float u, const float v )
    {
        fTextureEndU = u;
        fTextureEndV = v;
    }

    void LinearPrimitiveImpl::SetCutStart( const int iNewCutStart )
    {
        //  cout << "LinearPrimitiveImpl::SetCutStart new cut " << iNewCutStart << endl;
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

        BuildFaces();
    }

    void LinearPrimitiveImpl::SetCutEnd( const int iNewCutEnd )
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

        //cout << "SetCutEnd() " << iCutStart << " " << iCutEnd << endl;

        BuildFaces();
    }

    //! BAsically we check the quadrant, then intersect it, then we just intersect the radius with the appropriate face
    void LinearPrimitiveImpl::GetCutIntersect( GLVector3<float> &IntersectPoint, const int iCut, const float fCubeHalfWidth )
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
    }

    void LinearPrimitiveImpl::PopulateSingleCutFacePositiveDirection( CrossSection &Face, float &fLength, GLVector3<float> &CutPoint, int iQuadrant, float fHalfCubeWidth, bool bOuter )
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

    void LinearPrimitiveImpl::PopulateSingleCutFaceNegativeDirection( CrossSection &Face, float &fLength, GLVector3<float> &CutPoint, int iQuadrant, float fHalfCubeWidth, bool bOuter )
    {
        // cout << "PopulateSingleCutFaceNegativeDirection, quadrant " << iQuadrant << endl;
        NormalizeQuadrant( iQuadrant );

        Face.RemoveAllPoints();

        GLVector3<float> StartPoint;
        GLVector3<float> EndPoint;

        StartPoint.z = EndPoint.z = 0;

        EndPoint = CutPoint;

        StartPoint = ReferenceVertices[ iQuadrant ] * fHalfCubeWidth / 0.5;
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

    void LinearPrimitiveImpl::PopulateCompleteSide( CrossSection &Face, float &fLength, int iQuadrant, float fHalfCubeWidth, bool bOuter )
    {
        NormalizeQuadrant( iQuadrant );

        // cout << "Populatecompletesdie, quadrant " << iQuadrant << endl;

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

    void LinearPrimitiveImpl::RenderEndCapCutNoHollow( const int iBottomTextureID, const int iTopTextureID )
    {
        // cout << "RenderCutNoHollow" << endl;
        int i;
        for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
        {
            GLVector3<float> p1,p2;

            OuterFaces[i].GetVertex( p1, LinearExtrusionPath, 0, 0 );
            OuterFaces[i].GetVertex( p2, LinearExtrusionPath, 0, 1 );

            pRendererCallbacks->SetColor( vFaceColors[ iBottomTextureID ].r, vFaceColors[ iBottomTextureID ].g, vFaceColors[ iBottomTextureID ].b );
            pRendererCallbacks->SetTextureID( iFaceTextures[ iBottomTextureID ] );

            pRendererCallbacks->SetNormal( 0, 0, -1 );
            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( p2.x + 0.5, p2.y + 0.5 );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->SetTextureCoord( p1.x + 0.5, p1.y + 0.5 );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

            pRendererCallbacks->SetTextureCoord( 0.5, 0.5 );
            pRendererCallbacks->AddVertex( 0, 0, -0.5 );

            pRendererCallbacks->EndTriangle();

            OuterFaces[i].GetVertex( p1, LinearExtrusionPath, LinearExtrusionPath.GetNumberOfTransforms() - 1, 0 );
            OuterFaces[i].GetVertex( p2, LinearExtrusionPath, LinearExtrusionPath.GetNumberOfTransforms() - 1, 1 );

            pRendererCallbacks->SetColor( vFaceColors[ iTopTextureID ].r, vFaceColors[ iTopTextureID ].g, vFaceColors[ iTopTextureID ].b );
            pRendererCallbacks->SetTextureID( iFaceTextures[ iTopTextureID ] );
            pRendererCallbacks->SetNormal( 0, 0, 1 );
            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p1.x + 0.5 ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p2.x + 0.5 ), TextureVCoordToOffsetVCoord( p2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 ), TextureVCoordToOffsetVCoord( 0.5 ) );
            pRendererCallbacks->AddVertex( 0, 0, 0.5 );

            pRendererCallbacks->EndTriangle();
        }
    }

    void LinearPrimitiveImpl::RenderEndCapNoCutNoHollow( const int iBottomTextureID, const int iTopTextureID )
    {
        RenderEndCapHollow( iBottomTextureID, iTopTextureID );
        /*
         //cout << "Nocutnohollow" << endl;
               GLVector3<float> p1,p2,p3,p4;

        int i;


         glBindTexture(GL_TEXTURE_2D, iFaceTextures[ iBottomTextureID ] );
         glNormal3f( 0, 0, -1 );
         glBegin( GL_POLYGON );

         for( i = iLastOuterFace; i >= 0; i-- )
         {
            OuterFaces[i].GetVertex( p1, LinearExtrusionPath, 0, 0 );
            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( p1.x + 0.5 ) ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );
         }
          glEnd();


         glBindTexture(GL_TEXTURE_2D, iFaceTextures[ iTopTextureID ] );
          glNormal3f( 0, 0, 1 );
          pRendererCallbacks->StartTriangle();
         for( i = 0; i <= iLastOuterFace; i++ )
         {
            OuterFaces[i].GetVertex( p1, LinearExtrusionPath, LinearExtrusionPath.GetNumberOfTransforms() - 1, 0 );
            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p1.x + 0.5 ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );
         }
          glEnd();
        */
    }

    void LinearPrimitiveImpl::BuildCutFaces( const GLVector3<float> &cutstartouterface, const GLVector3<float> &cutstartinnerface, const GLVector3<float> &cutendouterface, const GLVector3<float> &cutendinnerface )
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

    void LinearPrimitiveImpl::SetupInnerFaceTextureOffsets( const float fStartSideInnerLength, const float fWholeSideLength, const float fTotalInnerLength )
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

    void LinearPrimitiveImpl::BuildFaces()
    {
        //cout << "BuildFaces iHollow = " << iHollow << endl;
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

        //   cout << "cutstartouterface " << cutstartouterface << endl;
        //   cout << "cutendouterface " << cutendouterface << endl;

        if( bShowHollow )
        {
            GetCutIntersect( cutstartinnerface, iCutStart, fHollowRatio * 0.5 );
            GetCutIntersect( cutendinnerface, iCutEnd, fHollowRatio * 0.5 );
        }

        //   cout << "cutstartinnerface " << cutstartinnerface << endl;
        //    cout << "cutendinnerface " << cutendinnerface << endl;

        //   cout << "DiagQuadrant: start: " << iCutStartDiagQuadrant << " end: " << iCutEndDiagQuadrant << endl;

        if( bShowCut )
        {
            BuildCutFaces( cutstartouterface, cutstartinnerface, cutendouterface, cutendinnerface );
        }

        //int iFaceIndex = 0;
        if( iCutStartDiagQuadrant == iCutEndDiagQuadrant )
        {
            iFirstOuterFace = iLastOuterFace = iCutStartDiagQuadrant;
            // cout << "Same quad, easy" << endl;
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
            OuterFaces[iFirstOuterFace].SetTextureStartOffset( fTextureStartU, fTextureStartV );
            OuterFaces[iFirstOuterFace].SetTextureEndOffset( fTextureEndU, fTextureEndV );
            if( bShowHollow )
            {
                PopulateSingleCutFacePositiveDirection( InnerFaces[iFirstOuterFace], fStartSideInnerLength, cutstartinnerface, iCutStartDiagQuadrant, fHollowRatio * 0.5, false );
                InnerFaces[iFirstOuterFace].SetTextureStartOffset( fTextureStartU, fTextureStartV );
                InnerFaces[iFirstOuterFace].SetTextureEndOffset( fTextureEndU, fTextureEndV );
                fTotalInnerLength += fStartSideInnerLength;
            }
            //  iFaceIndex++;

            int iQuadrant = iCutStartDiagQuadrant + 1;
            while( iQuadrant < iCutEndDiagQuadrant )
            {
                //cout << "iQuadrant = " << iQuadrant << " iCutEndDiagQuadrant = " << iCutEndDiagQuadrant <<  endl;
                PopulateCompleteSide( OuterFaces[ iQuadrant ], fDummyLength, iQuadrant, 0.5, true );
                OuterFaces[iQuadrant].SetTextureStartOffset( fTextureStartU, fTextureStartV );
                OuterFaces[iQuadrant].SetTextureEndOffset( fTextureEndU, fTextureEndV );

                if( bShowHollow )
                {
                    PopulateCompleteSide( InnerFaces[ iQuadrant ], fWholeSideLength, iQuadrant, fHollowRatio * 0.5, false );
                    InnerFaces[iQuadrant].SetTextureStartOffset( fTextureStartU, fTextureStartV );
                    InnerFaces[iQuadrant].SetTextureEndOffset( fTextureEndU, fTextureEndV );
                    fTotalInnerLength += fWholeSideLength;
                }

                //iFaceIndex++;
                iQuadrant++;
            }

            PopulateSingleCutFaceNegativeDirection( OuterFaces[iQuadrant], fDummyLength, cutendouterface, iCutEndDiagQuadrant, 0.5, true );
            OuterFaces[iQuadrant].SetTextureStartOffset( fTextureStartU, fTextureStartV );
            OuterFaces[iQuadrant].SetTextureEndOffset( fTextureEndU, fTextureEndV );
            if( bShowHollow )
            {
                PopulateSingleCutFaceNegativeDirection( InnerFaces[iQuadrant], fEndSideInnerLength, cutendinnerface, iCutEndDiagQuadrant, fHollowRatio * 0.5, false );
                InnerFaces[iQuadrant].SetTextureStartOffset( fTextureStartU, fTextureStartV );
                InnerFaces[iQuadrant].SetTextureEndOffset( fTextureEndU, fTextureEndV );
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


    void LinearPrimitiveImpl::RenderEndCapHollow( const int iBottomTextureID, const int iTopTextureID )
    {
        int i;
        for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
        {
            GLVector3<float> p1,p2,p3,p4;


            OuterFaces[i].GetVertex( p1, LinearExtrusionPath, 0, 0 );
            OuterFaces[i].GetVertex( p2, LinearExtrusionPath, 0, 1 );
            InnerFaces[i].GetVertex( p3, LinearExtrusionPath, 0, 0 );
            InnerFaces[i].GetVertex( p4, LinearExtrusionPath, 0, 1 );

            pRendererCallbacks->SetTextureID( iFaceTextures[ iBottomTextureID ] );
            pRendererCallbacks->SetNormal( 0, 0, -1 );

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( p4.x + 0.5 ) ), TextureVCoordToOffsetVCoord( p4.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p4.x, p4.y, p4.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( p3.x + 0.5 ) ), TextureVCoordToOffsetVCoord( p3.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( p2.x + 0.5 ) ), TextureVCoordToOffsetVCoord( p2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->EndTriangle();

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( p4.x + 0.5 ) ), TextureVCoordToOffsetVCoord( p4.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p4.x, p4.y, p4.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( p2.x + 0.5 ) ), TextureVCoordToOffsetVCoord( p2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 1 - ( p1.x + 0.5 ) ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );
            pRendererCallbacks->EndTriangle();

            OuterFaces[i].GetVertex( p1, LinearExtrusionPath, LinearExtrusionPath.GetNumberOfTransforms() - 1, 0 );
            OuterFaces[i].GetVertex( p2, LinearExtrusionPath, LinearExtrusionPath.GetNumberOfTransforms() - 1, 1 );
            InnerFaces[i].GetVertex( p3, LinearExtrusionPath, LinearExtrusionPath.GetNumberOfTransforms() - 1, 0 );
            InnerFaces[i].GetVertex( p4, LinearExtrusionPath, LinearExtrusionPath.GetNumberOfTransforms() - 1, 1 );

            pRendererCallbacks->SetTextureID( iFaceTextures[ iTopTextureID ] );
            pRendererCallbacks->SetNormal( 0, 0, 1 );

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p1.x + 0.5 ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p2.x + 0.5 ), TextureVCoordToOffsetVCoord( p2.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p3.x + 0.5 ), TextureVCoordToOffsetVCoord( p3.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

            pRendererCallbacks->EndTriangle();

            pRendererCallbacks->StartTriangle();

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p1.x + 0.5 ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p3.x + 0.5 ), TextureVCoordToOffsetVCoord( p3.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

            pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( p4.x + 0.5 ), TextureVCoordToOffsetVCoord( p4.y + 0.5 ) );
            pRendererCallbacks->AddVertex( p4.x, p4.y, p4.z );
            pRendererCallbacks->EndTriangle();
        }
    }
}
