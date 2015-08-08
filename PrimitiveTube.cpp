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
//! \brief PrimitiveTube class function definitions

#define GLVECTOR_IOSTREAM

#include "PrimitiveTube.h"
#include "PrimitiveTubeImpl.h"
#include "RendererCallbacks.h"

#include <iostream>
using namespace std;

namespace FractalSpline
{

   PrimitiveTube::PrimitiveTube( RendererCallbacksClass *pNewRendererCallbacks )
   {
      pPrimitive = new PrimitiveTubeImpl( pNewRendererCallbacks );
   }

   PrimitiveTube::~PrimitiveTube()
   {
      delete( pPrimitive );
   }
}