/*****************************************************************************
*                                                                            *
*  OpenNI 1.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "view/NiHandViewer.h"
#include "gesture/Gesture.h"
#include "util/FileUtil.h"
#include "util/ConstantsUtil.h"

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "../SamplesConfig.xml"


//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
xn::Context		g_context;
xn::ScriptNode	g_scriptNode;

/** Set this flag false if you want reproduce the video from file */
int MODE_ONLINE = true;

int main(int argc, char* argv[])
{
	XnStatus				rc;
	xn::EnumerationErrors	errors;
    xn::Player              player;
    
    if(argc < 3) {
        std::cout<<" ------ We expect 2 arguments ------ "<<endl;
        std::cout<<"The gesture name and the number of hands."<<endl;
        std::cout<<"The number of hands should be 1 or 2."<<endl;
        std::cout<<"Example:"<<endl;
        std::cout<<"./gesture_tracking paisagem 1"<<endl;
        return 0;
    }

    FileUtil::getInstance().setInfoGesture(argv[1], atoi(argv[2]));

    if(MODE_ONLINE){
        
        // Create a context with default settings
        rc = g_context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);
        if (rc == XN_STATUS_NO_NODE_PRESENT)
        {
            XnChar strError[1024];
            errors.ToString(strError, 1024);
            printf("%s\n", strError);
            return (rc);
        }
        
        CHECK_RC(rc, "Open failed: %s\n");
        
    }else{
        
        rc = g_context.Init();
        CHECK_RC(rc, "Init context: %s");
        
        rc = g_context.OpenFileRecording(FILE_NAME_RECORD_IMAGE);
        CHECK_RC(rc, "Open image file recorgind %s");
        
        rc = g_context.OpenFileRecording(FILE_NAME_RECORD_DEPTH);
        CHECK_RC(rc, "Open depth file recorgind %s");
        
    }
    
	SimpleViewer& viewer = HandViewer::CreateInstance(g_context);
    
    if(MODE_ONLINE){
        viewer.activeModeOnline();
    }
    
	rc = viewer.Init(argc, argv);
    CHECK_RC(rc, "Viewer init failed: %s\n");

	rc = viewer.Run();
    CHECK_RC(rc, "Viewer run failed: %s\n");
    
	return 0;
}