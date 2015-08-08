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
//! \brief ExtrusionPath class

#ifndef _EXTRUSIONPATH_H
#define _EXTRUSIONPATH_H

#include "fsdllsymbols.h"

#include "GLVector.h"
#include "GLMatrix.h"

namespace FractalSpline
{

//! ExtrusionPath generate a set of transformation matrices along an extrusion path
//!
//! ExtrusionPath generate a set of transformation matrices along an extrusion path
class EXPCL_FRACTALSPLINE ExtrusionPathClass
{
public:
   ExtrusionPathClass()
   {
      iNumberOfTransforms = 0;
      iTwist = 0;
      fShear = 0;
      fTopSizeX = fTopSizeY = 1;
      iLevelOfDetail = 16;
   }

   virtual void UpdatePath() = 0; //!< Update transforms matrices

   inline const int GetNumberOfTransforms() const{ return iNumberOfTransforms; } //!< Return number of transforms
   //void TransformPoint( GLVector3<float> &TransformedPoint, const GLVector3<float> &PointToTransform, const int iTransformIndex );  //!< Transform PointToTransform according to transform index
 //  void RenderVertex( const GLVector3<float> &PointToRender, const int iTransformIndex ) const;  //!< Render PointToTransform according to transform index
   void GetTransformedVertex( GLVector3<float> &TransformedPoint, const GLVector3<float> &PointToRender, const int iTransformIndex ) const;  //!< get transformed point

   void SetLevelOfDetail( const int NewDetail ){ iLevelOfDetail = NewDetail; } //!< Set level of details
   void SetTwist( const int iNewTwist ){ iTwist = iNewTwist; } //!< Set Twist, in degrees
   void SetShear( const float fNewShear ){ fShear = fNewShear; } //!< Set Sheer
   void SetTopSizeX( const float fNewTopSizeX ){ fTopSizeX = fNewTopSizeX; } //!< Set Sheer
   void SetTopSizeY( const float fNewTopSizeY ){ fTopSizeY = fNewTopSizeY; } //!< Set Sheer

protected:
   int iNumberOfTransforms;  //!< Number of Transforms
   GLMatrix<float> Transforms[32];  //!< All transforms

   int iLevelOfDetail; //!< Current LOD

   int iTwist;  //!< Current Twist
   float fShear; //!< Current Sheer
   float fTopSizeX;
   float fTopSizeY;
};

}

#endif // _EXTRUSIONPATH_H

