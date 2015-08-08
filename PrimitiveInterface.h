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
//! \brief Primitive class

#ifndef _PRIMITIVEINTERFACE_H
#define _PRIMITIVEINTERFACE_H

#include "fsdllsymbols.h"

namespace FractalSpline
{
	 const int ALL_FACES = -1;
	 
   //! Primitive is the base class for FractalSpline Primitive classes
   class EXPCL_FRACTALSPLINE PrimitiveInterface
   {
   	public:     
      virtual void Render() = 0;  //!< Renders primitive to OpenGL at origin
      virtual void UpdateTransforms() = 0; //!< Update transforms etc internally
      
      virtual void SetLevelOfDetail( const int iNewDetail ) = 0; //!< Set level of details
      
      virtual void SetTwist( const int iNewTwist ) = 0; //!< Set Twist, in degrees
      virtual void SetShear( const float fNewShear ) = 0; //!< Set Sheer  (float, default is 0.0, relative to side of cube, which is 1.0)
      virtual void SetTopSizeX( const float fNewSheer ) = 0; //!< Set topsizex (float, default is 1.0)
      virtual void SetTopSizeY( const float fNewTopSizeY ) = 0; //!< SetTopsizey (float, default is 1.0)
      virtual void SetCutStart( const int iNewCutStart ) = 0;  //!< Set cut start ( 0 - 199)
      virtual void SetCutEnd( const int iNewCutEnd ) = 0;  //!< set cut end (1 - 200)
      virtual void SetAdvancedCutStart( const int iNewCutStart ) = 0;  //!< Set cut start ( 0 - 199)
      virtual void SetAdvancedCutEnd( const int iNewCutEnd ) = 0;  //!< set cut end (1 - 200)
      virtual void SetHollow( const int iNewHollow ) = 0;  //!< set hollow ( 0 - 95 %)
      
      virtual void SetHoleSize( const int iHoleSize ) = 0; // 0 to 199
      
      virtual void SetTextureStartOffset( const float u, const float v ) = 0; //!< Set texture start offset
      virtual void SetTextureEndOffset( const float u, const float v ) = 0; //!< Set texture end offset
      
      virtual void SetTexture( const int iFaceNumber, const int iTextureID ) = 0; //!< Set Face Texture on face iFaceNumber; pass in OpenGL texture id; face -1 means all faces
      virtual void SetFaceColor( const int iFaceNumber, const float r, const float g, const float b ) = 0; //!< Set Face color on face iFaceNumber; pass in color in rgb; face -1 means all faces

};

}

#endif // _PRIMITIVEINTERFACE_H

