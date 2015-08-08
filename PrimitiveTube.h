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
//! \brief PrimitiveTube class

#ifndef _PRIMITIVETUBE_H
#define _PRIMITIVETUBE_H

#include "Primitive.h"
#include "RendererCallbacks.h"
//#include "PrimitiveTubeInterface.h"

namespace FractalSpline
{
//! PrimitiveTube will render a torus to OpenGL, applying various advanced transformations, such as twist
class EXPCL_FRACTALSPLINE PrimitiveTube : public Primitive
{
public:
   //PrimitiveTube();
   PrimitiveTube( RendererCallbacksClass *pNewRendererCallbacks );
   ~PrimitiveTube();

  // virtual void Render()
  // {
  // }
};
}

#endif // _PRIMITIVETUBE_H


