#include "ros/ros.h"
#include "krssg_ssl_msgs/BeliefState.h"
#include <krssg_ssl_msgs/SSL_DetectionFrame.h>
#include <geometry_msgs/Pose2D.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "pExec.h"

using namespace std;

ros::Subscriber beliefstate ;


/* 
** Call Back function to get belief state data **
*/

int main(int argc, char **argv)
{

	ros::init(argc, argv, "play_node");
     
    BeliefState &state; 
    PExec       pExec(&state);

    if(state.refreeUpdated) 
        {		
          state.refreeUpdated = false;
          //Util::Logger::toStdOut("Command From Refee.. Reselecting play..\n");
          pExec.selectPlay(NULL);
        }
        else if (pExec.playTerminated())
        {
          //Util::Logger::toStdOut("*************Play terminated.Select new play\n*********************");
          pExec.evaluatePlay();
          pExec.selectPlay(NULL);
        }
		//printf("executing play!\n");
        pExec.executePlay();
      


	ros::spin() ;
	return 0 ;
}