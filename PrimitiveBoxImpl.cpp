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
//! \brief PrimitiveBox class function definitions

#define GLVECTOR_IOSTREAM

#include "PrimitiveBoxImpl.h"

#include <iostream>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>

namespace FractalSpline
{

   PrimitiveBoxImpl::PrimitiveBoxImpl( RendererCallbacksClass *pNewRendererCallbacks )
   {
    //  cout << "PrimitiveBoxImpl::PrimitiveBox( RendererCallbacksClass *pNewRendererCallbacks )" << endl;
       ReferenceVertices[0].x = 0.5;
       ReferenceVertices[0].y = -0.5;
       ReferenceVertices[0].z = 0;

       ReferenceVertices[1].x = 0.5;
       ReferenceVertices[1].y = 0.5;
       ReferenceVertices[1].z = 0;

       ReferenceVertices[2].x = -0.5;
       ReferenceVertices[2].y = 0.5;
       ReferenceVertices[2].z = 0;

       ReferenceVertices[3].x = -0.5;
       ReferenceVertices[3].y = -0.5;
       ReferenceVertices[3].z = 0;

   	  iFirstOuterFace = 0;
   	  iLastOuterFace = 3;

   	  iNumberFaces = 4;

   	  bShowCut = false;
   	  bShowHollow = false;

   	  iCutStart = 0;
   	  iCutEnd = iMaxCut;

   	pRendererCallbacks = pNewRendererCallbacks;
   	SendRendererCallbacksToCrossSections();

      LinearExtrusionPath.UpdatePath();
      BuildFaces();
   }

   int PrimitiveBoxImpl::GetCutQuadrant( int iCut )
   {
   	  return ( iCut - 1) / 50;
   }

   float PrimitiveBoxImpl::GetAngleWithXAxis( float fCutRatio )
   {
       	 return ( fCutRatio - 0.125 )  * 2 * 3.1415927;
   }

   void PrimitiveBoxImpl::Render()
   {
   	 // cout << "PrimitiveBoxImpl::Render()" << endl;

      int i;
      int j;
      GLVector3<float> point;

      int iFaceNum = 0;

      for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
      {
      	 switch( i )
      	 {
            case 0:
               iFaceNum = 2;
               break;

            case 1:
               iFaceNum = 3;
               break;

            case 2:
               iFaceNum = 4;
               break;

            case 3:
               iFaceNum = 1;
               break;

      	 }
      	 // cout << "textureids: " << iFaceTextures[0] << " " << iFaceTextures[1] << " " << iFaceTextures[2] << endl;
         CallbackRendererColor( iFaceNum );
      	 pRendererCallbacks->SetTextureID( iFaceTextures[ iFaceNum ] );
      	// ( OuterFaces[i] ).SetAddVertexCallback( pAddVertex );
         ( OuterFaces[i] ).Render( LinearExtrusionPath );
      }

      if( bShowHollow )
      {
	      for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
	      {
      	   pRendererCallbacks->SetTextureID( iFaceTextures[5] );
         CallbackRendererColor(  5 );
         //pRendererCallbacks->SetColor( vFaceColors[ 5 ].r, vFaceColors[ 5 ].g, vFaceColors[ 5 ].b );
      	 //( InnerFaces[i] ).SetAddVertexCallback( pAddVertex );
	        ( InnerFaces[i] ).Render( LinearExtrusionPath );
	      }
      }

      if( bShowCut )
      {
          int iCutStartFace = 6;
          int iCutEndFace = 7;
          if( bShowHollow )
          {
             iCutStartFace = 7;
             iCutEndFace = 8;
         }

         pRendererCallbacks->SetTextureID( iFaceTextures[ iCutStartFace ] );
         pRendererCallbacks->SetColor( vFaceColors[ iCutStartFace ].r, vFaceColors[ iCutStartFace ].g, vFaceColors[ iCutStartFace ].b );
      	// ( CutFaces[0] ).SetAddVertexCallback( pAddVertex );
         ( CutFaces[0] ).Render( LinearExtrusionPath );
         pRendererCallbacks->SetTextureID( iFaceTextures[ iCutEndFace ] );
         pRendererCallbacks->SetColor( vFaceColors[ iCutEndFace ].r, vFaceColors[ iCutEndFace ].g, vFaceColors[ iCutEndFace ].b );
      	// ( CutFaces[1] ).SetAddVertexCallback( pAddVertex );
         ( CutFaces[1] ).Render( LinearExtrusionPath );
      }

      if( !bShowHollow )
      {
      	if( !bShowCut )
      	{
      		  RenderEndCapNoCutNoHollow( 5, 0 );
		    }
		    else
		    {
		    	 RenderEndCapCutNoHollow( 5, 0 );
		    }
      }
      else
      {
      	  RenderEndCapHollow( 6, 0 );
      }

     //  Faces[0].Render( LinearExtrusionPath );
   }

}
