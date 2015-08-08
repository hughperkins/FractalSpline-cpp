// Copyright Hugh Perkins 2005
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.
//
// You should have received a copy of the GNU General Public License along
// with this program in the file licence.txt; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-
// 1307 USA
// You can find the licence also on the web at:
// http://www.opensource.org/licenses/gpl-license.php
//

//! \file
//! \brief RotationalPrimitiveImpl class

#ifndef _ROTATIONALPRIMITIVEIMPL_H
#define _ROTATIONALPRIMITIVEIMPL_H

#include "fsdllsymbols.h"

#include "PrimitiveImpl.h"
#include "RotationalExtrusionPath.h"

namespace FractalSpline
{
//! PrimitivePrism will render a Prism to OpenGL, applying various advanced transformations, such as twist
class EXPCL_FRACTALSPLINE RotationalPrimitiveImpl : public PrimitiveImpl
{
public:
   RotationalPrimitiveImpl()
   {
   	  SetHoleSize( 50 );
   	  
      RotationalExtrusionPath.SetLevelOfDetail( iLevelOfDetail );
      
      iExtrusionStart = 0;
      iExtrusionEnd = 200;
   }
   
   virtual void Render() = 0;
   
   virtual void SetHoleSize( const int iHoleSize ); // 0 to 199
   
   virtual void SetLevelOfDetail( const int iNewDetail ); //!< Set level of details
   virtual void SetTwist( const int iNewTwist ); //!< Set Twist, in degrees
   virtual void SetShear( const float fNewShear ); //!< Set Shear
   virtual void SetTopSizeX( const float fNewShear ); //!< Set Shear
   virtual void SetTopSizeY( const float fNewTopSizeY );
   
   virtual void SetCutStart( const int iNewCutStart );
   virtual void SetCutEnd( const int iNewCutEnd );

   virtual void SetAdvancedCutStart( const int iNewCutStart );
   virtual void SetAdvancedCutEnd( const int iNewCutEnd );
   
   virtual void SetHollow( const int iNewHollow );
   
   virtual void SetTextureStartOffset( const float u, const float v );
   virtual void SetTextureEndOffset( const float u, const float v );
     
   void UpdateTransforms(); //!< Update transforms
   
protected:
   virtual void SendRendererCallbacksToCrossSections()
   {
      int i;
      for( i = 0; i < 4; i++ )
      {
         OuterFaces[i].SetRendererCallbacks( pRendererCallbacks );
         InnerFaces[i].SetRendererCallbacks( pRendererCallbacks );
      }
      CutFaces[0].SetRendererCallbacks( pRendererCallbacks );
      CutFaces[1].SetRendererCallbacks( pRendererCallbacks );
   }
	
   GLVector3<float> ReferenceVertices[4]; //!< reference vertices of our prim, to build the crosssections etc
   
   CrossSection OuterFaces[4]; //!< section for each extruded outer face
   CrossSection InnerFaces[4]; //!< section for each extruded inner face (hollow)
   
   int iFirstOuterFace; //!< If we're cutting, this might not be 0
   int iLastOuterFace; //!< If we're cutting, this might not be iNumberFaces
   
   CrossSection CutFaces[2]; //!< Two cut faces
   int iNumberFaces; //!< Number of faces on base primitive (set in derived primitive's constructor)
   
   bool bShowCut;
   bool bShowHollow;
   bool bShowEndCaps;
   
   int iExtrusionStart; //!< 0 to 200
   int iExtrusionEnd; //!< 0 to 200
   
   int iHoleSize; // 0 to 199
   
   RotationalExtrusionPathClass RotationalExtrusionPath;

   virtual void RenderEndCapNoHollow( const int iBottomTextureID, const int iTopTextureID );
   //virtual void RenderEndCapCutNoHollow( const int iBottomTextureID, const int iTopTextureID );
   virtual void RenderEndCapHollow( const int iBottomTextureID, const int iTopTextureID );
   
   virtual void BuildCutFaces( const GLVector3<float> &cutstartouterface, const GLVector3<float> &cutstartinnerface, const GLVector3<float> &cutendouterface, const GLVector3<float> &cutendinnerface );
   virtual void BuildFaces();  //!< Populates CutFaces, after cut change
   
   virtual int GetCutQuadrant( int iCut ) = 0; //!< REturns 0, 1, 2,3 etc according to cutratio (basically, which side of crossection it intersects)
   virtual void GetCutIntersect( GLVector3<float> &IntersectPoint, const int iCut, const float fCubeHalfWidth ); //!< Gets intersection of cut radius with edge of x-section

   virtual void SetupInnerFaceTextureOffsets( const float fStartSideInnerLength, const float fWholeSideLength, const float fTotalInnerLength );
   
   virtual void PopulateSingleCutFacePositiveDirection( CrossSection &Face, float &fLength, GLVector3<float> &CutPoint, int iQuadrant, float fHalfCubeWidth, bool bOuter );
   virtual void PopulateSingleCutFaceNegativeDirection( CrossSection &Face, float &fLength, GLVector3<float> &CutPoint, int iQuadrant, float fHalfCubeWidth, bool bOuter );
   virtual void PopulateCompleteSide( CrossSection &Face, float &fLength, int iQuadrant, float fHalfCubeWidth, bool bOuter );

   virtual void NormalizeQuadrant( int &iQuadrant )
   {
       if( iQuadrant >= iNumberFaces )
   	  {
   	  	iQuadrant -= iNumberFaces;
   	  }
   	  if( iQuadrant < 0 )
   	  {
   	  	iQuadrant += iNumberFaces;
   	  }
   }
   virtual float GetAngleWithXAxis( float fCutRatio ) = 0;
};

}

#endif // _ROTATIONALPRIMITIVEIMPL_H


