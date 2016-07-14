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
#include "../Commons/commons.hpp"

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "../SamplesConfig.xml"


//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
xn::Context		g_context;
xn::ScriptNode	g_scriptNode;
Params g_params;

int helpUsage();
int parse_command_line(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	XnStatus				rc;
	xn::EnumerationErrors	errors;
    xn::Player              player;
    FileUtil& futil = FileUtil::getInstance();
    Gesture& gesture = Gesture::getInstance();
    
    //Read the params from args
    if(parse_command_line(argc, argv)) return 0;
    // Load the gestures data
    futil.loadGestures();
    // Setter the gestures from file
    gesture.setGesturesFromFile(futil.getGesturesOneHand(), futil.getGesturesTwoHands());
    
    if(g_params.modeOnline){
        
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
        
    } else {
        
        rc = g_context.Init();
        CHECK_RC(rc, "Init context: %s");
        
        rc = g_context.OpenFileRecording(g_params.fileImage.c_str());
        CHECK_RC(rc, "Open image file recorgind %s");
        
        rc = g_context.OpenFileRecording(g_params.fileDepth.c_str());
        CHECK_RC(rc, "Open depth file recorgind %s");
        
    }
    
    //Copy the params to use in record
    gesture.setParams(g_params);
    
	SimpleViewer& viewer = HandViewer::CreateInstance(g_context);
    
    if(g_params.modeOnline){
        viewer.activeModeOnline();
    }
    
	rc = viewer.Init(argc, argv);
    CHECK_RC(rc, "Viewer init failed: %s\n");

	rc = viewer.Run();
    CHECK_RC(rc, "Viewer run failed: %s\n");
    
	return 0;
}

/**
 Show the help usage of the GestureRecord
*/
int helpUsage()
{
    PRINT(" ------- Command line input -------- ");
    PRINT("Usage: ./startApp options");
    PRINT("options: ");
    PRINT("-m mode : Mode of the execution (offline or online). Default is 'offline'");
    PRINT("-f file : File name of the gesture. Default is 'Stream'");
    PRINT("");
    PRINT(" ------- Command controls ------- ");
    PRINT("q : Quit the application");
    PRINT("m : Set global mirror");
    PRINT("1 : Display mode overlay");
    PRINT("2 : Display mode depth");
    PRINT("3 : Display mode image");
    PRINT("");
    PRINT("Example: ./startApp -m offline -f cancelar");
    return 1;
}

/**
 Read the params of the command line
*/
int parse_command_line(int argc, char* argv[]){

    //Initialize default params
    g_params.modeOnline = false;
    g_params.fileImage = std::string("../StreamImage.oni");
    g_params.fileDepth = std::string("../StreamDepth.oni");

    for(int i = 1; i < argc; i++) {
        if(argv[i][0] != '-') break;
        i++;
        switch(argv[i - 1][1]) {
            case 'h':
                return helpUsage();
                break;
            case 'm':
                if (strcmp(argv[i], "online") == 0) {
                    g_params.modeOnline = true;
                }
                break;
            case 'f':
                g_params.fileImage = std::string("../") + std::string(argv[i]) + std::string("Image.oni");
                g_params.fileDepth = std::string("../") + std::string(argv[i]) + std::string("Depth.oni");
                break;
            default:
                PRINT("unknown option - "<< argv[i - 1][1]);
                return 1;
                break;
        }
    }
    return 0;
}