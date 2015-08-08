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
//! \brief LinearExtrusionPath class member definitions

#define GLMATRIX_IOSTREAM

#include <iostream>
using namespace std;

#include "LinearExtrusionPath.h"

namespace FractalSpline
{

   void LinearExtrusionPathClass::UpdatePath()
   {
      iNumberOfTransforms = iLevelOfDetail;
      int i;
      float fRatio;
      for( i = 0; i < iNumberOfTransforms; i++ )
      {
      	 fRatio = (float)i / (float)( iNumberOfTransforms - 1 );
         Transforms[i].loadIdentity();
         Transforms[i].applyRotate( fRatio * (float)iTwist, 0, 0, 1 );
         Transforms[i].applyScale( 1 + fRatio * (fTopSizeX - 1), 1 + fRatio * ( fTopSizeY - 1 ), 1 );
         Transforms[i].applyTranslate( fRatio * fShear, 0, fRatio - 0.5 );
         //cout << "UpdatePath i = " << i << endl;
         //cout << Transforms[i] << endl;
      }
   }
}
