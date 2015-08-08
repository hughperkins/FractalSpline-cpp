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
//! \brief RendererCallbacksOpenGL.h file

#ifndef _RENDERERCALLBACKSOPENGL_H
#define _RENDERERCALLBACKSOPENGL_H

#include <GL/gl.h> 
#include <GL/glu.h> 

#include "fsdllsymbols.h"

#include "RendererCallbacks.h"

namespace FractalSpline
{

class __declspec(dllexport) RendererCallbacksOpenGLClass : public RendererCallbacksClass
{
public:
   RendererCallbacksOpenGLClass()
   {
   }

   virtual char *GetName() const{ return "RendererCallbacksOpenGLClass"; }
   	
   inline virtual void AddVertex( float x, float y, float z )
   {
    //  cout << " RendererCallbacksOpenGLClass::AddVertex()" << endl;
      glVertex3f( x,y,z );
   }
   inline virtual void SetNormal( float x, float y, float z )
   {
      glNormal3f( x,y, z );
   }
   inline virtual void SetTextureCoord( float u, float v )
   {
      glTexCoord2f( u, v );
   }
   inline virtual void StartTriangle()
   {
      glBegin( GL_TRIANGLES );
   }
   inline virtual void EndTriangle()
   {
      glEnd();
   }
   inline virtual void SetTextureID( int iTexture )
   {
      glBindTexture(GL_TEXTURE_2D, iTexture );
   }
   inline virtual void SetColor( float r, float g, float b )
   {
      float mcolor[4];

      mcolor[0] = r;
      mcolor[1] = g;
      mcolor[2] = b;
      mcolor[3] = 1.0;
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);   
   }

protected:
};

} // namespace FractalSpline

#endif // _RENDERERCALLBACKSOPENGL_H

