/***************************************************************************
	PLaTHEA: People Localization and Tracking for HomE Automation
	Copyright (C) 2014 Francesco Leotta
	
	This file is part of PLaTHEA
	PLaTHEA is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser Public License as published by
	the Free Software Foundation, version 3 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	
	GNU Lesser Public License for more details.

	You should have received a copy of the GNU Lesser Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include "ElaborationCore.h"
#include "PlanViewMap.h"
#include "ApplicationWorkFlow.h"
#include "Shared.h"
#include "video_output.h"
#include "PositionTestDialog.h"
#include "PlatheaDLL.h"

#include <LeoLog4CPP.h>

using namespace std;
using namespace stdext;
using namespace leostorm::logging;
using namespace leostorm::settingspersistence;

extern SystemInfo *si;

struct EditControlAction {
	ParameterSetter *paramSetter;
	char var[50];
};

unordered_map<HWND, EditControlAction> corresp;
LONG_PTR prevFunc; HWND hwndSettings;

LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_KEYDOWN:
			{
				wchar_t buf[20];
				if (wParam == 38 || wParam == 40) {
					corresp[hwnd].paramSetter->ModifyParameter(corresp[hwnd].var, (wParam==38 ? 1 : -1));
					corresp[hwnd].paramSetter->GetParameterMap()[corresp[hwnd].var].ToString(buf, 20);
					SetWindowText(hwnd, buf);
					return 0;
				}
				break;
			}
	}
	return CallWindowProc((WNDPROC) prevFunc, hwnd, uMsg, wParam, lParam);
}

/*
INT_PTR CALLBACK SettingsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
			hActiveModelessWindow = NULL;
		else
			hActiveModelessWindow = hwndDlg;
		return TRUE;
		case WM_INITDIALOG:
			{
				wchar_t buf[20]; HWND hwndEdit; EditControlAction ppc;
				corresp.clear();
				
				ppc.paramSetter = (ParameterSetter *) (*SettingsPersistence::GetInstance())["colorModel"];
				//Background Model nominal learning factor
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT3); strcpy_s(ppc.var, 50, "alphaNom"); corresp[hwndEdit] = ppc;
				//Background Model Activity Threshold
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT4); strcpy_s(ppc.var, 50, "activityThreshold"); corresp[hwndEdit] = ppc;

				ppc.paramSetter = (ParameterSetter *) (*SettingsPersistence::GetInstance())["edgeIntensityModel"];
				//Edge Model Learning Factor
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT1); strcpy_s(ppc.var, 50, "beta"); corresp[hwndEdit] = ppc;
				//Edge Model Minimum Rumour
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT7); strcpy_s(ppc.var, 50, "minRumour"); corresp[hwndEdit] = ppc;

				ppc.paramSetter = (ParameterSetter *) (*SettingsPersistence::GetInstance())["activityModel"];
				//Activity Learning Factor
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT5); strcpy_s(ppc.var, 50, "lambda"); corresp[hwndEdit] = ppc;

				ppc.paramSetter = (ParameterSetter *) (*SettingsPersistence::GetInstance())["foregroundSegmentation"];
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT2); strcpy_s(ppc.var, 50, "minColorDistortion"); corresp[hwndEdit] = ppc;
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT6); strcpy_s(ppc.var, 50, "lowRGBMaxBrightnessDistortion"); corresp[hwndEdit] = ppc;
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT9); strcpy_s(ppc.var, 50, "minDarkening"); corresp[hwndEdit] = ppc;
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT10);	strcpy_s(ppc.var, 50, "maxDarkening"); corresp[hwndEdit] = ppc;
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT11);	strcpy_s(ppc.var, 50, "ts"); corresp[hwndEdit] = ppc;
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT12); strcpy_s(ppc.var, 50, "th"); corresp[hwndEdit] = ppc;

				ppc.paramSetter = (ParameterSetter *) (*SettingsPersistence::GetInstance())["findConnectedComponents"];
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT8); strcpy_s(ppc.var, 50, "perimScale");	corresp[hwndEdit] = ppc;
			
				ppc.paramSetter = (ParameterSetter *) (*SettingsPersistence::GetInstance())["elaborationCore"];
				//Preprocessing Window Size
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT13);	strcpy_s(ppc.var, 50, "preFilterSize"); corresp[hwndEdit] = ppc;
				//Preprocessing Filter Cap
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT14);	strcpy_s(ppc.var, 50, "preFilterCap"); corresp[hwndEdit] = ppc;
				//SAD Window Size
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT15);	strcpy_s(ppc.var, 50, "SADWindowSize"); corresp[hwndEdit] = ppc;
				//minimum Disparity
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT16);	strcpy_s(ppc.var, 50, "minDisparity"); corresp[hwndEdit] = ppc;
				//number of disparities
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT17); strcpy_s(ppc.var, 50, "numberOfDisparities"); corresp[hwndEdit] = ppc;
				//Uniqueness Ratio
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT18);	strcpy_s(ppc.var, 50, "uniquenessRatio"); corresp[hwndEdit] = ppc;
				//TextureThreshold
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT19);	strcpy_s(ppc.var, 50, "textureThreshold"); corresp[hwndEdit] = ppc;

				ppc.paramSetter = (ParameterSetter *) (*SettingsPersistence::GetInstance())["planViewMap"];
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT23); strcpy_s(ppc.var, 50, "person_occupation_threshold"); corresp[hwndEdit] = ppc;
				hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT20);	strcpy_s(ppc.var, 50, "chi"); corresp[hwndEdit] = ppc;

				for each (pair<HWND, EditControlAction> c in corresp) {
					prevFunc = SetWindowLongPtr(c.first, GWLP_WNDPROC, (LONG_PTR) EditProc);
					c.second.paramSetter->GetParameterMap()[c.second.var].ToString(buf, 20);
					SetWindowText(c.first, buf);
				}
				CheckDlgButton(hwndDlg, IDC_CHECK3,
					((ParameterSetter *)(*SettingsPersistence::GetInstance())["foregroundSegmentation"])->GetParameterValue<bool>("useShadowSuppressor") ? BST_CHECKED : BST_UNCHECKED);
				CheckDlgButton(hwndDlg, IDC_CHECK2,
					((ParameterSetter *)(*SettingsPersistence::GetInstance())["findConnectedComponents"])->GetParameterValue<bool>("useContourScanner") ? BST_CHECKED : BST_UNCHECKED);

				return TRUE;
			}
		case WM_CLOSE:
			DestroyWindow(hwndDlg);
			hwndSettings = NULL;
			return TRUE;
		case WM_COMMAND:
			{
				if (LOWORD(wParam)==IDC_CHECK3) {
					((ParameterSetter *)(*SettingsPersistence::GetInstance())["foregroundSegmentation"])->SetParameterValue("useShadowSuppressor", IsDlgButtonChecked(hwndDlg, IDC_CHECK3) == BST_CHECKED);
					return TRUE;
				} else if (LOWORD(wParam)==IDC_CHECK2) {
					((ParameterSetter *)(*SettingsPersistence::GetInstance())["findConnectedComponents"])->SetParameterValue("useContourScanner", IsDlgButtonChecked(hwndDlg, IDC_CHECK3) == BST_CHECKED);
					return TRUE;
				}
			}
			break;
	}
	return FALSE;
}
*/

class ECParametersSetter: public ParameterSetter {
public:
	CvStereoBMState *BMState;
	int noCorrespondenceDisparityValue;
	ECParametersSetter();
	~ECParametersSetter();
	void ModifyParameter(char *var, int delta); //For perimScale
};

ECParametersSetter::ECParametersSetter(): ParameterSetter("elaborationCore") {
	BMState = cvCreateStereoBMState();

	AddParameter("preFilterSize", &BMState->preFilterSize);
	BMState->preFilterSize = 41;
	AddParameter("preFilterCap", &BMState->preFilterCap);
	BMState->preFilterCap = 31;
	AddParameter("SADWindowSize", &BMState->SADWindowSize);
	BMState->SADWindowSize = 15;
	AddParameter("minDisparity", &BMState->minDisparity);
	BMState->minDisparity = 0;
	AddParameter("numberOfDisparities", &BMState->numberOfDisparities);
	BMState->numberOfDisparities = 32;
	AddParameter("textureThreshold", &BMState->textureThreshold);
	BMState->textureThreshold = 30;
	AddParameter("uniquenessRatio", &BMState->uniquenessRatio);
	BMState->uniquenessRatio = 40;

	noCorrespondenceDisparityValue = (BMState->minDisparity-1)*16;
}

ECParametersSetter::~ECParametersSetter() {
	cvReleaseStereoBMState(&BMState);
}

void ECParametersSetter::ModifyParameter(char *var, int delta) {
	int height, width;
	si->GetStereoRig()->GetAcquisitionProperties().GetResolution(&height, &width, true);
	
	if (strcmp(var, "preFilterSize")==0) {
		BMState->preFilterSize+=(delta*2);
		BMState->preFilterSize = ApplyThreshold(BMState->preFilterSize, LEO_APPLYTHRESHOLD_MIN | LEO_APPLYTHRESHOLD_MAX, 5, 255);
	} else if (strcmp(var, "preFilterCap")==0) {
		BMState->preFilterCap+=delta;
		BMState->preFilterCap = ApplyThreshold(BMState->preFilterCap, LEO_APPLYTHRESHOLD_MIN | LEO_APPLYTHRESHOLD_MAX, 1, 63);
	} else if (strcmp(var, "SADWindowSize")==0) {
		BMState->SADWindowSize+=(delta*2);
		if (BMState->SADWindowSize > height || BMState->SADWindowSize > width) {
			BMState->SADWindowSize = MIN(height, width);
			if (!(BMState->SADWindowSize%2))
				BMState->SADWindowSize--;
		} else {
			BMState->SADWindowSize = ApplyThreshold(BMState->SADWindowSize, LEO_APPLYTHRESHOLD_MIN | LEO_APPLYTHRESHOLD_MAX, 5, 255);
		}
	} else if (strcmp(var, "minDisparity")==0) {
		BMState->minDisparity+=delta;
		BMState->minDisparity = ApplyThreshold(BMState->minDisparity, LEO_APPLYTHRESHOLD_MIN, -128, 0);
		noCorrespondenceDisparityValue = (BMState->minDisparity-1)*16;
	} else if (strcmp(var, "numberOfDisparities")==0) {
		BMState->numberOfDisparities+=(delta*16);
		BMState->numberOfDisparities = ApplyThreshold(BMState->numberOfDisparities, LEO_APPLYTHRESHOLD_MIN, 16, 0);
	} else if (strcmp(var, "uniquenessRatio")==0) {
		BMState->uniquenessRatio+=delta;
		BMState->uniquenessRatio = ApplyThreshold(BMState->uniquenessRatio, LEO_APPLYTHRESHOLD_MIN, 0, 0);
	} else if (strcmp(var, "textureThreshold")==0) {
		BMState->textureThreshold+=delta;
		BMState->textureThreshold = ApplyThreshold(BMState->textureThreshold, LEO_APPLYTHRESHOLD_MIN, 0, 0);
	}
}

static ECParametersSetter ecParameters;

ElaborationCore::ElaborationCore(): BMState(ecParameters.BMState), noCorrespondenceDisparityValue(ecParameters.noCorrespondenceDisparityValue) {
	this->hStereoEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	this->hStopRunningEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	this->bm = NULL;
	this->pvm = NULL;
	hwndSettings = NULL;

	currentElaborationCoreMode = FULL_FEATURE_ELABORATION_CORE_MODE;
	
	initPhase = true;
	//printf("elaboration core %d\n", si->GetElaborationCore());
	//printf("%d initPhase  true=%d\n", &initPhase,initPhase);

	/*this->SGBMState = new cv::StereoSGBM(ecParameters.BMState->minDisparity, ecParameters.BMState->numberOfDisparities,
		ecParameters.BMState->SADWindowSize);
	this->BMState = new cv::StereoBM(cv::StereoBM::BASIC_PRESET, ecParameters.BMState->numberOfDisparities, 11);*/
}

ElaborationCore::~ElaborationCore() {
	CloseHandle(hStereoEvent);
	CloseHandle(hStopRunningEvent);
	if (hwndSettings)
		DestroyWindow(hwndSettings);
	//delete this->SGBMState;
}


void ElaborationCore::ShowSettingsWindow() {
	if (!hwndSettings) {
//		hwndSettings = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTINGS), si->GetMainWindow(), SettingsProc);
		ShowWindow(hwndSettings, SW_SHOW);
	}
}

DWORD WINAPI HaarThread(LPVOID lpParam) {
	CvMemStorage *storage = cvCreateMemStorage(0);
	HaarClassifierThreadParam *hctp = (HaarClassifierThreadParam *) lpParam;
	IplImage *grayImg = cvCreateImage(cvSize(hctp->bigImagePointer->width, hctp->bigImagePointer->height),
		IPL_DEPTH_8U, 1);
	IplImage *preSizedImage = cvCreateImage(cvSize(150, 150), IPL_DEPTH_8U, 3);
	int smallHeight, smallWidth;
	si->GetStereoRig()->GetAcquisitionProperties().GetResolution(&smallHeight, &smallWidth, true);
	int bigHeight, bigWidth;
	si->GetStereoRig()->GetAcquisitionProperties().GetResolution(&bigHeight, &bigWidth, false);
	float pixelXMod = float(smallWidth)/float(bigWidth);
	float pixelYMod = float(smallHeight)/float(bigHeight);
	HANDLE eventsToWait[] = {hctp->hEventBigImageReady, hctp->hStopRunningEvent};
	while (WaitForMultipleObjects(2, eventsToWait, FALSE, INFINITE) == WAIT_OBJECT_0) {
		cvCvtColor(hctp->bigImagePointer, grayImg, CV_BGR2GRAY);
		cvEqualizeHist(grayImg, grayImg);
		cvClearMemStorage(storage);
		hctp->faces = cvHaarDetectObjects(grayImg, hctp->cascade, storage, 1.1, 2, 0, cvSize(50, 50));
		for (int i = 0; i < (hctp->faces ? hctp->faces->total : 0); i++) {
			CvRect *seqElem = (CvRect *) cvGetSeqElem(hctp->faces, i);
			cvSetImageROI(hctp->bigImagePointer, *seqElem);
			if (seqElem->width >= 150)
				cvResize(hctp->bigImagePointer, preSizedImage, CV_INTER_AREA);
			else
				cvCopyMakeBorder(hctp->bigImagePointer, preSizedImage, cvPoint(0, 0), IPL_BORDER_CONSTANT);
			cvResetImageROI(hctp->bigImagePointer);

			int bestPersonScore;
			int bestPerson = mainFaceDatabase.FaceMatch(preSizedImage, &bestPersonScore);
			if (bestPerson >= 0 && bestPersonScore >= 5) {
				Logger::writeToLOG(L"%S: %d\r\n", mainFaceDatabase[bestPerson].name, bestPersonScore);
				int xCenter = seqElem->x + (seqElem->width/2);
				int yCenter = seqElem->y + ((seqElem->height*3)/2); //Head sometimes doesn't contain disparity information so we find the chest
				if (yCenter < hctp->bigImagePointer->height) {
					xCenter = int(xCenter*pixelXMod); yCenter = int(yCenter*pixelYMod);
					double floatXCenter = cvGetReal2D(StereoCalibration::GetInstance()->mx_LEFT, yCenter, xCenter);
					double floatYCenter = cvGetReal2D(StereoCalibration::GetInstance()->my_LEFT, yCenter, xCenter);
					xCenter = (int) floatXCenter; yCenter = (int) floatYCenter;
					if (xCenter < smallWidth && yCenter < smallHeight) {
						int pos = (yCenter)*smallWidth + xCenter;
						CvPoint planViewFaceProj = hctp->imagePixelMapping[pos];
						if (planViewFaceProj.x >= 0 && planViewFaceProj.y >= 0) {
							int nearestCandidate = -1; float bestDistance = 0.f;
							for (unsigned int j = 0; j < hctp->tfc.size(); j++) {
								CvRect bRect = hctp->tfc[j].boundingRect;
								if (planViewFaceProj.x >= bRect.x && planViewFaceProj.y >= bRect.y && planViewFaceProj.x < bRect.x + bRect.width && planViewFaceProj.y < bRect.y + bRect.height) {
									float xDiff = planViewFaceProj.x - hctp->tfc[j].xPosition; xDiff*=xDiff;
									float yDiff = planViewFaceProj.y - hctp->tfc[j].yPosition; yDiff*=yDiff;
									float distance = sqrt(xDiff + yDiff);
									if (nearestCandidate == -1 || distance < bestDistance) {
										nearestCandidate = j;
										bestDistance = distance;
									}
								}
							}
							if (nearestCandidate != -1) {
								hctp->tfc[nearestCandidate].founded = true;
								strcpy_s(hctp->tfc[nearestCandidate].name, 256, mainFaceDatabase[bestPerson].name);
								hctp->tfc[nearestCandidate].personID = bestPerson;
							}
						}
					}
				}
			}
		}
		SetEvent(hctp->hEventFacesReady);
	}
	cvReleaseImage(&grayImg);
	cvReleaseImage(&preSizedImage);
	cvReleaseMemStorage(&storage);
	return 0;
}

bool ElaborationCore::PrerequisitesCheck(wchar_t *errMsg, int bufferSize, bool *warning) {
	//printf("check\n");
	if (!StereoCalibration::GetInstance()->IsComplete()) {
		if (errMsg) {
			wcscpy_s(errMsg, bufferSize, L"Stereo Calibration Data has not been loaded!!\r\n\r\nElaboration Core won't be able to execute the whole work.");
		}
		printf("stereo calibration data has not been loaded\n");
		return false;
	} else if (currentElaborationCoreMode == FULL_FEATURE_ELABORATION_CORE_MODE) {
		if (!ExternalCalibration::GetInstance()->IsComplete()) {
			printf("External calibration data has not been loaded\n");

			if (errMsg) {
				wcscpy_s(errMsg, bufferSize, L"External Calibration Data has not been loaded!!\r\n\r\nElaboration Core won't be able to execute the whole work.");
			}
			return false;
		}
		else if (!mainFaceDatabase.PrerequisitesCheck(errMsg, bufferSize, warning)) {
			printf("mainFaceDatabase data has not been loaded\n");

			return false;
		}
	}
	if (warning)
		*warning = false;
	return true;
}

void ElaborationCore::Run(void *param) {
	//printf("elaboration core run\n");
	ApplicationWorkFlow::GetInstance()->UpdateSystemState(ELABORATION_STARTED);
	//printf("1\n");
	StereoRig *asr = si->GetStereoRig();
	//printf("2\n");
	int height, width;
	//printf("3\n");
	si->GetStereoRig()->GetAcquisitionProperties().GetResolution(&height, &width, true);
	//printf("4\n");
	IplImage * grayScaleImage[2] = {cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1),
		cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1)};
	//printf("5\n");
	IplImage * vEdge = cvCreateImage(cvSize(width, height), IPL_DEPTH_16S, 1);
	//printf("6\n");
	IplImage * hEdge = cvCreateImage(cvSize(width, height), IPL_DEPTH_16S, 1);
	//printf("7\n");
	IplImage * clonedColorImages[2] = {cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3),
		cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3)};
	//printf("7\n");
	int heightS, widthS;
	//printf("8\n");
	si->GetStereoRig()->GetAcquisitionProperties().GetResolution(&heightS, &widthS, false);
	//printf("9\n");
	IplImage *clonedColorBigLeftImage = cvCreateImage(cvSize(widthS, heightS), IPL_DEPTH_8U, 3);
	//printf("10\n");
	CvMat *disp = cvCreateMat(height, width, CV_16S); //Short Type Disparity Map
	//printf("11\n");
	CvMat *dispFloat = cvCreateMat(height, width, CV_32FC1);
	//printf("12\n");
	IplImage *temp3channel = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	//printf("13\n");
	IplImage *temp1channel = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	//printf("14\n");
	RoomSettingsStruct *rss = &RoomSettings::GetInstance()->data;
	//printf("15\n");
	bm = new BackgroundModeling(height, width);
	//printf("16\n");
	CvMat *reprojection3D = cvCreateMat(height, width, CV_32FC3); //Reprojection to 3D
	//printf("17\n");
	pvm = NULL;
	//printf("18\n");
	HaarClassifierThreadParam hctp;
	//printf("19\n");
	HANDLE hHaarThread = INVALID_HANDLE_VALUE;
	//printf("20\n");
	HANDLE hSaveToFile = INVALID_HANDLE_VALUE;
	//printf("21\n");
	if (currentElaborationCoreMode == FULL_FEATURE_ELABORATION_CORE_MODE) {
		StereoExplorer::GetInstance(reprojection3D, dispFloat);
	
		pvm = new PlanViewMap();

		hctp.bigImagePointer = clonedColorBigLeftImage;
		hctp.hStopRunningEvent = hStopRunningEvent;
		hctp.hEventBigImageReady = CreateEvent(NULL, FALSE, FALSE, NULL);
		hctp.hEventFacesReady = CreateEvent(NULL, FALSE, FALSE, NULL);
		hctp.cascade = (CvHaarClassifierCascade *) cvLoad(mainFaceDatabase.GetHaarClassifierFilename());
		hctp.imagePixelMapping = new CvPoint[height*width];

		hHaarThread = chBEGINTHREADEX(NULL, 0, HaarThread, (LPVOID) &hctp, 0, NULL);

		if (saveToFile) {
			hSaveToFile = CreateFileA(saveToFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		}
	}
	//printf("22\n");
	asr->SubscribeEvent(STEREO_IMAGE_READY, hStereoEvent);
	//printf("23\n");
	int faceRecognitionPhase = 0;

	HANDLE hEventsToWait[] = {hStereoEvent, hStopRunningEvent};
	//printf("24\n");
	StereoRig::StereoTimestampsStruct lastTimestamps; lastTimestamps.stereoTimeStamp = GetTickCount();

	//printf("ElaborationCore: wait for multiple Objects\n");
	while (WaitForMultipleObjects(2, hEventsToWait, FALSE, INFINITE) == WAIT_OBJECT_0) {
		//printf("while (WaitForMultipleObjects\n");
		IplImage * leftImage, * rightImage;
		//printf("ElaborationCore: AcquireReadLock\n");
		asr->StereoLock.AcquireReadLock();
			//printf("25 2\n");
			DWORD initialCount = GetTickCount();
			//printf("25 3\n");
			asr->GetStereoImages(&leftImage, &rightImage, false);
			//printf("25 4\n");
			cvCopy(leftImage, clonedColorImages[LEFT_SIDE_CAMERA]);
			//printf("25 5\n");
			cvCopy(rightImage, clonedColorImages[RIGHT_SIDE_CAMERA]);
			//printf("25 6\n");
			StereoRig::StereoTimestampsStruct newTimestamps = asr->GetStereoImages(&leftImage, &rightImage, true);
			//printf("25 7\n"); 
			DWORD dt = newTimestamps.stereoTimeStamp - lastTimestamps.stereoTimeStamp;
			//printf("25 8\n");
			lastTimestamps = newTimestamps;
			//printf("25 9\n");
			if (faceRecognitionPhase == 0)
				cvCopy(leftImage, clonedColorBigLeftImage);
			//printf("25 10\n");
			asr->SetFrameAsConsumed();
			//printf("25 11\n");
		asr->StereoLock.ReleaseReadLock();
		//printf("25 12\n");
		asr->RequestMoreFrames();
		//printf("25 13\n");
		//A lot of algorithms work with gray scale version of input images
		cvCvtColor(clonedColorImages[LEFT_SIDE_CAMERA], grayScaleImage[LEFT_SIDE_CAMERA], CV_BGR2GRAY);
		cvCvtColor(clonedColorImages[RIGHT_SIDE_CAMERA], grayScaleImage[RIGHT_SIDE_CAMERA], CV_BGR2GRAY);
		//printf("25 14\n");
		//We need the image border to compute activities
		cvSobel(grayScaleImage[LEFT_SIDE_CAMERA], vEdge, 0, 1, 3);
		cvSobel(grayScaleImage[LEFT_SIDE_CAMERA], hEdge, 1, 0, 3);
		//printf("25 15\n");
		//Disparity Map and normalization to gray scale image
		cvFindStereoCorrespondenceBM(grayScaleImage[LEFT_SIDE_CAMERA], grayScaleImage[RIGHT_SIDE_CAMERA], disp, BMState);
		/*this->BMState->operator()(cv::Mat(grayScaleImage[LEFT_SIDE_CAMERA]), cv::Mat(grayScaleImage[RIGHT_SIDE_CAMERA]),
			cv::Mat(disp));*/
		/*this->SGBMState->SADWindowSize = BMState->SADWindowSize;
		this->SGBMState->minDisparity = BMState->minDisparity;
		this->SGBMState->numberOfDisparities = BMState->numberOfDisparities;
		this->SGBMState->uniquenessRatio = BMState->uniquenessRatio;
		this->SGBMState->preFilterCap = BMState->preFilterCap;
		this->SGBMState->operator()(cv::Mat(grayScaleImage[LEFT_SIDE_CAMERA]), cv::Mat(grayScaleImage[RIGHT_SIDE_CAMERA]),
			cv::Mat(disp));*/
		//printf("25 16\n");
		cvScale(disp, dispFloat, 0.0625);
		//printf("25 17\n"); 
		cvReprojectImageTo3D(dispFloat, reprojection3D, StereoCalibration::GetInstance()->Q);
		//printf("25 18\n");
		modelLock.AcquireWriteLock();
		//printf("25 19\n");
		//We update the foreground info
		CvSeq *blobs = bm->UpdateForeground(clonedColorImages[LEFT_SIDE_CAMERA], vEdge, hEdge, initPhase);
		//printf("initPhase %d\n", initPhase);
		//printf("25 20\n");
		if (pvm) {
			//printf("if (pvm)\n");
			if (!initPhase && faceRecognitionPhase == 0) {
				pvm->UpdatePlanViewMap(reprojection3D, bm->GetFindConnectedComponents()->GetConnectedComponents(),
					disp, clonedColorImages[LEFT_SIDE_CAMERA], hctp.imagePixelMapping);
				faceRecognitionPhase = 1;
			} else {
				pvm->UpdatePlanViewMap(reprojection3D, bm->GetFindConnectedComponents()->GetConnectedComponents(),
					disp, clonedColorImages[LEFT_SIDE_CAMERA], NULL);
			}
			pvm->IdentifyVisibleSubjects();
			if (!initPhase) {
				//printf("if (!initPhase)\n");
				if (faceRecognitionPhase == 1) {
					pvm->Track_em(&hctp.tfc, dt);
					faceRecognitionPhase = 2;
					SetEvent(hctp.hEventBigImageReady);
				} else {
					pvm->Track_em(NULL, dt);
				}
				DWORD haarThreadState = WaitForSingleObject(hctp.hEventFacesReady, 0);
				if (haarThreadState == WAIT_OBJECT_0) {
					pvm->InterpolateIdentity(hctp.tfc);
					faceRecognitionPhase = 0;
				}
				if (GetCurrentlySelectedTest()->IsTestStarted()) {
					RegisterNewSoftwareMeasurements(pvm->trackedPersons, initialCount - GetCurrentlySelectedTest()->GetStartTime());
				}
				//if (serv != NULL)
					//serv->NotifyClients(pvm->trackedPersons);
				//printf("tracked persons %d\n",pvm->trackedPersons.size());
				updateTrackedPeople(pvm->trackedPersons);				
				if (saveToFile && hSaveToFile != INVALID_HANDLE_VALUE) {
					char lineToWrite[256]; DWORD writtenBytes;
					for (std::vector<TrackedObject *>::const_iterator it = pvm->trackedPersons.begin(); it != pvm->trackedPersons.end(); it++) {
						if ((*it)->type != NEW_OBJECT) {
							int toWriteBytes = sprintf_s(lineToWrite, "%d %f %f %lu\r\n", (*it)->ID, (*it)->kalmanFilter->state_post->data.fl[0],
								(*it)->kalmanFilter->state_post->data.fl[1], dt);
							WriteFile(hSaveToFile, lineToWrite, toWriteBytes, &writtenBytes, NULL);
						}
					}
				}
			}
		}
		//printf("25 21\n");
		modelLock.ReleaseWriteLock();
		//printf("25 22\n");
		modelLock.AcquireReadLock();
		//printf("25 23\n");
		DWORD currWait = GetTickCount() - initialCount;
		//printf("25 24\n");
		processingStageOutput.showImage(FILTERED_FOREGROUND_STAGE, bm->GetFindConnectedComponents()->GetConnectedComponents());
		//printf("25 25\n");
		cvNormalize(disp, temp1channel, 0, 255, CV_MINMAX);
		//printf("25 26\n");
		processingStageOutput.showImage(DISPARITY_STAGE, temp1channel);
		//printf("25 27\n");
		processingStageOutput.showImage(RAW_FOREGROUND_STAGE, bm->GetForegroundSegmentationPtr()->GetCurrentForegroundPtr());
		//printf("25 28\n");
		cvConvert(bm->GetEdgeIntensityModelPtr()->GetDifferential(), temp1channel);
		//printf("25 29\n");
		processingStageOutput.showImage(EDGE_ACTIVITY_STAGE, temp1channel);
		//printf("25 30\n");
		cvConvert( bm->GetColorModelPtr()->GetAveragePtr(), temp3channel);
		//printf("25 31\n");
		processingStageOutput.showImage(BACKGROUND_STAGE, temp3channel);
		//printf("25 32\n");
		if (pvm) {
			processingStageOutput.showImage(PLANVIEW_OCCUPANCY_STAGE, pvm->GetOccupancyImage());

			processingStageOutput.showImage(PLANVIEW_HEIGHT_STAGE, pvm->GetHeightImage());
		}
		//printf("25 33\n");
		modelLock.ReleaseReadLock();
		//printf("25 34\n");
		wchar_t text_buf[20];
		swprintf_s(text_buf, 20, L"%u ms", currWait);
		PostMessage(si->GetStatusBar(), (UINT) SB_SETTEXT, (WPARAM) (INT) 0, (LPARAM) text_buf);
		//printf("25 35\n");
	}

	if (pvm) {
		WaitForSingleObject(hHaarThread, INFINITE);
		ResetEvent(hStopRunningEvent);
		CloseHandle(hHaarThread);
		if (saveToFile && hSaveToFile != INVALID_HANDLE_VALUE)
			CloseHandle(hSaveToFile);
		CloseHandle(hctp.hEventBigImageReady);
		CloseHandle(hctp.hEventFacesReady);
		cvReleaseHaarClassifierCascade(&hctp.cascade);
		delete[] hctp.imagePixelMapping;

		delete StereoExplorer::GetInstance();
		delete pvm;
		pvm = NULL;
	}
	
	//printf("26\n");
	cvReleaseImage(&grayScaleImage[0]);
	cvReleaseImage(&grayScaleImage[1]);
	cvReleaseImage(&vEdge);
	cvReleaseImage(&hEdge);
	cvReleaseImage(&clonedColorImages[LEFT_SIDE_CAMERA]);
	cvReleaseImage(&clonedColorImages[RIGHT_SIDE_CAMERA]);
	cvReleaseImage(&clonedColorBigLeftImage);
	cvReleaseMat(&disp);
	cvReleaseMat(&reprojection3D);
	cvReleaseImage(&temp3channel);
	cvReleaseImage(&temp1channel);
	//printf("27\n");
	asr->UnSubscribeEvent(STEREO_IMAGE_READY, hStereoEvent);
	//printf("28\n");
	delete bm;
	bm = NULL;
	//printf("29\n");
	ApplicationWorkFlow::GetInstance()->UpdateSystemState(ELABORATION_ENDED);
	//printf("30\n");
}

bool ElaborationCore::StopPreprocedure() {
	SetEvent(hStopRunningEvent);
	return true;
}

void ElaborationCore::SaveCurrentState(CvArrStorage &stateContainer) {
	modelLock.AcquireReadLock();
	bm->GetColorModelPtr()->cloneColorModel(stateContainer);
	modelLock.ReleaseReadLock();
}

void ElaborationCore::LoadSavedState(CvArrStorage &stateContainer) {
	modelLock.AcquireWriteLock();
	if (pvm)
		pvm->Reset();
	
	int height, width;
	si->GetStereoRig()->GetAcquisitionProperties().GetResolution(&height, &width, true);
	delete bm;
	bm = new BackgroundModeling(height, width);
	printf("replaceColorModel\n");
	bm->GetColorModelPtr()->replaceColorModel(stateContainer);
	modelLock.ReleaseWriteLock();
}

bool ElaborationCore::ActivateSaveToFileMode(const char *fileName) {
	if (this->IsRunning())
		return false;
	saveToFile = true;
	strcpy_s(saveToFilename, fileName);
	return true;
}