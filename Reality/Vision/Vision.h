#pragma once

////////////////////////////////////////////////////////////////////
//  vision.h
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include <iostream>

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"
#include "ImmortalsProtocols.pb.h"

#include "VisionSetting.h"

#include "PracticalSocket.h"
#include "FilteredObject.h"
#include "MedianFilter.h"
#include "MATHS_REGRESSION_PARABOLIC.h"
#include "WorldState.h"

#ifndef INT_MAX
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif

#ifndef POWED_DIS
#define POWED_DIS(a,b,c,d) (((a-c)*(a-c))+((b-d)*(b-d)))
#endif

#define PREDICT_STEPS 8.0f

#define MAX_BALLS 10
#define MAX_BALL_NOT_SEEN 10

#define MAX_ROBOT_NOT_SEEN 20

#define MERGE_DISTANCE 5000

#define MAX_INCOMING_PACKET_SIZE 1000

#define BALL_BUFFER_FRAMES 30

VisionSetting * _visionSetting ( bool , std::string , short , std::string , short , bool = true , bool = true );

/**
Class VisionModule : Captures the vision packet from the network, and sends it to the rest
of the AI class.
@author Ali Salehi ( Rewritten from original 2009 source code )
@version 0.1
June 2010
Changes include : 
**/

class VisionModule
{
	public:

		VisionModule ( VisionSetting * );

		VisionSetting * GetSetting ( void );

		bool recievePacket ( void );
		bool connectToVisionServer ( const std::string & , const unsigned short );
		void ProcessVision ( WorldState * );
		bool isConnected ( void );

		void ProcessRobots ( WorldState * );
		int ExtractBlueRobots ( void );
		int ExtractYellowRobots ( void );
		int MergeRobots ( int num );
		void FilterRobots ( int num , bool own );
		void predictRobotsForward( WorldState * );
		void SendStates ( WorldState * );

		void SendGUIData ( WorldState * , AI_Debug & );

		void ProcessBalls ( WorldState * );
		int ExtractBalls ( void );
		int MergeBalls ( int num );
		void FilterBalls ( int num , WorldState * );
		void predictBallForward( WorldState * );
		void calculateBallHeight ( void );

	private:

		float ballBufferX[BALL_BUFFER_FRAMES];
		float ballBufferY[BALL_BUFFER_FRAMES];
		int ballBufferIndex;

		//TVec2 lastShootPosition;
		//TVec2 prevBallVel;


		Parabolic ballParabolic;

		FilteredObject ball_kalman;
		FilteredObject robot_kalman[2][MAX_ROBOTS];

		MedianFilter<float> AngleFilter[2][MAX_ROBOTS];
		float rawAngles[2][MAX_ROBOTS];

		VisionSetting * setting;
		bool connected;

		int ball_not_seen;
		int robot_not_seen[2][MAX_ROBOTS];

		SSL_DetectionBall lastRawBall;

		RobotState robotState[2][MAX_ROBOTS];

		char incoming_buffer[MAX_INCOMING_PACKET_SIZE];

		UDPSocket * VisionUDP;
		UDPSocket * GUIUDP;

		robotDataMsg robotPacket[2][MAX_ROBOTS];

		SSL_WrapperPacket packet;
		SSL_DetectionFrame frame[2];
		SSL_DetectionBall d_ball[MAX_BALLS*2];
		SSL_DetectionRobot robot[MAX_ROBOTS*2];

		bool packet_recieved[2];

};
