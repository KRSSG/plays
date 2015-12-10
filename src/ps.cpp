/* This file contains the PS class(Play Selector) that implements the algorithm
 * to select the play to be implemented in each frame. It uses the probabilistic model 
 * to select the most appropriate play among the list of applicable plays.
 */
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include "ps.h"
#include "playBook.h"
#include "play.hpp"

#include "krssg_ssl_msgs/BeliefState.h"

using namespace std;

#define D_PS 1

  PS::PS(const BeliefState* state) : 
    PlayBook(state),
    playID(PlayBook::None)
  {
    for (int playID=0; playID<PlayBook::MAX_PLAYS; ++playID)
    {
      appl[playID] = false;
      pProb[playID] = 0.0f;
    }
 }

  PS::~PS()
  { }

  void PS::select(void)
  {
    // Find the applicable plays
    const int num_plays = PlayBook::MAX_PLAYS;
    printf("Game Running: %d",state->pr_gameRunning);
    for (int pID = 0; pID < num_plays; ++pID)
    {
      appl[pID] = playList[pID]->applicable();
    }

    float cumWeight = 0.0f;
    for (int pID = 0; pID < num_plays; ++pID)
    {
      if (appl[pID])
      {
        cumWeight += playList[pID]->weight;
      }
    }

    for (int pID = 0; pID < num_plays; ++pID)
    {
      if (appl[pID])
      {
        pProb[pID] = playList[pID]->weight/float(cumWeight);
      }
      else
      {
        pProb[pID] = 0.0f;
      }
    }

    float cumProb = 0.0f;
    float randVal = (float)std::rand()/RAND_MAX + 0.000001f;
    if (randVal >= 1.0f)
    {
      randVal = 0.999999f;
    }

     // playID = PlayBook::PenaltyOur ;
     //playID = PlayBook::PositionOurPenalty;
	 //playID = PlayBook::PositionGather;
    //playID = PlayBook::PositionOurFreeBall;
	 //playID = PlayBook::OurFreeKick;
    //playID = PlayBook::PenaltyOur;
	 //playID = PlayBook::Kickoff ;
	//playID = PlayBook::PositionOurKickoff;
	
	//printf("pop called");
	//playID = PlayBook::Offense1;

    for (int pID = 0; pID < num_plays; ++pID)
    {
      cumProb += pProb[pID];
      if (cumProb >= randVal && appl[pID])
      {
        playID = (PlayID)pID;
        break;
      }
    }


		/*Arpit: treating special case for our penalty kcik and kickoff*/
	  // uncomment when all the plays has been added 
    /*
    if(state->pr_ourKickOffStart) {
			playID = PlayBook::Kickoff;
		} else if(state->pr_ourPenaltyKickStart) {
			playID = PlayBook::PenaltyOur;
		}
   	if (playID == PlayBook::None)
		playID = PlayBook::Offense1;
		*/
		//cout << "yes";
    assert(playID != PlayBook::None); // No play selected

    playList[playID]->startTimer();
  } // select

  /*
  void PS::selectfromStr_Gui(int pID){
	  playID = (Strategy::PlayBook::PlayID)pID;
  }
  */

  