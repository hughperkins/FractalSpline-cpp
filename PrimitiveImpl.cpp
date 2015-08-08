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
//! \brief Primitive class function definitions

#define GLVECTOR_IOSTREAM

#include "PrimitiveImpl.h"

#include <iostream>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>


namespace FractalSpline
{
   //static RendererCallbacksClass TempRendererCallbacks;
   //RendererCallbacksClass &PrimitiveImpl::RendererCallbacks = TempRendererCallbacks;
   //RendererCallbacksClass *PrimitiveImpl::pDefaultRendererCallbacks = 0;

	void PrimitiveImpl::SetTexture( const int iFaceNumber, const int iTextureID )
	{
		 if( iFaceNumber >= 0 && iFaceNumber < iMaxFaceTextures )
		 {
		    iFaceTextures[ iFaceNumber ] = iTextureID;
		  }
		  else if( iFaceNumber == ALL_FACES )  // all faces
		  {
		  	int i;
		  	for( i = 0; i < iMaxFaceTextures; i++ )
		  	{
		  		iFaceTextures[i] = iTextureID;
		  	}
		  }
		  else
		  {
		  	 cout << "FractalSpline::PrimitiveImpl::SetTexture() ERROR: facenumber " << iFaceNumber << " << out of range" << endl;
		  }
	}

   void PrimitiveImpl::SetFaceColor( const int iFaceNumber, const float r, const float g, const float b )
   {
      //cout << " r " << r << " g " << g << " b " << b << endl;
		 if( iFaceNumber >= 0 && iFaceNumber < iMaxFaceTextures )
		 {
		    vFaceColors[ iFaceNumber ] = Color( r,g,b);
		  }
		  else if( iFaceNumber == ALL_FACES )  // all faces
		  {
		  	int i;
		  	for( i = 0; i < iMaxFaceTextures; i++ )
		  	{
		  		//vFaceColors[i] = iTextureID;
		      vFaceColors[ i ].r = r;
		      vFaceColors[ i ].g = g;
		      vFaceColors[ i ].b = b;
		    }
		 // cout << "FractalSpline::PrimitiveImpl::SetFaceColor() " << vFaceColors[ i ] << endl;		  	}
		  }
		  else
		  {
		  	 cout << "FractalSpline::PrimitiveImpl::SetFaceColor() ERROR: facenumber " << iFaceNumber << " << out of range" << endl;
		  }

   }

  // 	float PrimitiveImpl::TextureUCoordToOffsetUCoord( const float NonOffsetUCoord )
  // 	{
  // 		  return fTextureStartU + NonOffsetUCoord * ( fTextureEndU - fTextureStartU );
   //	}

ostream& operator<< ( ostream& os, const Color &color )
{
  	os << "<color r=\"" << color.r << "\" g=\"" << color.g << "\" b=\"" << color.b << "\"/>" ;
	  return os;
}

}
