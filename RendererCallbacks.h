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
//! \brief RendererCallbacks.h file

#ifndef _RENDERERCALLBACKS_H
#define _RENDERERCALLBACKS_H

#include <iostream>
using namespace std;

#include "fsdllsymbols.h"

namespace FractalSpline
{

class EXPCL_FRACTALSPLINE RendererCallbacksClass
{
public:
   RendererCallbacksClass()
   {
       pStartTriangle = 0;
       pEndTriangle = 0;
       pAddVertex = 0;
       pSetTextureCoord = 0;
       pSetNormal = 0;
   }
   
   virtual char *GetName() const{ return "RendererCallbacksClass"; }
   	
	virtual void SetStartTriangleCallback( void (* StartTriangle )() )
	{
		pStartTriangle = StartTriangle;
	}
	
	virtual void SetEndTriangleCallback( void (* EndTriangle ) () )
	{
		pEndTriangle = EndTriangle;
	}
	
	virtual void SetTextureCoordCallback( void (* SetTextureCoord)( float u, float v ) )
	{
		pSetTextureCoord = SetTextureCoord;
	}

	virtual void SetNormalCallback( void (* SetNormal)( float x, float y, float z ) )
	{
		pSetNormal = SetNormal;
	}

	virtual void SetAddVertexCallback( void (* AddVertex)( float x, float y, float z ) )
	{
		pAddVertex = AddVertex;
	}

   inline virtual void AddVertex( float x, float y, float z )
   {
   	  cout << "RendererCallbacksClass::AddVertex()" << endl;
   	  exit(1);
      if( pAddVertex != 0 )
      {
         (*pAddVertex)( x, y, z );
      }
   }
   inline virtual void SetNormal( float x, float y, float z )
   {
      if( pSetNormal != 0 )
      {
         (*pSetNormal)( x, y, z );
      }
   }
   inline virtual void SetTextureCoord( float u, float v )
   {
      if( pSetTextureCoord != 0 )
      {
         (*pSetTextureCoord)( u, v );
      }
   }
   inline virtual void StartTriangle()
   {
      if( pStartTriangle != 0 )
      {
         (*pStartTriangle)();
      }
   }
   inline virtual void EndTriangle()
   {
      if( pEndTriangle != 0 )
      {
         (*pEndTriangle)();
      }
   }

   inline virtual void SetTextureID( int iTexture )
   {
   }

   inline virtual void SetColor( float r, float g, float b )
   {
   }

protected:
   void (* pStartTriangle )();
   void (* pEndTriangle ) ();
   void (* pAddVertex)( float x, float y, float z );
   void (* pSetTextureCoord)( float u, float v );
   void (* pSetNormal)( float x, float y, float z );	
};

} // namespace FractalSpline

#endif // _RENDERERCALLBACKS_H

