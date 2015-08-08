ifeq ($(LINUX),1)
C++FLAGS = -I. \
           -I/usr/include \
           -I/home/hughperkins/dev/Mesa-6.2.1/include \
           -I/home/hughperkins/dev/mathgl-pp-0.5.0/src/libmathglpp \
           -g -fPIC 
CCFLAGS = $(C++FLAGS)
C++ = g++ $(C++FLAGS) -c
OBJPOSTFIX = o

all:	libfractalspline.so

endif

ifeq ($(MSVC),1)
C++FLAGS = /I.
CCFLAGS = $(C++FLAGS)
C++ = cl $(C++FLAGS) /c 
OBJPOSTFIX = obj

all:	libfractalspline.lib

endif


ALLOBJECTS = CrossSection.$(OBJPOSTFIX) PrimitiveBox.$(OBJPOSTFIX) LinearExtrusionPath.$(OBJPOSTFIX) \
	ExtrusionPath.$(OBJPOSTFIX) \
	LinearPrimitiveImpl.$(OBJPOSTFIX) PrimitiveImpl.$(OBJPOSTFIX) RotationalPrimitiveImpl.$(OBJPOSTFIX) \
	PrimitivePrism.$(OBJPOSTFIX) PrimitiveCylinder.$(OBJPOSTFIX) RotationalExtrusionPath.$(OBJPOSTFIX) \
	PrimitiveTorus.$(OBJPOSTFIX) PrimitiveTube.$(OBJPOSTFIX) \
	PrimitiveRing.$(OBJPOSTFIX) Primitive.$(OBJPOSTFIX) \
	PrimitiveBoxImpl.$(OBJPOSTFIX) PrimitiveTorusImpl.$(OBJPOSTFIX) PrimitiveCylinderImpl.$(OBJPOSTFIX) \
	PrimitiveTubeImpl.$(OBJPOSTFIX) PrimitiveRingImpl.$(OBJPOSTFIX) PrimitivePrismImpl.$(OBJPOSTFIX)

libfractalspline.so:	$(ALLOBJECTS)
	ld -shared -out:libfractalspline.so $(ALLOBJECTS)

libfractalspline.lib:  $(ALLOBJECTS)
	link /DLL /OUT:libfractalspline.dll $(ALLOBJECTS)


ifeq ($(MSVC),1)
testfractalspline:	testfractalspline.exe
	

testfractalspline.exe:	libfractalspline.lib testfractalspline.obj
	link /OUT:testfractalspline.exe testfractalspline.obj libfractalspline.lib

endif


ifeq ($(LINUX),1)
testfractalspline:	libfractalspline.so testfractalspline.obj
	g++ -out:testfractalspline testfractalspline.obj libfractalspline.so
endif


PrimitiveBox.$(OBJPOSTFIX): PrimitiveBox.cpp PrimitiveBox.h Primitive.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveBox.cpp
   
Primitive.$(OBJPOSTFIX): Primitive.cpp Primitive.h
	$(C++) /DBUILDING_FRACTALSPLINE Primitive.cpp
   
PrimitivePrism.$(OBJPOSTFIX): PrimitivePrism.cpp PrimitivePrism.h Primitive.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitivePrism.cpp
   
PrimitiveTorus.$(OBJPOSTFIX): PrimitiveTorus.cpp PrimitiveTorus.h Primitive.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveTorus.cpp
   
PrimitiveTube.$(OBJPOSTFIX): PrimitiveTube.cpp PrimitiveTube.h Primitive.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveTube.cpp
   
PrimitiveRing.$(OBJPOSTFIX): PrimitiveRing.cpp PrimitiveRing.h  Primitive.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveRing.cpp
   
PrimitiveCylinder.$(OBJPOSTFIX): PrimitiveCylinder.cpp PrimitiveCylinder.h Primitive.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveCylinder.cpp


PrimitiveBoxImpl.$(OBJPOSTFIX): PrimitiveBoxImpl.cpp PrimitiveBoxImpl.h LinearPrimitiveImpl.h PrimitiveImpl.h PrimitiveInterface.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveBoxImpl.cpp
   
PrimitiveTorusImpl.$(OBJPOSTFIX): PrimitiveTorusImpl.cpp PrimitiveTorusImpl.h RotationalPrimitiveImpl.h PrimitiveImpl.h PrimitiveInterface.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveTorusImpl.cpp

PrimitiveCylinderImpl.$(OBJPOSTFIX): PrimitiveCylinderImpl.cpp PrimitiveCylinderImpl.h LinearPrimitiveImpl.h PrimitiveImpl.h PrimitiveInterface.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveCylinderImpl.cpp

PrimitiveTubeImpl.$(OBJPOSTFIX): PrimitiveTubeImpl.cpp PrimitiveTubeImpl.h RotationalPrimitiveImpl.h PrimitiveImpl.h PrimitiveInterface.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveTubeImpl.cpp

PrimitiveRingImpl.$(OBJPOSTFIX): PrimitiveRingImpl.cpp PrimitiveRingImpl.h RotationalPrimitiveImpl.h PrimitiveImpl.h PrimitiveInterface.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveRingImpl.cpp

PrimitivePrismImpl.$(OBJPOSTFIX): PrimitivePrismImpl.cpp PrimitivePrismImpl.h LinearPrimitiveImpl.h PrimitiveImpl.h PrimitiveInterface.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitivePrismImpl.cpp

   
LinearPrimitiveImpl.$(OBJPOSTFIX): LinearPrimitiveImpl.cpp PrimitiveImpl.h
	$(C++) /DBUILDING_FRACTALSPLINE LinearPrimitiveImpl.cpp
   
RotationalPrimitiveImpl.$(OBJPOSTFIX): RotationalPrimitiveImpl.cpp PrimitiveImpl.h
	$(C++) /DBUILDING_FRACTALSPLINE RotationalPrimitiveImpl.cpp
   
PrimitiveImpl.$(OBJPOSTFIX): PrimitiveImpl.cpp PrimitiveImpl.h PrimitiveInterface.h
	$(C++) /DBUILDING_FRACTALSPLINE PrimitiveImpl.cpp
   
CrossSection.$(OBJPOSTFIX): CrossSection.cpp CrossSection.h RendererCallbacks.h RendererCallbacksOpenGL.h
	$(C++) /DBUILDING_FRACTALSPLINE CrossSection.cpp
   
ExtrusionPath.$(OBJPOSTFIX): ExtrusionPath.cpp ExtrusionPath.h
	$(C++) /DBUILDING_FRACTALSPLINE ExtrusionPath.cpp
   
LinearExtrusionPath.$(OBJPOSTFIX): LinearExtrusionPath.cpp LinearExtrusionPath.h
	$(C++) /DBUILDING_FRACTALSPLINE LinearExtrusionPath.cpp
   
RotationalExtrusionPath.$(OBJPOSTFIX): RotationalExtrusionPath.cpp RotationalExtrusionPath.h
	$(C++) /DBUILDING_FRACTALSPLINE RotationalExtrusionPath.cpp
   
testfractalspline.$(OBJPOSTFIX): testfractalspline.cpp PrimitiveBox.h Primitive.h
	$(C++) /DBUILDING_TESTFRACTALSPLINE testfractalspline.cpp
 