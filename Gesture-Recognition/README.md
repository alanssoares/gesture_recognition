# Gesture-Recognition

Como configurar no Mac:

Em Build Phases->Link Binary With Libraries, adicionar os seguintes frameworks:
- GLUT.framework
- OpenGL.framework

Em Build Phases->Copy Files, adicionar o seguinte arquivo:
- SamplesConfig.xml

Em Build Settings->Linking->Other Linker Flags
- /usr/local/Cellar/nite/1.5.2.21/lib/libXnVHandGenerator_1_5_2.dylib
- /usr/local/Cellar/nite/1.5.2.21/lib/libXnVFeatures_1_5_2.dylib
- /usr/local/Cellar/openni/1.5.7.10/lib/libnimCodecs.dylib
- /usr/local/Cellar/openni/1.5.7.10/lib/libnimMockNodes.dylib
- /usr/local/Cellar/openni/1.5.7.10/lib/libnimRecorder.dylib
- /usr/local/Cellar/openni/1.5.7.10/lib/libOpenNI.dylib

Em Build Settings->Search Paths->Header Search Paths, adicionar:
- /usr/include/ni
- /usr/include/nite
- /usr/local/Cellar/opencv

