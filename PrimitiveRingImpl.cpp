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
//! \brief PrimitiveRingImpl class function definitions

#define GLVECTOR_IOSTREAM

#include "PrimitiveRingImpl.h"

#include <iostream>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>

namespace FractalSpline
{

   PrimitiveRingImpl::PrimitiveRingImpl( RendererCallbacksClass *pNewRendererCallbacks )
   {
   //   cout << "PrimitiveRingImpl::PrimitiveRingImpl( RendererCallbacksClass *pNewRendererCallbacks )" << endl;
      /*
       ReferenceVertices[0].y = 0.5;
       ReferenceVertices[0].x = -0.2886751;
       ReferenceVertices[0].z = 0;

       ReferenceVertices[1].y = 0;
       ReferenceVertices[1].x = 0.57735;
       ReferenceVertices[1].z = 0;

       ReferenceVertices[2].x = -0.5;
       ReferenceVertices[2].y = -0.2886751;
       ReferenceVertices[2].z = 0;
       */

       ReferenceVertices[0].x = 0.5;
       ReferenceVertices[0].y = -0.5;
       ReferenceVertices[0].z = 0;

       ReferenceVertices[1].x = 0;
       ReferenceVertices[1].y = 0.5;
       ReferenceVertices[1].z = 0;

       ReferenceVertices[2].x = -0.5;
       ReferenceVertices[2].y = -0.5;
       ReferenceVertices[2].z = 0;

      iFirstOuterFace = 0;
      iLastOuterFace = 2;

      iNumberFaces = 3;

      bShowCut = false;
      bShowHollow = false;

      iCutStart = 0;
      iCutEnd = iMaxCut;

      pRendererCallbacks = pNewRendererCallbacks;
      SendRendererCallbacksToCrossSections();

      RotationalExtrusionPath.UpdatePath();
      BuildFaces();
   }

   int PrimitiveRingImpl::GetCutQuadrant( int iCut )
   {
//   	  return ( iCut - 1) / 50;
   	  return ( iCut - 1 ) / 67;
   }

   float PrimitiveRingImpl::GetAngleWithXAxis( float fCutRatio )
   {
       	// return ( fCutRatio - 0.125 )  * 2 * 3.1415927;
       	return ( fCutRatio - 0.125 ) * 2 * 3.1415927;
   }

   void PrimitiveRingImpl::Render()
   {
   	 // cout << "PrimitiveRingImpl::Render()" << endl;

      int i;
      int j;
      GLVector3<float> point;

      int iFaceNum = 0;

      for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
      {
         /*
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
      	 */

      	 iFaceNum = i + 1;

      	 // cout << "textureids: " << iFaceTextures[0] << " " << iFaceTextures[1] << " " << iFaceTextures[2] << endl;
         CallbackRendererColor( iFaceNum );
      	 pRendererCallbacks->SetTextureID( iFaceTextures[ iFaceNum ] );
         ( OuterFaces[i] ).Render( RotationalExtrusionPath );
      }

      if( bShowHollow )
      {
	      for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
	      {
            CallbackRendererColor( 4 );
      	   pRendererCallbacks->SetTextureID( iFaceTextures[4] );
	        ( InnerFaces[i] ).Render( RotationalExtrusionPath );
	      }
      }

      if( bShowCut )
      {
          int iCutStartFace = 5;
          int iCutEndFace = 6;
          if( bShowHollow )
          {
             iCutStartFace = 6;
             iCutEndFace = 7;
         }

         CallbackRendererColor( iCutStartFace );
         pRendererCallbacks->SetTextureID( iFaceTextures[ iCutStartFace ] );
         ( CutFaces[0] ).Render( RotationalExtrusionPath );
         pRendererCallbacks->SetTextureID( iFaceTextures[ iCutEndFace ] );
         CallbackRendererColor( iCutEndFace );
         ( CutFaces[1] ).Render( RotationalExtrusionPath );
      }

     // if( false )
    //  {
      if( !bShowHollow )
      {
      	  RenderEndCapNoHollow( 4, 0 );
      }
      else
      {
      	  RenderEndCapHollow( 5, 0 );
      }
   //   }

     //  Faces[0].Render( LinearExtrusionPath );
   }

}
