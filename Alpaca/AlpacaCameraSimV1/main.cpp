#include "stdafx.h"
#include "CameraSimApp.h"


int main(int argc, char *argv[])
{
	App::CameraSimApp CamApp(argc, argv);

	auto ret = CamApp.init();
	
	return ret ? CamApp.exec() : -1;
}
