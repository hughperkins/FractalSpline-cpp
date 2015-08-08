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
//! \brief RotationalExtrusionPath class

#ifndef _ROTATIONALEXTRUSIONPATH_H
#define _ROTATIONALEXTRUSIONPATH_H

#include "fsdllsymbols.h"

#include "ExtrusionPath.h"

namespace FractalSpline
{
   
//! RotationalExtrusionPath generate a set of transformation matrices along a linear extrusion path
//!
//! RotationalExtrusionPath generate a set of transformation matrices along a linear extrusion path
class EXPCL_FRACTALSPLINE RotationalExtrusionPathClass : public ExtrusionPathClass
{
public:
   RotationalExtrusionPathClass()
   {
      fCutStartAngle = 0;
      fCutEndAngle = 2 * 3.1415927;
      fExtrusionRadius = 1.0;
      fRadialSectionScale = 1.0;
      
      UpdatePath();
   }
   
   virtual void UpdatePath(); //!< Set level of details
   
   virtual void SetCutStartAngle( const float fNewCutStartAngle ){ fCutStartAngle = fNewCutStartAngle; }
   virtual void SetCutEndAngle( const float fNewCutEndAngle ){ fCutEndAngle = fNewCutEndAngle; }   
   virtual void SetRadius( const float fNewExtrusionRadius ){ fExtrusionRadius = fNewExtrusionRadius; }   
   virtual void SetRadialSectionScale( const float fNewRadialSectionScale ){ fRadialSectionScale = fNewRadialSectionScale; }
   
protected:
   float fCutStartAngle;
   float fCutEndAngle;
   float fExtrusionRadius;
   float fRadialSectionScale;
};

}

#endif // _ROTATIONALEXTRUSIONPATH_H

