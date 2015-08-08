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

#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "fsdllsymbols.h"

#include "PrimitiveInterface.h"

namespace FractalSpline
{
//	 const int ALL_FACES = -1;

   //! Primitive is the base class for FractalSpline Primitive classes
   class EXPCL_FRACTALSPLINE Primitive : public PrimitiveInterface
   {
      public:
      Primitive();
      ~Primitive();
      inline void Render()
      {
         pPrimitive->Render();
      }

      inline void UpdateTransforms()
      {
         pPrimitive->UpdateTransforms();
      }

      inline void SetLevelOfDetail( const int iNewDetail )
      {
         pPrimitive->SetLevelOfDetail(iNewDetail);
      }
      inline void SetTwist( const int iNewTwist )
      {
         pPrimitive->SetTwist(iNewTwist);
      }
      inline void SetShear( const float fNewSheer )
      {
         pPrimitive->SetShear(fNewSheer);
      }
      inline void SetTopSizeX( const float fNewTopSizeX )
      {
         pPrimitive->SetTopSizeX(fNewTopSizeX);
      }
      inline void SetTopSizeY( const float fNewTopSizeY )
      {
         pPrimitive->SetTopSizeY(fNewTopSizeY);
      }
      inline void SetCutStart( const int iNewCutStart )
      {
         pPrimitive->SetCutStart(iNewCutStart);
      }
      inline void SetCutEnd( const int iNewCutEnd )
      {
         pPrimitive->SetCutEnd(iNewCutEnd);
      }
      inline void SetAdvancedCutStart( const int iNewCutStart )
      {
         pPrimitive->SetAdvancedCutStart(iNewCutStart);
      }
      inline void SetAdvancedCutEnd( const int iNewCutEnd )
      {
         pPrimitive->SetAdvancedCutEnd(iNewCutEnd);
      }
      inline void SetHollow( const int iNewHollow )
      {
         pPrimitive->SetHollow(iNewHollow);
      }

      inline void SetTextureStartOffset( const float u, const float v )
      {
         pPrimitive->SetTextureStartOffset(u,v);
      }
      inline void SetTextureEndOffset( const float u, const float v )
      {
         pPrimitive->SetTextureEndOffset(u,v);
      }

      inline void SetTexture( const int iFaceNumber, const int iTextureID )
      {
         pPrimitive->SetTexture(iFaceNumber,iTextureID);
      }
      inline void SetFaceColor( const int iFaceNumber, const float r, const float g, const float b )
      {
         pPrimitive->SetFaceColor(iFaceNumber,r,g,b);
      }
      inline void SetHoleSize( const int iHoleSize )
      {
         pPrimitive->SetHoleSize( iHoleSize );
      }

protected:
    PrimitiveInterface *pPrimitive;
};

}

#endif // _PRIMITIVE_H

