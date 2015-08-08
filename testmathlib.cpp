#ifdef _WIN32
#include <windows.h> 
#endif

#define GLVECTOR_IOSTREAM

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>
using namespace std;

#include "GLMatrix.h"
#include "GLVector.h"

void DumpMatrix( GLMatrix<float> matrix )
{
	int i, j;
	  for( i = 0; i < 4; i++ )
	  {
	  	for( j = 0; j < 4; j++ )
	  	{
	  	 cout << matrix[i * 4 + j ] << " ";
	  	}
	  	cout << endl;
	  }
}

int main( int argc, char *argv[] )
{
	  GLMatrix<float> mytestmatrix;
	  mytestmatrix.loadIdentity();
	  //cout << mytestmatrix << endl;
	  
	  DumpMatrix( mytestmatrix );
	  mytestmatrix.applyRotate( 90, 1,0,0);
	  DumpMatrix( mytestmatrix );
	  
	  GLVector3<float> v1, v2, result;
	  v1.x = 1;
	  v1.y = 0.5;
	  v1.z = 0.5;
	  v2.x = 0;
	  v2.y = 1;
	  v2.z = 0;
	  result = v1.getCross( v2 );
	  
	  cout << v1 << endl;
	  cout << v2 << endl;
	  cout << result << endl;
}






