// DO NOT EDIT: This file is auto-generated

#include <queue>
#include <cstdlib>
#include <utility>
#include "playBook.h"
#include "play.hpp"
/*
#include "pNone.hpp"
#include "pTestPlay.hpp"
#include "pSetPosition.hpp"
#include "pOurGoalKick.hpp"
#include "pOppGoalKick.hpp"
#include "pStop.hpp"
#include "pPositionOurFreeKick.hpp"
#include "pPositionOppFreeKick.hpp"
#include "pPositionOurFreeBall.hpp"
#include "pPositionOppFreeBall.hpp"
#include "pPositionOurKickoff.hpp"
#include "pPositionOppKickoff.hpp"
#include "pPositionOurPenalty.hpp"
#include "pPositionOppPenalty.hpp"
#include "pPositionOurGoalKick.hpp"
#include "pPositionOppGoalKick.hpp"
#include "pPenaltyOur.hpp"
#include "pPenaltyOpp.hpp"
#include "pSuperOffense.hpp"
#include "pOffense1.hpp"
#include "pOffense2.hpp"
#include "pSuperDefense.hpp"
#include "pDefense1.hpp"
#include "pDefense2.hpp"#include "krssg_ssl_msgs/BeliefState.h"

#include "pKickoff.hpp"
#include "pPositionGather.hpp"
#include "pTakeGoalKick.hpp"
#include "pOppFreeBall.hpp"
#include "pOurFreeBall.hpp"
#include "pOppFreeKick.hpp"
#include "pOurFreeKick.hpp"
*/
#include "krssg_ssl_msgs/BeliefState.h"

namespace Strategy
{
  PlayBook::PlayBook(const BeliefState* state) : 
    state(state)
  {
    reload();
  } // PlayBook

  PlayBook::~PlayBook()
  { } // ~PlayBook

  void PlayBook::reload(void)
  {
    // Initializing all play objects with NULL
    for (int pID=0; pID<PlayBook::MAX_PLAYS; ++pID)
    {
      playList[pID] = NULL;
    }

    // Inserting all the name-object pairs into the  Play List
    /*
    // Edit it upon inclusion of new play 
    playList[PlayBook::None]                  = new PNone(*state);
    playList[PlayBook::TestPlay]              = new PTestPlay(*state);
    playList[PlayBook::SetPosition]           = new PSetPosition(*state);  
    playList[PlayBook::Stop]                  = new PStop(*state);
    playList[PlayBook::OurGoalKick]           = new POurGoalKick(*state);
    playList[PlayBook::OppGoalKick]           = new POppGoalKick(*state);
    playList[PlayBook::PenaltyOur]						=	new PPenaltyOur(*state);
		playList[PlayBook::PenaltyOpp]						=	new PPenaltyOpp(*state);
    playList[PlayBook::SuperOffense]          = new PSuperOffensePlay(*state);
    playList[PlayBook::Offense1]              = new POffense1Play(*state);
    playList[PlayBook::Offense2]              = new POffense2Play(*state);
    playList[PlayBook::SuperDefense]          = new PSuperDefensePlay(*state);
    playList[PlayBook::Defense1]              = new PDefense1Play(*state);
    playList[PlayBook::Defense2]              = new PDefense2Play(*state);
	  playList[PlayBook::Kickoff]               = new PKickoff(*state);
		playList[PlayBook::PositionOurKickoff]    = new PPositionOurKickoff(*state);
		playList[PlayBook::PositionOppKickoff]    = new PPositionOppKickoff(*state);
		playList[PlayBook::PositionOurPenalty]    = new PPositionOurPenalty(*state);
		playList[PlayBook::PositionOppPenalty]    = new PPositionOppPenalty(*state);
		playList[PlayBook::PositionOurFreeBall]    = new PPositionOurFreeBall(*state);
		playList[PlayBook::PositionOppFreeBall]    = new PPositionOppFreeBall(*state);
		playList[PlayBook::PositionOurFreeKick]    = new PPositionOurFreeKick(*state);
		playList[PlayBook::PositionOppFreeKick]    = new PPositionOppFreeKick(*state);
		playList[PlayBook::PositionOurGoalKick]    = new PPositionOurGoalKick(*state);
		playList[PlayBook::PositionOppGoalKick]    = new PPositionOppGoalKick(*state);
		playList[PlayBook::TakeGoalKick]          = new PTakeGoalKick(*state);
    playList[PlayBook::PositionGather]        = new PPositionGather(*state); 
		playList[PlayBook::OurFreeBall]           = new POurFreeBall(*state);
		playList[PlayBook::OppFreeBall]           = new POppFreeBall(*state);
		playList[PlayBook::OurFreeBall]           = new POurFreeBall(*state);
		playList[PlayBook::OppFreeKick]           = new POppFreeKick(*state);
		playList[PlayBook::OurFreeKick]           = new POurFreeKick(*state);

    */
    
    // Checking if all the play ids have been associated with their corresponding Play objects
    for (int pID=0; pID<PlayBook::MAX_PLAYS; ++pID)
    {
      assert(playList[pID] != NULL); // Play has not been assigned a Play object
    }
  }
  //
  
 
}