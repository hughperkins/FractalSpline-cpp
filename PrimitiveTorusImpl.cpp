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
//! \brief PrimitiveTorus class function definitions

#define GLVECTOR_IOSTREAM

#include <iostream>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>

#include "GLVector.h"
#include "PrimitiveTorusImpl.h"

namespace FractalSpline
{

	inline void CalculateNormal( GLVector3<float> &normal, const GLVector3<float> &p1,const GLVector3<float> &p2,const GLVector3<float> &p3 )
	{
		  GLVector3<float> vectorab = p2 - p1;
		  GLVector3<float> vectorac = p3 - p1;
		  normal = vectorab.getCross( vectorac ).unit();
		 // cout << "ab: " << vectorab << " ac: " << vectorac << " normal: " << normal << endl;
		 // normal.x = 0;
		 // normal.y = 0;
		 // normal.z = 1;
	}

 	inline void CalculateNormal2( GLVector3<float> &normal, const GLVector3<float> &p1,const GLVector3<float> &p2,const GLVector3<float> &p3,const GLVector3<float> &p4  )
	{
		  GLVector3<float> vectorac = p3 - p1;
		  GLVector3<float> vectorbd = p4 - p2;
		  normal = vectorac.getCross( vectorbd ).unit();
		 // cout << "ab: " << vectorab << " ac: " << vectorac << " normal: " << normal << endl;
		 // normal.x = 0;
		 // normal.y = 0;
		 // normal.z = 1;
	}

  PrimitiveTorusImpl::PrimitiveTorusImpl( RendererCallbacksClass *pNewRendererCallbacks )
   {
   	  iFirstOuterFace = 0;
   	  iLastOuterFace = 0;

   	  iNumberFaces = 1;

   	  bShowCut = false;
   	  bShowHollow = false;



   	  iCutStart = 0;
   	  iCutEnd = iMaxCut;

      pRendererCallbacks = pNewRendererCallbacks;
   	SendRendererCallbacksToCrossSections();

      //cout << "iLastOuterFace = " << iLastOuterFace << endl;
      RotationalExtrusionPath.UpdatePath();
      //cout << "iLastOuterFace = " << iLastOuterFace << endl;
      BuildFaces();
      //cout << "iLastOuterFace = " << iLastOuterFace << endl;
      //exit(1);
   }

   void PrimitiveTorusImpl::GetCutIntersect( GLVector3<float> &IntersectPoint, const int iCut, const float fPrimHalfWidth )
   {
       float fAngle = (float)iCut / (float)iMaxCut * 2 * 3.1415927;
       IntersectPoint.z = 0;
       IntersectPoint.x = fPrimHalfWidth * cos( fAngle );
       IntersectPoint.y = fPrimHalfWidth * sin( fAngle );
   }

   void PrimitiveTorusImpl::BuildFaces()
   {
   	//  cout << endl << endl << "PrimitiveTorus, BuildFaces() >>>" << endl;
   	   if( iCutStart == 0 && iCutEnd == iMaxCut )
   	   {
   	       bShowCut = false;
   	  }
   	  else
   	  {
   	       bShowCut = true;
   	   }

   	   if( iExtrusionStart == 0 && iExtrusionEnd == 200 )
   	   {
   	      bShowEndCaps = false;
   	   }
   	   else
   	   {
   	      bShowEndCaps = true;
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

	 // cout << "startcut " << iCutStart << " endcut " << iCutEnd << endl;

   	  GetCutIntersect( cutstartouterface, iCutStart, 0.5 );
   	  GetCutIntersect( cutendouterface, iCutEnd, 0.5 );

   	 // cout << "cutstartouterface " << cutstartouterface << endl;
   	  //cout << "cutendouterface " << cutendouterface << endl;

   	  if( bShowHollow )
   	  {
   	     GetCutIntersect( cutstartinnerface, iCutStart, fHollowRatio * 0.5 );
   	     GetCutIntersect( cutendinnerface, iCutEnd, fHollowRatio * 0.5 );
   	  }

   	  //cout << "cutstartinnerface " << cutstartinnerface << endl;
   	  //cout << "cutendinnerface " << cutendinnerface << endl;

   	 // cout << "DiagQuadrant: start: " << iCutStartDiagQuadrant << " end: " << iCutEndDiagQuadrant << endl;

       if( bShowCut )
       {
           BuildCutFaces( cutstartouterface, cutstartinnerface, cutendouterface, cutendinnerface );
   	  }

   	  OuterFaces[0].RemoveAllPoints();
      InnerFaces[0].RemoveAllPoints();

      float fAngle = 0;
      float fStartAngle = (float)iCutStart / (float)iMaxCut * 2 * 3.1415927;
      float fEndAngle = (float)iCutEnd / (float)iMaxCut * 2 * 3.1415927;

      GLVector3<float> NextOuterPoint, NextInnerPoint;
      NextOuterPoint.z = 0;
      NextInnerPoint.z = 0;

      int iFacePoint = 0;
      for( iFacePoint = 0; iFacePoint <= iLevelOfDetail; iFacePoint++ )
      {
          fAngle = fStartAngle + (float)iFacePoint / (float)iLevelOfDetail * ( fEndAngle - fStartAngle );
          //cout << "fangle " << fAngle << " startangle " << fStartAngle << " endangle " << fEndAngle << " lod " << iLevelOfDetail << endl;

          NextOuterPoint.x = 0.5 * cos( fAngle );
          NextOuterPoint.y = 0.5 * sin( fAngle );
          OuterFaces[0].AddPoint( NextOuterPoint.x, NextOuterPoint.y, NextOuterPoint.z );
      }

      if( bShowHollow )
      {
        // cout << "bShowHollow " << bShowHollow << endl;
         for( iFacePoint = iLevelOfDetail; iFacePoint >= 0; iFacePoint-- )
         {
             fAngle = fStartAngle + (float)iFacePoint / (float)iLevelOfDetail * ( fEndAngle - fStartAngle );

             NextInnerPoint.x = 0.5 * cos( fAngle ) * (float)iHollow / 100.0;
             NextInnerPoint.y = 0.5 * sin( fAngle ) * (float)iHollow / 100.0;

             // cout << "adding point " << NextInnerPoint << endl;
             InnerFaces[0].AddPoint( NextInnerPoint.x, NextInnerPoint.y, NextInnerPoint.z );
         }
      }

  	  OuterFaces[0].SetTextureStartOffset( fTextureStartU, fTextureStartV );
  	  OuterFaces[0].SetTextureEndOffset( fTextureEndU, fTextureEndV );

  	  InnerFaces[0].SetTextureStartOffset( fTextureStartU, fTextureStartV );
  	  InnerFaces[0].SetTextureEndOffset( fTextureEndU, fTextureEndV );
   }

   void PrimitiveTorusImpl::RenderEndCapNoHollow( const int iBottomTextureID, const int iTopTextureID )
   {
      // cout << "renderendcapcutnohollow" << endl;
      GLVector3<float> p1, p2, rawcentre, transformedcentre;

	   	   GLVector3<float> normal;

      rawcentre.x = 0;
      rawcentre.y = 0.0;
      rawcentre.z = 0.0;

         CallbackRendererColor( iBottomTextureID );
      pRendererCallbacks->SetTextureID( iFaceTextures[ iBottomTextureID ] );
	  pRendererCallbacks->SetNormal( 0, 0, -1 );

      int iFacePoint = 0;
      for( iFacePoint = 0; iFacePoint < iLevelOfDetail; iFacePoint++ )
      {
	   	   OuterFaces[ 0 ].GetVertex( p1, RotationalExtrusionPath, 0, iFacePoint );
	   	   OuterFaces[ 0 ].GetVertex( p2, RotationalExtrusionPath, 0, iFacePoint + 1 );
	   	   RotationalExtrusionPath.GetTransformedVertex( transformedcentre, rawcentre, 0 );

	   	  // cout << " p1 " << p1 << " p2 " << p2<< endl;

            CalculateNormal( normal, p2,p1,transformedcentre );
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

	   	   pRendererCallbacks->StartTriangle();

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - p2.x ), TextureVCoordToOffsetVCoord( p2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - p1.x ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - transformedcentre.x ), TextureVCoordToOffsetVCoord( transformedcentre.x + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( transformedcentre.x, transformedcentre.y, transformedcentre.z );

	   	   pRendererCallbacks->EndTriangle();
      }
	   	   //exit(1);

         CallbackRendererColor( iTopTextureID );
      pRendererCallbacks->SetTextureID( iFaceTextures[ iTopTextureID ] );
      for( iFacePoint = 0; iFacePoint < iLevelOfDetail; iFacePoint++ )
      {
	   	   OuterFaces[ 0 ].GetVertex( p1, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, iFacePoint );
	   	   OuterFaces[ 0 ].GetVertex( p2, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, iFacePoint + 1 );
	   	   RotationalExtrusionPath.GetTransformedVertex( transformedcentre, rawcentre, RotationalExtrusionPath.GetNumberOfTransforms() - 1 );


            CalculateNormal( normal, transformedcentre, p1,p2);
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

	   	   pRendererCallbacks->StartTriangle();

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - transformedcentre.x ), TextureVCoordToOffsetVCoord( transformedcentre.x + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( transformedcentre.x, transformedcentre.y, transformedcentre.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - p1.x ), TextureVCoordToOffsetVCoord( p1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - p2.x ), TextureVCoordToOffsetVCoord( p2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

	   	   pRendererCallbacks->EndTriangle();

      }
   }

   void PrimitiveTorusImpl::RenderEndCapHollow( const int iBottomTextureID, const int iTopTextureID )
   {
     // cout << "renderendcapcutnohollow" << endl;
      GLVector3<float> o1, o2;
      GLVector3<float> i1, i2;

	   	   GLVector3<float> normal;

         CallbackRendererColor( iBottomTextureID );
      pRendererCallbacks->SetTextureID( iFaceTextures[ iBottomTextureID ] );

      int iFacePoint = 0;
      for( iFacePoint = 0; iFacePoint < iLevelOfDetail; iFacePoint++ )
      {
	   	   OuterFaces[ 0 ].GetVertex( o1, RotationalExtrusionPath, 0, iFacePoint );
	   	   OuterFaces[ 0 ].GetVertex( o2, RotationalExtrusionPath, 0, iFacePoint + 1 );
	   	   InnerFaces[ 0 ].GetVertex( i1, RotationalExtrusionPath, 0, iLevelOfDetail - iFacePoint - 1 );
	   	   InnerFaces[ 0 ].GetVertex( i2, RotationalExtrusionPath, 0, iLevelOfDetail - iFacePoint );

	   	   //cout << " o1 " << o1 << " o2 " << o2<< " i1 " << i1 << " i2 " << i2 << endl;

            CalculateNormal2( normal, i2,i1,o2, o1 );
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

 	   pRendererCallbacks->StartTriangle();

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - i2.x ), TextureVCoordToOffsetVCoord( i2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( i2.x, i2.y, i2.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - i1.x ), TextureVCoordToOffsetVCoord( i1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( i1.x, i1.y, i1.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - o2.x ), TextureVCoordToOffsetVCoord( o2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( o2.x, o2.y, o2.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - o1.x ), TextureVCoordToOffsetVCoord( o1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( o1.x, o1.y, o1.z );

	   	   pRendererCallbacks->EndTriangle();

 	   pRendererCallbacks->StartTriangle();

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - i2.x ), TextureVCoordToOffsetVCoord( i2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( i2.x, i2.y, i2.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - i1.x ), TextureVCoordToOffsetVCoord( i1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( i1.x, i1.y, i1.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - o2.x ), TextureVCoordToOffsetVCoord( o2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( o2.x, o2.y, o2.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - o1.x ), TextureVCoordToOffsetVCoord( o1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( o1.x, o1.y, o1.z );

	   	   pRendererCallbacks->EndTriangle();
      }
	   	   //exit(1);

         CallbackRendererColor( iTopTextureID );
      pRendererCallbacks->SetTextureID( iFaceTextures[ iTopTextureID ] );
      for( iFacePoint = 0; iFacePoint < iLevelOfDetail; iFacePoint++ )
      {
	   	   OuterFaces[ 0 ].GetVertex( o1, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, iFacePoint );
	   	   OuterFaces[ 0 ].GetVertex( o2, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, iFacePoint + 1 );
	   	   InnerFaces[ 0 ].GetVertex( i1, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, iLevelOfDetail - iFacePoint - 1 );
	   	   InnerFaces[ 0 ].GetVertex( i2, RotationalExtrusionPath, RotationalExtrusionPath.GetNumberOfTransforms() - 1, iLevelOfDetail - iFacePoint );

	   	   //cout << " o1 " << o1 << " o2 " << o2<< " i1 " << i1 << " i2 " << i2 << endl;

            CalculateNormal2( normal, o1, o2, i1, i2 );
            pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

	   	   pRendererCallbacks->StartTriangle();

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - o1.x ), TextureVCoordToOffsetVCoord( o1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( o1.x, o1.y, o1.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - o2.x ), TextureVCoordToOffsetVCoord( o2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( o2.x, o2.y, o2.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - i1.x ), TextureVCoordToOffsetVCoord( i1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( i1.x, i1.y, i1.z );

	   	   pRendererCallbacks->EndTriangle();

	   	   pRendererCallbacks->StartTriangle();

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - o1.x ), TextureVCoordToOffsetVCoord( o1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( o1.x, o1.y, o1.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - i1.x ), TextureVCoordToOffsetVCoord( i1.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( i1.x, i1.y, i1.z );

           pRendererCallbacks->SetTextureCoord( TextureUCoordToOffsetUCoord( 0.5 - i2.x ), TextureVCoordToOffsetVCoord( i2.y + 0.5 ) );
	   	   pRendererCallbacks->AddVertex( i2.x, i2.y, i2.z );

	   	   pRendererCallbacks->EndTriangle();

      }
   }

   void PrimitiveTorusImpl::Render()
   {
      int i;
      int j;
      GLVector3<float> point;

      int iFaceNum = 0;

      //cout << "Rendering cylinder" << endl;

    //  glPushMatrix();

     // cout << "firstouterface " << iFirstOuterFace << " " << iLastOuterFace << endl;
     // exit(1);
      	 // cout << "textureids: " << iFaceTextures[0] << " " << iFaceTextures[1] << " " << iFaceTextures[2] << endl;
      pRendererCallbacks->SetTextureID( iFaceTextures[ 1 ] );
         CallbackRendererColor( 1 );
      ( OuterFaces[0] ).Render( RotationalExtrusionPath );

    //cout << "bshowhollow " << bShowHollow << endl;

      if( bShowHollow )
      {
         CallbackRendererColor( 2 );
   	   pRendererCallbacks->SetTextureID( iFaceTextures[2] );
        ( InnerFaces[0] ).Render( RotationalExtrusionPath );
      }

      if( bShowCut )
      {
          int iCutStartFace = 3;
          int iCutEndFace = 4;
          if( bShowHollow )
          {
             iCutStartFace = 4;
             iCutEndFace = 5;
         }

         CallbackRendererColor( iCutStartFace );
         pRendererCallbacks->SetTextureID( iFaceTextures[ iCutStartFace ] );
         ( CutFaces[0] ).Render( RotationalExtrusionPath );
         CallbackRendererColor( iCutEndFace );
         pRendererCallbacks->SetTextureID( iFaceTextures[ iCutEndFace ] );
         ( CutFaces[1] ).Render( RotationalExtrusionPath );
      }

      if( bShowEndCaps )
      {
         if( !bShowHollow )
         {
      		RenderEndCapNoHollow( 2, 0 );
         }
         else
         {
             RenderEndCapHollow( 3, 0 );
         }
      }

     //  Faces[0].Render( RotationalExtrusionPath );

   //  glPopMatrix();
   }

}

