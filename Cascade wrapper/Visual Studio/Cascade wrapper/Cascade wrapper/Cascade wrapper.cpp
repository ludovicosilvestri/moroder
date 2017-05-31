// Cascade wrapper.cpp: definisce le funzioni esportate per l'applicazione DLL.
//

#include "stdafx.h"


__declspec(dllexport) int OpenCamera(int16 *CamHandle, int16 *bitdepth) // sets up and open the camera, giving back the handle
{
	rs_bool guard; // used to check PVCAM functions output
	int16 NrOfCameras; // total number of cameras found
	char CameraName[CAM_NAME_LEN]; // string containing the name of the camera

	// initialize PVCAM
	if (PV_OK != pl_pvcam_init())
	{
		return(1); // error: unable to initialize PVCAM
	}

	// get total number of cameras
	guard = pl_cam_get_total(&NrOfCameras);
	if ((guard != PV_OK) || (NrOfCameras == 0))
	{
		return(2); // error: no cameras found
	}

	// get PVCAM-name of camera 0
	if (PV_OK != pl_cam_get_name(0, CameraName))
	{
		return(3); // error: camera name error
	}

	if (PV_OK != pl_cam_open(CameraName, CamHandle, OPEN_EXCLUSIVE))
	{
		return(4); // error: unable to open camera
	}

	// set readout mode
	PL_PMODES readout = PMODE_FT;
	if (PV_OK != pl_set_param(*CamHandle, PARAM_PMODE, &readout))
	{
		return(5); // error: cannot set readout mode to frame transfer
	}

	//int16 bitdepth = 16;
	// set bit depth
	if (PV_OK != pl_get_param(*CamHandle, PARAM_BIT_DEPTH, ATTR_CURRENT, (void *)bitdepth))
	{
		return(55); // error: cannot set 16 bit depth
	}

	return(0);
}

__declspec(dllexport) int SetGainAndSpeed(int16 CamHandle, int16 ADCGain, int16 MultGain, int EM_on) // speed currently not supported
{
	// set adc gain
	if (PV_OK != pl_set_param(CamHandle, PARAM_GAIN_INDEX, &ADCGain))
	{
		return(6); // error: cannot set ADC Gain
	}

	// set multiplication gain
	if (PV_OK != pl_set_param(CamHandle, PARAM_GAIN_MULT_FACTOR, &MultGain))
	{
		return(7); // error: cannot set Multiplication Gain
	}

	// set the right readout port
	PL_READOUT_PORTS readoutPort;
	if (EM_on == 1)
		readoutPort = READOUT_PORT_0; // EM port
	else
		readoutPort = READOUT_PORT_1; // normal port
	
	if (PV_OK != pl_set_param(CamHandle, PARAM_READOUT_PORT, &readoutPort))
	{
		return(8); // error: cannot set readout port
	}

	return(0);
}

__declspec(dllexport) int SetTemperature(int16 CamHandle, int16 Temperature)
{
	// set setpoint temperature
	if (PV_OK != pl_set_param(CamHandle, PARAM_TEMP_SETPOINT, &Temperature))
	{
		return(9); // error: cannot set Temperature
	}

	return(0);
}

__declspec(dllexport) int SetClearAndShutter(int16 CamHandle, int32 ClearMode, UINT16 ClearCycles, int32 ShutterMode)
{
	// set clear mode
	if (PV_OK != pl_set_param(CamHandle, PARAM_CLEAR_MODE, &ClearMode))
	{
		return(10); // error: cannot set Clear Mode
	}

	// set # of clear cycles
	uns16 cycles = (uns16)ClearCycles;
	if (PV_OK != pl_set_param(CamHandle, PARAM_CLEAR_CYCLES, &cycles))
	{
		return(11); // error: cannot set # of Clear Cycles
	}

	// set shutter mode
	if (PV_OK != pl_set_param(CamHandle, PARAM_SHTR_OPEN_MODE, &ShutterMode))
	{
		return(12); // error: cannot set Shutter Open Mode
	}

	return(0);
}

__declspec(dllexport) int GetFrameSize(int16 CamHandle, UINT16 *xSize, UINT16 *ySize)
{
	if (PV_OK != pl_get_param(CamHandle, PARAM_SER_SIZE, ATTR_CURRENT, (void *)xSize))
	{
		return(500); // can't read x resolution
	}

	if (PV_OK != pl_get_param(CamHandle, PARAM_PAR_SIZE, ATTR_CURRENT, (void *)ySize))
	{
		return(501); // can't read y resolution
	}

	return(0);
}

__declspec(dllexport) int StartAcquisition(int16 CamHandle, UINT32 Exposure, int16 ExpMode, int16 CircularMode, UINT16 xSize, UINT16 ySize, UINT16 *Buffer, UINT16 BufferFrames, int *dimensione)
{
	rgn_type Region;
	Region.s1 = 0;
	Region.s2 = (uns16)xSize - 1;
	Region.sbin = 1;
	Region.p1 = 0;
	Region.p2 = (uns16)ySize - 1;
	Region.pbin = 1;
	uns32 ExpSize;

	if (PV_OK != pl_exp_setup_cont(CamHandle, 1, &Region, ExpMode, (uns32)Exposure, &ExpSize, CircularMode))
	{
		return(7000); // error: could not set up acquisition
	}
	uns32 dim = BufferFrames * ExpSize / sizeof(uns16);
	*dimensione = (int)dim;
	uns16 *LocalBuffer = new (std::nothrow) uns16[dim];
	if (LocalBuffer == NULL)
	{
		return(7001); // error: could not allocate memory
	}

	if (PV_OK != pl_exp_start_cont(CamHandle, (void *)LocalBuffer, dim))
	{
		return(7002); // error: could not start acquisition
	}
	
	Buffer = (UINT16*)LocalBuffer;

	return (0);
}

__declspec(dllexport) int GetFrame(int16 CamHandle, UINT16 *Frame, UINT16 xSize, UINT16 ySize)
{
	int size = xSize * ySize;
	uns16 *local_frame;
	//local_frame = new(std::nothrow) uns16[size];


	if (PV_OK != pl_exp_get_latest_frame(CamHandle, (void **)&local_frame))
	{
		return(7005); //unable to access frame
	}

	int i;
	for (i = 0; i < size; i++)
	{
		Frame[i] = (UINT16)local_frame[i];
	}

	//delete[] local_frame;

	return(0);
}

__declspec(dllexport) int StopAcquisition(int16 CamHandle, UINT16 *Buffer)
{
	if (PV_OK != pl_exp_stop_cont(CamHandle, CCS_CLEAR))
	{
		return(7006); // error: cannot stop the camera
	}

	delete[] Buffer;
	return(0);
}

__declspec(dllexport) int CloseCamera(int16 CamHandle)
{
	if (PV_OK != pl_cam_close(CamHandle))
	{
		return(1000); // error while closing camera
	}

	if (PV_OK != pl_pvcam_uninit())
	{
		return(1001); // error while unloading PVCAM
	}

	return(0);
}

__declspec(dllexport) int IsFrameAvaliable(int16 CamHandle, int *available)
{
	int16 status;
	uns32 byte_cnt;
	uns32 buffer_cnt;

	pl_exp_check_cont_status(CamHandle, &status, &byte_cnt, &buffer_cnt);

	if (status == FRAME_AVAILABLE)
		*available = 1;
	else
		*available = 0;
	
	if (status == READOUT_FAILED)
	{
		return(90001); // readout failed error
	}

	return(0);
}