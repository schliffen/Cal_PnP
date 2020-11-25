/*
advanced camera calibration
*/
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "include/Cfg.h"
#include "include/CamCal.h"


int main(int argc, char *argv[]) {
	cv::Mat oImgFrm;
	cv::Mat_<float> oCamInParam;
	cv::Mat_<float> ovDistCoeff;
	CCamCal oCamCal;
	CCfg oCfg;
	// read configuration file
	if (2 < argc)
	{
		std::cout << "usage: " << argv[0] << " <cfg_file_path>" << std::endl;
		return 0;
	}
	else if (2 == argc)
		oCfg.ldCfgFl(argv[1]);
	else
		oCfg.ldCfgFl(NULL);

	// read frame image
	oImgFrm = cv::imread(oCfg.getInFrmPth(), cv::IMREAD_COLOR);
	// resize frame if necessary
	if (0 < oCfg.getRszFrmHei())
	{
		cv::Size oFrmSz((((float)oImgFrm.size().width / (float)oImgFrm.size().height) * oCfg.getRszFrmHei()), oCfg.getRszFrmHei());
		cv::resize(oImgFrm, oImgFrm, oFrmSz);
	}
	// correct camera distortion
	if (oCfg.getCalDistFlg())
	{
		cv::Mat oImgUndist;
		oCamInParam = oCfg.getCalIntMat();
		ovDistCoeff = oCfg.getCalDistCoeffMat();
		cv::undistort(oImgFrm, oImgUndist, oCamInParam, ovDistCoeff);
		oImgFrm = oImgUndist.clone();
	}

	// initialize the camera calibrator
	oCamCal.initialize(oCfg, oImgFrm);

	// run camera calibration
	oCamCal.process();

	// output calibration results
	oCamCal.output();






	std::cout << "Finished! \n";
	return 0;
}