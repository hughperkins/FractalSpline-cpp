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
//! \brief Primitive class

#ifndef _PRIMITIVEIMPL_H
#define _PRIMITIVEIMPL_H

#include "fsdllsymbols.h"

#include "PrimitiveInterface.h"

//#include "RendererCallbacks.h"
#include "CrossSection.h"

namespace FractalSpline
{
	// const int ALL_FACES = -1;
	 class Color
	 {
	   public:
	      Color()
	      {
	         r = g= b = 0;
	      }
	      Color( float nr, float ng, float nb)
	      {
	         r = nr; g = ng; b = nb;
	      }
	      const Color &operator =( const Color &src )
	      {
	         r = src.r; g = src.g; b = src.b;
	         return *this;
	      }
	   float r;
	   float g;
	   float b;
	     friend ostream& operator<< ( ostream& os, const Color &color );
	};
	 
   //! Primitive is the base class for FractalSpline Primitive classes
   class EXPCL_FRACTALSPLINE PrimitiveImpl : public PrimitiveInterface
   {
   	public:
      PrimitiveImpl() :
      	  iMaxCut( 200 ),
      	  iMaxFaceTextures( 9 )
      {			
         iCutStart = 0;
         iCutEnd = 200;
         iHollow = 0;
         
         iLevelOfDetail = 16;
         
         fTextureStartU = fTextureStartV = 0.0;
         fTextureEndU = fTextureEndV = 1.0;
         
         int i;
         for( i = 0; i < 10; i++ )
         {
            vFaceColors[i] = Color(1,1,1 );
         }
         
     //    cout << "Primitive::Primitive()" << endl;
      }      
      
      virtual void Render() = 0;  //!< Renders primitive to OpenGL at origin
      virtual void UpdateTransforms() = 0; //!< Update transforms etc internally
      
      virtual void SetLevelOfDetail( const int iNewDetail ) = 0; //!< Set level of details
      virtual void SetTwist( const int iNewTwist ) = 0; //!< Set Twist, in degrees
      virtual void SetShear( const float fNewShear ) = 0; //!< Set Sheer  (float, default is 0.0, relative to side of cube, which is 1.0)
      virtual void SetTopSizeX( const float fNewSheer ) = 0; //!< Set topsizex (float, default is 1.0)
      virtual void SetTopSizeY( const float fNewTopSizeY ) = 0; //!< SetTopsizey (float, default is 1.0)
      virtual void SetCutStart( const int iNewCutStart ) = 0;  //!< Set cut start ( 0 - 199)
      virtual void SetCutEnd( const int iNewCutEnd ) = 0;  //!< set cut end (1 - 200)
      virtual void SetAdvancedCutStart( const int iNewCutStart ){}  //!< Set cut start ( 0 - 199)
      virtual void SetAdvancedCutEnd( const int iNewCutEnd ){}  //!< set cut end (1 - 200)
      virtual void SetHollow( const int iNewHollow ) = 0;  //!< set hollow ( 0 - 95 %)
      
      virtual void SetTextureStartOffset( const float u, const float v ) = 0; //!< Set texture start offset
      virtual void SetTextureEndOffset( const float u, const float v ) = 0; //!< Set texture end offset
      
      virtual void SetTexture( const int iFaceNumber, const int iTextureID ); //!< Set Face Texture on face iFaceNumber; pass in OpenGL texture id; face -1 means all faces
      virtual void SetFaceColor( const int iFaceNumber, const float r, const float g, const float b ); //!< Set Face color on face iFaceNumber; pass in color in rgb; face -1 means all faces
      
      virtual void SetHoleSize( const int iHoleSize ){} // 0 to 199

//      static void SayHello(){ cout << "hello"; }
    //  static void SetDefaultRendererCallbacks( RendererCallbacksClass *pNewRendererCallbacks )
    //  {
         // RendererCallbacks = NewRendererCallbacks;
     //    pDefaultRendererCallbacks = pNewRendererCallbacks;
        // CrossSection::SetDefaultRendererCallbacks( pNewRendererCallbacks );
    //     cout << "NewDefaultRendererCallbacks.GetName() " << pNewRendererCallbacks->GetName() << endl;
         //cout << RendererCallbacks.GetName() << endl;
    //     cout << "pRendererCallbacks->GetName()" << pDefaultRendererCallbacks->GetName() << endl;
         //pRendererCallbacks->StartTriangle();
    //  }

   	protected:   		
      //CrossSection Sections;  //!< All crosssections through object
      
      int iTwist;  //!< Current Twist
      float fSheer; //!< Current Sheer
      int iLevelOfDetail; //!< Current LOD
      float fTopSizeX; //!< Topsize X
      float fTopSizeY; //!< Topsize Y
      int iCutStart;  //!< Cut Start (0 to 200)
      int iCutEnd;  //!< Cut end ( 0 to 200 )
      int iHollow; //!< Hollow % (0 to 95)
      
      const int iMaxCut;  //!< Maximum value for cut
      
      int iFaceTextures[10]; //!< OpenGL Texture IDs
      Color vFaceColors[10];
      const int iMaxFaceTextures;
      
      float fTextureStartU, fTextureStartV;
      float fTextureEndU, fTextureEndV;
      
   	  inline float TextureUCoordToOffsetUCoord( const float NonOffsetUCoord )
   	{
   		  return fTextureStartU + NonOffsetUCoord * ( fTextureEndU - fTextureStartU );
   	}
   	
   	  inline float TextureVCoordToOffsetVCoord( const float NonOffsetVCoord )
	   	{
	   		  return fTextureStartV + NonOffsetVCoord * ( fTextureEndV - fTextureStartV );
	   	}
	   	
	   	virtual void CallbackRendererColor( int iFaceNum )
	   	{
	   	         pRendererCallbacks->SetColor( vFaceColors[ iFaceNum ].r, vFaceColors[ iFaceNum ].g, vFaceColors[ iFaceNum ].b );
	   	 }


   RendererCallbacksClass *pRendererCallbacks;

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
	   
   //static RendererCallbacksClass *pDefaultRendererCallbacks;	   
};

}

#endif // _PRIMITIVEIMPL_H

