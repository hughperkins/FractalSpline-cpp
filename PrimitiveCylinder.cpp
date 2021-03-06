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
//! \brief PrimitiveCylinder class function definitions

#define GLVECTOR_IOSTREAM

#include "PrimitiveCylinder.h"
#include "PrimitiveCylinderImpl.h"
#include "RendererCallbacks.h"

#include <iostream>
using namespace std;

namespace FractalSpline
{

   PrimitiveCylinder::PrimitiveCylinder( RendererCallbacksClass *pNewRendererCallbacks )
   {
      pPrimitive = new PrimitiveCylinderImpl( pNewRendererCallbacks );
   }

   PrimitiveCylinder::~PrimitiveCylinder()
   {
      delete( pPrimitive );
   }
}
