// ClientDLL.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <iostream>
#include "PlatheaDLL.h"
#include <vector>
#include <time.h>
/*
#include <LeoWindowsGDI.h>
#include <LeoWindowsOpenCV.h>
#include <LeoLog4CPP.h>
#include <LeoWindowsGUI.h>
#include <LeoOpenCVExtension.h>
*/



using namespace std;


int main()
{	
	char str[] = "D:\\PLaTHEATest\\experiment.xml ";
	system_loadconfigurationfile(0,str);
	internalCalibrationDemo("D:\\PLaTHEATest\\InternalCalibration");
	calibration_loadexternalcalibrationdata("D:\\PLaTHEATest\\ExternalCalibration");
	localizationengine_selectsvmclassifier("D:\\PLaTHEATest\\Tracking\\svmclassifier.xml");
	initializeSystemDemo();
	localizationengine_startlocalizationengine(false, false, "");
	const char* s = "D:\\PLaTHEATest\\Tests\\21-12-2012 - 11-25-10-165";
	test_platheaplayer();
	test_platheaplayer_start(s);
	//ElaborationCore* ec=getElaborationCore();
	//printf("ec: %d\n",ec);
	
	//test_positiontest(true);
	//test_positiontest_start();
	//ec = getElaborationCore();
	//printf("ec: %d\n", ec);
	//test_plathearecorder_stop();
	
	StreamsVideo* sv = getStreamsVideo();
	//while (true) sv->getFrame();
	//printf("getframe: %d\n",sv->getFrame().size());

	std::vector<TrackedObject*> tracked = getTrackedPeople();
	printf("\ntracked: %d\n",tracked.size());
	
	while (tracked.size() == 0) {
		tracked = getTrackedPeople();
		printf("get tracked\n");
		//if(tracked.size()>0) printf("%d\n",tracked[0]->ID);
	}
	
	printf("%s\n",tracked[0]->name);
	printf("%d\n",tracked[0]->nameID);
	printf("%d\n",tracked[0]->X);
	printf("%d\n",tracked[0]->Y);
	
	test_plathearecorder_stop();

	

	
	cout << "Done!" << endl;
	getchar();
	return 0;
}


