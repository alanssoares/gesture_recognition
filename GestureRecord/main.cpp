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

//Define the global type params
Params g_params;

int helpUsage();
int parse_command_line(int argc, char* argv[]);

int main(int argc, char* argv[]) 
{
	XnStatus				rc;
	xn::EnumerationErrors	errors;
    xn::Player              player;

    //Validate input parameters
    if(parse_command_line(argc, argv)) return 0;
    
    //Set parameters
    FileUtil::getInstance().setInfoGesture(g_params.name, g_params.numHands);
    
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
    
	SimpleViewer& viewer = HandViewer::CreateInstance(g_context);

	rc = viewer.Init(argc, argv);
    CHECK_RC(rc, "Viewer init failed: %s\n");
	
    rc = viewer.Run();
    CHECK_RC(rc, "Viewer run failed: %s\n");
    
	return 0;
}

/**
 Show the help usage of the GestureRecord
*/
int helpUsage(){
    PRINT(" ------- Command line input -------- ");
    PRINT("Usage: ./GestureRecord options");
    PRINT("options: ");
    PRINT("-g gesture : the gesture name");
    PRINT("-n hands : the number of hands (1 or 2)");
    PRINT("Example: ./GestureRecord -g paisagem -n 2");
    PRINT(" ------- Command controls ------- ");
    PRINT("i : init the hand tracking");
    PRINT("p : stop the hand tracking and save all files (depth, image and track)");
    PRINT("m : set the global mirror");
    PRINT("q : quit the application");
    return 1;
}

/**
 Read the params of the command line
*/
int parse_command_line(int argc, char* argv[]){
    g_params.numHands = 1;
    g_params.name = (char*) malloc(sizeof(char) * 50);
    strcpy(g_params.name, "default");
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] != '-') break;
        i++;
        switch(argv[i - 1][1]) {
            case 'h':
                return helpUsage();
                break;
            case 'n':
                g_params.numHands = atoi(argv[i]);
                if(g_params.numHands != 1 && g_params.numHands != 2) {
                    PRINT("Warning - The number of hands should be 1 or 2");
                    return 1;
                }
                break;
            case 'g':
                g_params.name = argv[i];
                break;
            default:
                PRINT("Unknown option -"<< argv[i - 1][1]);
                return 1;
        }
    }
    return 0;
}