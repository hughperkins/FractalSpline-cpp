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

//#ifdef _WIN32
//#include <windows.h>
//#endif

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>
using namespace std;

#include <stdlib.h>

//#include <GL/gl.h>
//#include <GL/glu.h>

#define GLVECTOR_IOSTREAM

#include "RendererCallbacks.h"

#include "CrossSection.h"

namespace FractalSpline
{
    //RendererCallbacksClass *CrossSection::pDefaultRendererCallbacks = 0;

    inline void CalculateNormal( GLVector3<float> &normal, const GLVector3<float> &p1,const GLVector3<float> &p2,const GLVector3<float> &p3 )
    {
        GLVector3<float> vectorab = p2 - p1;
        GLVector3<float> vectorac = p3 - p1;
        normal = vectorab.getCross( vectorac ).unit();
        // cout << "ab: " << vectorab << " ac: " << vectorac << " normal: " << normal << endl;
        // normal.x = 0;
        // normal.y = 0;
        // normal.z = 1;
    }

    inline void CalculateNormal2( GLVector3<float> &normal, const GLVector3<float> &p1,const GLVector3<float> &p2,const GLVector3<float> &p3,const GLVector3<float> &p4  )
    {
        GLVector3<float> vectorac = p3 - p1;
        GLVector3<float> vectorbd = p4 - p2;
        normal = vectorac.getCross( vectorbd ).unit();
        // cout << "ab: " << vectorab << " ac: " << vectorac << " normal: " << normal << endl;
        // normal.x = 0;
        // normal.y = 0;
        // normal.z = 1;
    }

    void CrossSection::AddPoint( const float x, const float y, const float z )
    {
        Points[iNumPoints].x = x;
        Points[iNumPoints].y = y;
        Points[iNumPoints].z = z;
        iNumPoints++;
    }

    void CrossSection::GetVertex( GLVector3<float> &Vertex, const ExtrusionPathClass &ExtrusionPath, const int iPathSliceIndex, const int iPointIndex ) const
    {
        ExtrusionPath.GetTransformedVertex( Vertex, Points[iPointIndex], iPathSliceIndex );
    }

    void CrossSection::Render( const ExtrusionPathClass &ExtrusionPath ) const
    {
        int i;
        int j;
        //GLVector3<float> point;

        // Get grid of points. p1 - p4 are the quad we are rendering
        //

        GLVector3<float> p1,p2,p3,p4;

        //cout << "Render() ExtrusionPath.GetNumberOfTransforms() = " << ExtrusionPath.GetNumberOfTransforms() << " iNumPoints = " << iNumPoints << endl;
        for( i = 0; i < ExtrusionPath.GetNumberOfTransforms() - 1; i++ )
        {
            for( j = 0; j < iNumPoints - 1; j++ )
            {
                //cout << "Render() i = " << i << " j = " << j << endl;

                ExtrusionPath.GetTransformedVertex( p1, Points[j], i );
                ExtrusionPath.GetTransformedVertex( p2, Points[j + 1], i );
                ExtrusionPath.GetTransformedVertex( p3, Points[j + 1], i + 1 );
                ExtrusionPath.GetTransformedVertex( p4, Points[j], i + 1 );

                // cout << "p1 " << p1 << " p2 " << p2 << " p3 " << p3 << " p4 " << p4 << endl;

                GLVector3<float> normal;

                float x, y, xinc, yinc;

                // cout << "texstartu " << fTextureStartU << " endu " << fTextureEndU << endl;
                // cout << "texstartv " << fTextureStartV << " endv " << fTextureEndV << endl;
                // exit(0);
                x = fTextureStartU + (float)j / ( iNumPoints - 1 );
                xinc = (float)( fTextureEndU - fTextureStartU ) / (iNumPoints - 1 );

                y = fTextureStartV + (float)i / ( ExtrusionPath.GetNumberOfTransforms() - 1 );
                yinc = (float)( fTextureEndV - fTextureStartV ) / (ExtrusionPath.GetNumberOfTransforms() - 1 );

                //cout << "x = " << x << " xinc = " << xinc << " y = " << y << " yinc = " << yinc << endl;

                CalculateNormal2( normal, p1,p2,p3, p4 );
                pRendererCallbacks->SetNormal( normal.x, normal.y, normal.z );

                pRendererCallbacks->StartTriangle();
                pRendererCallbacks->SetTextureCoord( x, y );
                pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

                //            CalculateNormal2( normal, p1,p2,p3, p4 );
                //          glNormal3f( normal.x, normal.y, normal.z );
                pRendererCallbacks->SetTextureCoord( x + xinc, y );
                pRendererCallbacks->AddVertex( p2.x, p2.y, p2.z );

                //        CalculateNormal2( normal, p1,p2,p3, p4 );
                //       glNormal3f( normal.x, normal.y, normal.z );
                pRendererCallbacks->SetTextureCoord( x + xinc, y + yinc );
                pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

                //ExtrusionPath.RenderVertex( Points[j], i );
                //ExtrusionPath.RenderVertex( Points[j + 1], i );
                //ExtrusionPath.RenderVertex( Points[j + 1], i + 1 );
                //ExtrusionPath.RenderVertex( Points[j], i + 1 );
                pRendererCallbacks->EndTriangle();

                pRendererCallbacks->StartTriangle();
                pRendererCallbacks->SetTextureCoord( x, y );
                pRendererCallbacks->AddVertex( p1.x, p1.y, p1.z );

                pRendererCallbacks->SetTextureCoord( x + xinc, y + yinc );
                pRendererCallbacks->AddVertex( p3.x, p3.y, p3.z );

                pRendererCallbacks->SetTextureCoord( x, y + yinc );
                pRendererCallbacks->AddVertex( p4.x, p4.y, p4.z );

                pRendererCallbacks->EndTriangle();
                //glEnd();
                //exit(0);
            }
        }
    }

}



