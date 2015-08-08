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
//! \brief CrossSection class

#ifndef _CROSSSECTION_H
#define _CROSSSECTION_H

#include <iostream>
using namespace std;

#include "GLMatrix.h"
#include "GLVector.h"

#include "fsdllsymbols.h"

#include "ExtrusionPath.h"
#include "RendererCallbacks.h"

namespace FractalSpline
{
    struct point
    {
        float x;
        float y;
        float z;
    };

    //! CrossSection class is used to store the points of an object's cross section, and to process transforms
    //!
    //! CrossSection class is used to store the points of an object's cross section, and to process transforms
    //!
    //! Usage:
    //! - use AddPoint to add each point of crosssection
    //! - use ApplyRotate, ApplyTranslate, ApplyScale to apply appropriate transforms
    //! - read the transformed coordinates of each point using GetWorldCoordinates
    class EXPCL_FRACTALSPLINE CrossSection
    {

    public:
        CrossSection()
        {
            iNumPoints = 0;
            fTextureStartU = fTextureStartV = 0.0;
            fTextureEndU = fTextureEndV = 1.0;
        }
        void AddPoint( const float x, const float y, const float z );  //!< Adds a new point
        void RemoveAllPoints()
        {
            iNumPoints = 0;
        } //!< Removes all points

        const int GetNumPoints() const
        {
            return iNumPoints;
        } //!< Returns number of points

        //! Sets start texture offset
        void SetTextureStartOffset( const float u, const float v )
        {
            fTextureStartU = u;
            fTextureStartV = v;
        }

        //! Sets end texture offset
        void SetTextureEndOffset( const float u, const float v )
        {
            fTextureEndU = u;
            fTextureEndV = v;
        }

        void Render( const ExtrusionPathClass &ExtrusionPath ) const; //!< Renders cross section with current texture
        void GetVertex( GLVector3<float> &Vertex, const ExtrusionPathClass &ExtrusionPath, const int iPathSliceIndex, const int iPointIndex ) const; //!< Renders cross section with current texture
        const GLVector3<float> &GetPoint( const int iPointNum ) const
        {
            return Points[ iPointNum ];
        } //!< Return untransformed point

        virtual void SetRendererCallbacks( RendererCallbacksClass *pNewRendererCallbacks )
        {
            pRendererCallbacks = pNewRendererCallbacks;
            // cout << pRendererCallbacks->GetName() << endl;
        }
        // static void SetDefaultRendererCallbacks( RendererCallbacksClass *pNewRendererCallbacks )
        // {
        //    pDefaultRendererCallbacks = pNewRendererCallbacks;
        //    cout << pDefaultRendererCallbacks->GetName() << endl;
        // }

    private:

        int iNumPoints;   //!< Number of points currently stored
        GLVector3<float> Points[100];  //!< All points; currently space for 100

        float fTextureStartU, fTextureStartV;
        float fTextureEndU, fTextureEndV;

        RendererCallbacksClass *pRendererCallbacks;
        //   static RendererCallbacksClass *pDefaultRendererCallbacks;
    };

}

#endif // _EXTRUSION_H

