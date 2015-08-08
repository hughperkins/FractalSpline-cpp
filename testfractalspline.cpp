#define TIXML_USE_STL

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>
using namespace std;

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glut.h>

#include "TextureLoader.h"

//#include "GLMatrix.h"
//#include "CrossSection.h"
//#include "Primitive.h"
#include "PrimitiveBox.h"
#include "PrimitivePrism.h"
#include "PrimitiveCylinder.h"
#include "PrimitiveTorus.h"
#include "PrimitiveTube.h"
#include "PrimitiveRing.h"

#include "RendererCallbacks.h"
#include "RendererCallbacksOpenGL.h"

#include "RotationalExtrusionPath.h"
#include "LinearExtrusionPath.h"
#include "CrossSection.h"

static int win;
int iWindowHeight, iWindowWidth;

int TextureIds[13];

int LoadGLTextures()           // Load Bitmaps And Convert To Textures
{

    int i;
    char Filename[64] = "";
    for( i = 0; i <= 9; i++ )
    {
        sprintf( Filename, "%i.tga", i );
        cout << "loading " << Filename << endl;
        Tartan::LoadTextureFile( TextureIds[i], Filename );
    }
    cout << i << endl;

    Tartan::LoadTextureFile( TextureIds[10], "Compressed.tga" );
    Tartan::LoadTextureFile( TextureIds[11], "Background.tga" );
    Tartan::LoadTextureFile( TextureIds[12], "USMC.pcx" );

    cout << "EndLoadGLTextures()" << endl;
    return true;            // Return The Status
}

FractalSpline::CrossSection NewCrossSection;
FractalSpline::RotationalExtrusionPathClass RotationalExtrusionPath;
FractalSpline::LinearExtrusionPathClass LinearExtrusionPath;

#define SHOWBOXES
//#define RINGSONLY

const int iStressNumX = 10;

#ifndef RINGSONLY
FractalSpline::PrimitiveBox *boxes[iStressNumX];
FractalSpline::PrimitivePrism *prisms[iStressNumX];
FractalSpline::PrimitiveCylinder *cylinders[iStressNumX];
#endif

class Spacer
{
public:
    Spacer()
    {
        cout << endl;
    }
};

FractalSpline::RendererCallbacksOpenGLClass RendererCallbacks;
Spacer a;

#ifndef RINGSONLY
#ifdef SHOWBOXES
FractalSpline::PrimitiveBox mybox( &RendererCallbacks );
Spacer c;
FractalSpline::PrimitiveBox mybox2( &RendererCallbacks );
Spacer d;
FractalSpline::PrimitiveBox mybox3( &RendererCallbacks );
Spacer e;
FractalSpline::PrimitiveBox mybox4( &RendererCallbacks );
FractalSpline::PrimitiveBox mybox5( &RendererCallbacks );
FractalSpline::PrimitiveBox mybox6( &RendererCallbacks );

FractalSpline::PrimitiveBox mybox7( &RendererCallbacks );
FractalSpline::PrimitiveBox mybox8( &RendererCallbacks );
FractalSpline::PrimitiveBox mybox9( &RendererCallbacks );
FractalSpline::PrimitiveBox mybox10( &RendererCallbacks );
FractalSpline::PrimitiveBox mybox11( &RendererCallbacks );
FractalSpline::PrimitiveBox mybox12( &RendererCallbacks );
#endif

FractalSpline::PrimitiveBox somebox( &RendererCallbacks );

FractalSpline::PrimitivePrism prism1( &RendererCallbacks );
FractalSpline::PrimitivePrism prism2( &RendererCallbacks );
FractalSpline::PrimitivePrism prism3( &RendererCallbacks );

FractalSpline::PrimitiveCylinder cyl1( &RendererCallbacks );
FractalSpline::PrimitiveCylinder cyl2( &RendererCallbacks );
FractalSpline::PrimitiveCylinder cyl3( &RendererCallbacks );
FractalSpline::PrimitiveCylinder cyl4( &RendererCallbacks );
FractalSpline::PrimitiveCylinder cyl5( &RendererCallbacks );

FractalSpline::PrimitiveTorus tor1( &RendererCallbacks );
FractalSpline::PrimitiveTorus tor2( &RendererCallbacks );
FractalSpline::PrimitiveTorus tor3( &RendererCallbacks );
FractalSpline::PrimitiveTorus tor4( &RendererCallbacks );
FractalSpline::PrimitiveTorus tor5( &RendererCallbacks );

FractalSpline::PrimitiveCylinder cyl6( &RendererCallbacks );
FractalSpline::PrimitiveCylinder cyl7( &RendererCallbacks );

FractalSpline::PrimitivePrism prism4( &RendererCallbacks );
FractalSpline::PrimitivePrism prism5( &RendererCallbacks );
FractalSpline::PrimitivePrism prism6( &RendererCallbacks );

FractalSpline::PrimitiveTube tube1( &RendererCallbacks );
FractalSpline::PrimitiveTube tube2( &RendererCallbacks );
FractalSpline::PrimitiveTube tube3( &RendererCallbacks );
FractalSpline::PrimitiveTube tube4( &RendererCallbacks );
#endif

FractalSpline::PrimitiveRing ring1( &RendererCallbacks );
FractalSpline::PrimitiveRing ring2( &RendererCallbacks );
FractalSpline::PrimitiveRing ring3( &RendererCallbacks );
FractalSpline::PrimitiveRing ring4( &RendererCallbacks );
FractalSpline::PrimitiveRing ring5( &RendererCallbacks );

int iTwist = 0;

void AddVertex( float x, float y, float z )
{
    //  glVertex3f( x, y, z );
}

void SetupCrossSection()
{
    int i;

    cout << "Setupcrosssection()" << endl;

    cout << RendererCallbacks.GetName() << endl;

    //FractalSpline::RendererCallbacksClass &RenderByRef = RendererCallbacks;

    //cout << RenderByRef.GetName() << endl;

    //RendererCallbacks.SetAddVertexCallback( &AddVertex );
    //  FractalSpline::Primitive::SetDefaultRendererCallbacks( &RendererCallbacks );
    // exit(1);

#ifndef RINGSONLY

    NewCrossSection.AddPoint( 0,0,0);
    NewCrossSection.AddPoint( 1,0,0);
    NewCrossSection.AddPoint( 1,0,1);
    NewCrossSection.AddPoint( 0,0,1);
    NewCrossSection.AddPoint( 0,0,0);

    RotationalExtrusionPath.SetTwist(30);
    RotationalExtrusionPath.UpdatePath();

#ifdef SHOWBOXES

    mybox.SetTexture(FractalSpline::ALL_FACES, TextureIds[10] );
    mybox.SetCutStart( 100 );
    mybox.SetCutEnd( 175 );
    mybox.SetTwist( 90 );
    mybox.SetLevelOfDetail(4);
    mybox.SetHollow( 30 );
    mybox.UpdateTransforms();

    mybox2.SetTexture(FractalSpline::ALL_FACES, TextureIds[10] );
    mybox2.SetTextureStartOffset( 0.5, 0.5 );
    mybox2.UpdateTransforms();

    mybox3.SetTexture(FractalSpline::ALL_FACES, TextureIds[11] );
    mybox3.SetTextureStartOffset( 0.5, 0.5 );
    mybox3.SetTextureEndOffset( 5.5, 5.5 );
    mybox3.SetShear(0.0);
    mybox3.SetHollow(70);
    mybox3.SetCutStart( 30 );
    mybox3.UpdateTransforms();

    mybox4.SetTexture(FractalSpline::ALL_FACES, TextureIds[11] );
    mybox4.SetTextureEndOffset( 0.5, 0.5 );
    mybox4.SetTopSizeX( 0.5 );
    mybox4.SetCutStart( 30 );
    mybox4.UpdateTransforms();

    mybox5.SetTexture(FractalSpline::ALL_FACES, TextureIds[12] );
    mybox5.SetTopSizeY( 0.5 );
    mybox5.SetHollow(70 );
    mybox5.UpdateTransforms();


    mybox6.SetTexture(FractalSpline::ALL_FACES, TextureIds[12] );
    mybox6.SetTextureStartOffset( 0.25, 0.25 );
    mybox6.SetTextureEndOffset( 0.75, 0.75 );
    mybox6.UpdateTransforms();
#endif
#endif

    for( i = 0; i <= 8; i++ )
    {
#ifndef RINGSONLY
#ifdef SHOWBOXES
        mybox7.SetTexture( i, TextureIds[i] );
        mybox8.SetTexture( i, TextureIds[i] );
        mybox9.SetTexture( i, TextureIds[i] );
        mybox10.SetTexture( i, TextureIds[i] );
        mybox11.SetTexture( i, TextureIds[i] );
        mybox12.SetTexture( i, TextureIds[i] );
#endif

        prism1.SetTexture( i, TextureIds[i] );
        prism2.SetTexture( i, TextureIds[i] );
        prism3.SetTexture( i, TextureIds[i] );

        cyl1.SetTexture( i, TextureIds[i] );
        cyl2.SetTexture( i, TextureIds[i] );
        cyl3.SetTexture( i, TextureIds[i] );
        cyl4.SetTexture( i, TextureIds[i] );
        cyl5.SetTexture( i, TextureIds[i] );

        tor1.SetTexture( i, TextureIds[i] );
        tor2.SetTexture( i, TextureIds[i] );
        tor3.SetTexture( i, TextureIds[i] );
        tor4.SetTexture( i, TextureIds[i] );
        tor5.SetTexture( i, TextureIds[i] );

        tube1.SetTexture( i, TextureIds[i] );
        tube2.SetTexture( i, TextureIds[i] );
        tube3.SetTexture( i, TextureIds[i] );
        tube4.SetTexture( i, TextureIds[i] );

#endif

        ring1.SetTexture( i, TextureIds[i] );
        ring2.SetTexture( i, TextureIds[i] );
        ring3.SetTexture( i, TextureIds[i] );
        ring4.SetTexture( i, TextureIds[i] );
        ring5.SetTexture( i, TextureIds[i] );
    }

#ifndef RINGSONLY
#ifdef SHOWBOXES
    mybox7.SetHollow(70);
    mybox7.UpdateTransforms();

    mybox8.SetHollow(70);
    mybox8.SetCutStart(100);
    mybox8.UpdateTransforms();

    mybox9.SetHollow(10);
    mybox9.UpdateTransforms();

    mybox10.SetHollow(10);
    mybox10.SetCutStart(30 );
    mybox10.UpdateTransforms();

    mybox11.SetHollow(70);
    mybox11.SetCutStart(50);
    mybox11.SetTextureStartOffset( 0.25, 0 );
    mybox11.SetTextureEndOffset( 0.75, 0 );
    mybox11.UpdateTransforms();
#endif

    prism1.SetHollow(70);
    prism1.SetTwist(90);
    prism1.UpdateTransforms();

    prism2.SetCutStart(70);
    prism2.UpdateTransforms();

    prism2.SetFaceColor( FractalSpline::ALL_FACES, 0, 0, 1 );

    cyl1.SetCutStart( 80 );
    cyl1.UpdateTransforms();

    cyl2.SetCutStart( 80 );
    cyl2.SetTwist( 80 );
    cyl2.SetHollow( 70 );
    cyl2.UpdateTransforms();

    cyl3.SetHollow( 80 );
    cyl3.UpdateTransforms();

    cyl4.SetCutStart( 80 );
    cyl4.SetHollow( 10 );
    cyl4.UpdateTransforms();

    cyl4.SetFaceColor( FractalSpline::ALL_FACES, 0, 0, 1 );

    tor1.SetCutStart( 30 );
    tor1.SetAdvancedCutStart( 70 );
    tor1.UpdateTransforms();

    tor2.SetCutStart( 70 );
    tor2.SetHollow( 30 );
    tor2.SetHoleSize( 10 );
    tor2.UpdateTransforms();

    tor3.SetCutStart( 70 );
    tor3.SetAdvancedCutStart( 70 );
    tor3.SetHollow( 30 );
    tor3.SetHoleSize( 0 );
    tor3.UpdateTransforms();

    tor4.SetHoleSize( 100 );
    tor4.UpdateTransforms();

    tor5.SetHoleSize( 190 );
    tor5.UpdateTransforms();

    tor5.SetFaceColor( FractalSpline::ALL_FACES, 0, 0, 1 );

    tube1.SetCutStart( 120 );
    tube1.UpdateTransforms();

    tube2.SetCutStart( 120 );
    tube2.SetHollow( 20 );
    tube2.UpdateTransforms();

    tube3.SetCutStart( 120 );
    tube3.SetAdvancedCutStart( 70 );
    tube3.UpdateTransforms();

    tube4.SetCutStart( 120 );
    tube4.SetAdvancedCutStart( 70 );
    tube4.SetHoleSize( 100 );
    tube4.SetHollow( 50 );
    tube4.UpdateTransforms();

    tube4.SetFaceColor( FractalSpline::ALL_FACES, 0, 0, 1 );

#endif

    ring1.SetCutStart( 120 );
    ring1.UpdateTransforms();

    ring2.SetCutStart( 120 );
    ring2.SetHollow( 20 );
    ring2.UpdateTransforms();

    ring3.SetCutStart( 120 );
    ring3.SetAdvancedCutStart( 70 );
    ring3.UpdateTransforms();

    ring4.SetCutStart( 120 );
    ring4.SetAdvancedCutStart( 70 );
    ring4.SetHoleSize( 100 );
    ring4.SetHollow( 50 );
    ring4.UpdateTransforms();


    ring5.SetHoleSize( 150 );
    ring5.UpdateTransforms();
    ring5.SetFaceColor( FractalSpline::ALL_FACES, 0, 0, 1 );

#ifndef RINGSONLY

    somebox.SetTexture( FractalSpline::ALL_FACES, TextureIds[ 10 ] );
    somebox.SetCutStart( 70 );
    somebox.SetHollow( 90 );
    somebox.UpdateTransforms();

    int j;
    for( i = 0; i < iStressNumX; i++ )
    {
        boxes[i] = new FractalSpline::PrimitiveBox(&RendererCallbacks);
        prisms[i] = new FractalSpline::PrimitivePrism(&RendererCallbacks);
        cylinders[i] = new FractalSpline::PrimitiveCylinder(&RendererCallbacks);

        boxes[i]->SetTwist( 30 );
        prisms[i]->SetTwist( 30 );
        cylinders[i]->SetTwist( 30 );

        boxes[i]->SetCutStart( 30 );
        prisms[i]->SetCutStart( 30 );
        cylinders[i]->SetCutStart( 30 );

        boxes[i]->SetHollow( 30 );
        prisms[i]->SetHollow( 30 );
        cylinders[i]->SetHollow( 30 );

        boxes[i]->SetFaceColor( FractalSpline::ALL_FACES, 1, 0, 0 );
        prisms[i]->SetFaceColor( FractalSpline::ALL_FACES, 0, 1, 0 );
        cylinders[i]->SetFaceColor( FractalSpline::ALL_FACES, 0, 0, 1 );

        boxes[i]->UpdateTransforms();
        prisms[i]->UpdateTransforms();
        cylinders[i]->UpdateTransforms();

        for( j = 0; j <= 8; j++ )
        {
            boxes[i]->SetTexture( j, TextureIds[j] );
            prisms[i]->SetTexture( j, TextureIds[j] );
            cylinders[i]->SetTexture( j, TextureIds[j] );
        }
    }
#endif
}

float zRot = 0;
float yRot = 0;

void DrawWorld()
{
    //   glRotatef( 5, 0, 0, 1 );
    glRotatef( zRot, 0, 0, 1 );
    glRotatef( yRot, 0, 1, 0 );
    //zRot += 0.35;


    /*
        GLVector3<float> nextpoint;
        glBegin( GL_LINE_LOOP );
        int i;
        for( i = 0; i < NewCrossSection.GetNumPoints(); i++ )
        {
           NewCrossSection.GetWorldCoordinates( nextpoint, i );
           glVertex3f( nextpoint.x, nextpoint.y, nextpoint.z );
        }
        glEnd();
      */

    //  glBindTexture(GL_TEXTURE_2D, TextureIds[0] );   // ( CHANGE )

    // glutSolidTeapot( 1.0 );

    //glScalef( 3, 3, 5 );

    glPushMatrix();

#ifndef RINGSONLY
#ifdef SHOWBOXES

    mybox.Render();


    glTranslatef( 1.5,0,0);
    mybox2.Render();

    // glBindTexture(GL_TEXTURE_2D, TextureIds[1] );   // ( CHANGE )
    glTranslatef( 1.5,0,0);
    mybox3.Render();

    glTranslatef( -3, 1.5, 0 );
    mybox4.Render();

    //glBindTexture(GL_TEXTURE_2D, TextureIds[2] );   // ( CHANGE )
    glTranslatef( 1.5,0,0);
    mybox5.Render();

    glTranslatef( 1.5,0,0);
    mybox6.Render();

    glTranslatef( -3, 1.5, 0 );
    mybox7.Render();

    glTranslatef( 1.5,0,0);
    mybox8.Render();

    glTranslatef( 1.5,0,0);
    mybox9.Render();

    glTranslatef( 1.5,0,0);
    mybox10.Render();

    glTranslatef( 1.5,0,0);
    mybox11.Render();

    glTranslatef( 1.5,0,0);
    mybox12.Render();

    glTranslatef( -7.5, 1.5, 0 );

#endif

    prism1.Render();

    glTranslatef( 1.5,0,0);
    prism2.Render();

    glTranslatef( 1.5,0,0);
    prism3.Render();

    glTranslatef( -3, 1.5, 0 );
    cyl1.Render();

    glTranslatef( 1.5,0,0);
    cyl2.Render();

    glTranslatef( 1.5,0,0);
    cyl3.Render();

    glTranslatef( 1.5,0,0);
    cyl4.Render();

    glTranslatef( 1.5,0,0);
    cyl5.Render();

    glTranslatef( -3, 1.5, 0 );
    tor1.Render();

    glTranslatef( 1.5,0,0);
    tor2.Render();

    glTranslatef( 1.5,0,0);
    tor3.Render();

    glTranslatef( 1.5,0,0);
    tor4.Render();

    glTranslatef( 1.5,0,0);
    tor5.Render();

    glTranslatef( -9, -9, 0 );
    tube1.Render();

    glTranslatef( 1.5,0,0);
    tube2.Render();

    glTranslatef( 1.5,0,0);
    tube3.Render();

    glTranslatef( 1.5,0,0);
    tube4.Render();
#endif

    glTranslatef( -4.5, -1.5, 0 );
    ring1.Render();

    glTranslatef( 1.5,0,0);
    ring2.Render();

    glTranslatef( 1.5,0,0);
    ring3.Render();

    glTranslatef( 1.5,0,0);
    ring4.Render();

    glTranslatef( 1.5,0,0);
    ring5.Render();

    glPopMatrix();
#ifndef RINGSONLY

    glTranslatef( 0, -4.5, 0 );
    somebox.Render();

    static int iDirection = 1;
    somebox.SetTwist( iTwist );
    somebox.UpdateTransforms();

    iTwist += iDirection;

    if( iTwist > 180 )
    {
        iDirection = -1;
    }
    else if( iTwist < -180 )
    {
        iDirection = 1;
    }

    glTranslatef( 0, 15, 0 );
    int i;
    for( i = 0; i < iStressNumX; i++ )
    {
        ( boxes[i] )->Render();
        glTranslatef( 1.5, 0, 0 );
        ( prisms[i] )->Render();
        glTranslatef( 1.5, 0, 0 );
        ( cylinders[i] )->Render();
        glTranslatef( -3, 1.5, 0 );
    }

#endif

    /*
    glBegin( GL_QUADS );
    glVertex3f( 0,0,0);
    glVertex3f( 0,0,1);
    glVertex3f( 0,1,1);
    glVertex3f( 0,1,0);
    glEnd();
    */
    // glutSolidTeapot( 1.0 );

    //glBindTexture(GL_TEXTURE_2D, TextureIds[1] );   // ( CHANGE )

    //  glBindTexture(GL_TEXTURE_2D, texture[0].texID);   // ( CHANGE )
    //glutSolidTeapot( 1.0 );
}

void SetColor( float r, float g, float b )
{
    float mcolor[4];

    mcolor[0] = r;
    mcolor[1] = g;
    mcolor[2] = b;
    mcolor[3] = 1.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
}

void disp()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLoadIdentity();

    // rotate so z axis is up, and x axis is forward
    glRotatef( 90, 0.0, 0.0, 1.0 );
    glRotatef( 90, 0.0, 1.0, 0.0 );

    glRotatef( 0, 0, 1, 0 );
    glRotatef( 0, 0, 0, 1 );

    glTranslatef( 15, 0, 0 );

    glPushMatrix();
    // SetColor( 1,1,1);
    DrawWorld();
    glPopMatrix();

    //   GLfloat position[] = { -20.5f, 20.0f, 30.0f, 1.0f };
    //   glLightfv(GL_LIGHT0, GL_POSITION, position);

    glutSwapBuffers();
}

void MainLoop()
{
    glutPostRedisplay();
}

bool bLMouseDown = false;
int mousex, mousey;

void mousemove( int x, int y )
{
    if( bLMouseDown )
    {
        zRot += (x - mousex);
        yRot += (y - mousey);
        mousex = x;
        mousey = y;
    }
}

void mouse(int button, int state, int x, int y)
{
    if( button == GLUT_LEFT_BUTTON )
    {
        if( state == GLUT_DOWN )
        {
            mousex = x;
            mousey = y;
            bLMouseDown = true;
        }
        else if( state == GLUT_UP )
        {
            bLMouseDown = false;
        }
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    iWindowWidth = glutGet( GLUT_SCREEN_WIDTH );
    iWindowHeight = glutGet( GLUT_SCREEN_HEIGHT );
    glutInitWindowSize(iWindowWidth,iWindowHeight);
    glutInitWindowPosition(0,0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    win = glutCreateWindow("blah");

    glClearColor(0.0,0.0,0.0,0.0);

    glEnable(GL_DEPTH_TEST);
    //  glEnable (GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] =
        {
            0.5f, 0.5f, 0.5f, 1.0f
        };
    GLfloat diffuseLight[] =
        {
            0.8f, 0.8f, 0.8, 1.0f
        };
    GLfloat specularLight[] =
        {
            0.5f, 0.5f, 0.5f, 1.0f
        };
    GLfloat position[] =
        {
            -1.5f, 1.0f, -4.0f, 1.0f
        };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    if (!LoadGLTextures())
    {
        printf( "failed to load textures\n" );//
        exit(1);
    }

    glEnable(GL_TEXTURE_2D);


    SetupCrossSection();

    glLoadIdentity();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    GLfloat aspect = (GLfloat) iWindowWidth / iWindowHeight;
    gluPerspective( 45.0, aspect, 0.5, 100.0 );

    // glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

    glMatrixMode( GL_MODELVIEW );
    glViewport (0, 0, iWindowWidth, iWindowHeight);

    glutDisplayFunc(disp);
    glutIdleFunc( MainLoop );

    glutMouseFunc( mouse );
    glutMotionFunc( mousemove );

    glutMainLoop();

    return 0;
}


