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
//! \brief PrimitivePrismImpl class function definitions

#define GLVECTOR_IOSTREAM

#include "PrimitivePrismImpl.h"

#include <iostream>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>

namespace FractalSpline
{

    PrimitivePrismImpl::PrimitivePrismImpl( RendererCallbacksClass *pNewRendererCallbacks )
    {
        ReferenceVertices[0].x = 0.5;
        ReferenceVertices[0].y = -0.2886751;
        ReferenceVertices[0].z = 0;

        ReferenceVertices[1].x = 0;
        ReferenceVertices[1].y = 0.57735;
        ReferenceVertices[1].z = 0;

        ReferenceVertices[2].x = -0.5;
        ReferenceVertices[2].y = -0.2886751;
        ReferenceVertices[2].z = 0;

        iFirstOuterFace = 0;
        iLastOuterFace = 2;

        iNumberFaces = 3;

        bShowCut = false;
        bShowHollow = false;

        iCutStart = 0;
        iCutEnd = iMaxCut;

        pRendererCallbacks = pNewRendererCallbacks;
        SendRendererCallbacksToCrossSections();

        //cout << "iLastOuterFace = " << iLastOuterFace << endl;
        LinearExtrusionPath.UpdatePath();
        //cout << "iLastOuterFace = " << iLastOuterFace << endl;
        BuildFaces();
        //cout << "iLastOuterFace = " << iLastOuterFace << endl;
        //exit(1);
    }

    int PrimitivePrismImpl::GetCutQuadrant( int iCut )
    {
        return iCut / 67;
    }

    void PrimitivePrismImpl::Render()
    {
        int i;
        int j;
        GLVector3<float> point;

        int iFaceNum = 0;

        //   glPushMatrix();
        //   glScalef( 1.0 / 0.8660254, 1.0, 1.0 );
        //   glRotatef( - 90, 0, 0, 1 );

        // cout << "firstouterface " << iFirstOuterFace << " " << iLastOuterFace << endl;
        // exit(1);
        for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
        {
            /*
            switch( i )
            {
               case 0:
                  iFaceNum = 2;
                  break;

               case 1:
                  iFaceNum = 3;
                  break;

               case 2:
                  iFaceNum = 1;
                  break;

            }
            */

            iFaceNum = i + 1;

            // cout << "textureids: " << iFaceTextures[0] << " " << iFaceTextures[1] << " " << iFaceTextures[2] << endl;
            pRendererCallbacks->SetTextureID( iFaceTextures[ iFaceNum ] );
            CallbackRendererColor( iFaceNum );
            ( OuterFaces[i] ).Render( LinearExtrusionPath );
        }

        if( bShowHollow )
        {
            for( i = iFirstOuterFace; i <= iLastOuterFace; i++ )
            {
                CallbackRendererColor( 4 );
                pRendererCallbacks->SetTextureID( iFaceTextures[4] );
                ( InnerFaces[i] ).Render( LinearExtrusionPath );
            }
        }

        if( bShowCut )
        {
            int iCutStartFace = 5;
            int iCutEndFace = 6;
            if( bShowHollow )
            {
                iCutStartFace = 6;
                iCutEndFace = 7;
            }

            pRendererCallbacks->SetTextureID( iFaceTextures[ iCutStartFace ] );
            CallbackRendererColor( iCutStartFace );
            ( CutFaces[0] ).Render( LinearExtrusionPath );
            pRendererCallbacks->SetTextureID( iFaceTextures[ iCutEndFace ] );
            CallbackRendererColor( iCutEndFace );
            ( CutFaces[1] ).Render( LinearExtrusionPath );
        }

        if( !bShowHollow )
        {
            if( !bShowCut )
            {
                RenderEndCapNoCutNoHollow( 4, 0 );
            }
            else
            {
                RenderEndCapCutNoHollow( 5, 0 );
            }
        }
        else
        {
            RenderEndCapHollow( 5, 0 );
        }

        //  Faces[0].Render( LinearExtrusionPath );

        //glPopMatrix();
    }

}

