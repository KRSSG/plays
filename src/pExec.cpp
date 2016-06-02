#include <list>
#include "pExec.h"
#include "robot.h"
#include  "tactics/tactic.h"

using namespace std;
//using namespace HAL;

namespace Strategy
{
  PExec::PExec(krssg_ssl_msgs::BeliefState* state,ros::NodeHandle& n) :
    NaivePS(*state),state1(*state)
  {
     //state1=state;
    for (int botID = 0; botID < HomeTeam::SIZE; ++botID)
    {
      //tactic[botID] = new  Tactic();
      robot[botID]    = new Robot(botID,n);
    }
  } // PExec

  PExec::~PExec()
  {
    for (int botID = 0; botID < HomeTeam::SIZE; ++botID)
    {
      //delete tactic[botID];
      delete robot[botID];
    }
  } // ~PExec

//*********************this function assigns roles to bots **********************************************

  void PExec::assignRoles(void)
  {
    if (playID == PlayBook::None)
    {
      return;
    }
    Play* currPlay = playList[playID];
    bool goodBot[6] = {true, true, true, true, true, true};
    int goodBotCount = 6;
    // Initialization
    list<int> freeBots;
    for (int botID = 0; botID < HomeTeam::SIZE; ++botID) // Iterating over all bots - making them all free for role allocation
    {
      if(goodBot[botID])
      freeBots.push_back(botID);
    }
    for (int roleIdx = 0; roleIdx < HomeTeam::SIZE; ++roleIdx) // Iterating over all roles
    {
      if (currTacticIdx < currPlay->roleList[roleIdx].size()) // Tactic exists for the current role iteration
      {
        std::string    tID     = currPlay->roleList[roleIdx][currTacticIdx].first;
        Tactic::Param tParam  = currPlay->roleList[roleIdx][currTacticIdx].second;
        
        int           bestBot;

        /* Assign the Goalie role to always the bot id 0 */
        //if(roleIdx != 0)
        bestBot = robot[roleIdx]->curTactic.get()->chooseBestBot(state1,freeBots, tParam);

        freeBots.remove(bestBot);
        
       //  // Saving the current tactic to be executed by the bot selected
       //  currTactic[bestBot]   = currPlay->roleList[roleIdx][currTacticIdx];
       // // Util::Logger::toStdOut("Updating tactics\n");
       //  // Updating the tactic of the selected bot
        robot[bestBot]->tID    = tID;
        robot[bestBot]->tParamJSON =TacticFactory::instance()->Create(tID, 0)->paramToJSON(tParam);
        //robot[bestBot]->tStateSh = Tactic::INIT;
      }
      if(roleIdx == goodBotCount-1) {
        for (int botID = 0; botID < HomeTeam::SIZE; ++botID) // Iterating over all bots - making them all free for role allocation
        {
          if(!goodBot[botID])
          freeBots.push_back(botID);
        }
      }
    }
    //*returnIndx=currTacticIdx;
    //Logger::toStdOut("Assigned roles for tactic index %d\n", currTacticIdx);
  } // asisgnRoles

  bool PExec::canTransit(void)
  {
    if (playID == PlayBook::None)
    {
      return true;
    }

    if (currTacticIdx >= playList[playID]->maxTacticsPerRole)
    {
      return false;
    }

    Play* currPlay = playList[playID];

    int numActiveTactics = 0;

    for (int roleID = 0; roleID < HomeTeam::SIZE; ++roleID)
    {
      std::string tID       = currPlay->roleList[roleID][currTacticIdx].first;
      Tactic*    selTactic = robot[roleID]->curTactic.get();

      if (selTactic->isActiveTactic())
      {
        ++numActiveTactics;

          if (!selTactic->isCompleted(state1))
          {
            // If there is at least one incomplete active tactic, then cannot transit
            //Util::Logger::toStdOut("Active tactic not completed  : %d %d\n",roleID,selTactic->tState);
            return false;
          }
          else
          {
            //Util::Logger::toStdOut("Active tactic COMPLETED : %d %d\n",roleID,selTactic->tState);
          }
      }
    }
    

    if (numActiveTactics > 0)
    {
      //Util::Logger::toStdOut("ACTIVE TACTIC COMPLETED. TRY Transit");
      return true;  // There is atleast 1 active tactic and all of them have completed hence can transit
    }
    else
    {
      // There are no active tactics in this iteration and hence all the tactics must be completed in order to transit
      for (int roleID = 0; roleID < HomeTeam::SIZE; ++roleID)
      {
        std::string tID       = currPlay->roleList[roleID][currTacticIdx].first;
        Tactic*    selTactic = robot[roleID]->curTactic.get();

        if (!selTactic->isCompleted(state1))
        {
          // If there is at least one incomplete tactic, then cannot transit
          return false;
        }
      }
    }
    //Util::Logger::toStdOut("Can Transit returning true.");
    return true;
  } // canTransit

  bool PExec::tryTransit(void)
  {
    if (currTacticIdx + 1 < playList[playID]->maxTacticsPerRole)
    {
      ++currTacticIdx;
      return true;
    }
    //Logger::toStdOut("Try Transit returning false\n");
    return false;
  } // tryTransit

  Robot** PExec::selectPlay(void)
  {
    select();
    playResult = Play::NOT_TERMINATED;
    currTacticIdx = 0;
    assignRoles();   //####################### why assign roles here ? ###########################
    return robot;
  } // selectPlay

  Robot** PExec::executePlay()
  {
    if (canTransit() && tryTransit())
    {
      assignRoles();
    }
    return robot;
  } // executePlay

  void PExec::evaluatePlay(void)
  {
    if (playID == PlayBook::None)
    {
      return;
    }
    
    updateWeights(playResult);
  } // evaluatePlay

  bool PExec::playTerminated(void)
  {
    if (playID == PlayBook::None)
    {
      //Util::Logger::toStdOut("Last Play was None!\n");
      ROS_INFO("Last Play was None");
      return true;
    }
    if(playList[playID]->timedOut())
    {
      //Util::Logger::toStdOut("Play Timed out.\n");
      ROS_INFO("Play Timed out");
      playResult = Play::TIMED_OUT;
      return true;
    }
    /* The completion of a play is defined to be the completion of all the tactics assigned
     * to all the bots. Since this information is available in the Tactic class and not known
     * to the Play class, the evaluation of the completion of the play is done here
     * instead of being done in the done() function of the individual plays
     */
    if (playCompleted())
    {
      ROS_INFO("Play Completed");
      playResult = Play::COMPLETED;
      return true;
    }

    Play::Result result = playList[playID]->done();
    if (result == Play::NOT_TERMINATED)
    {
      ROS_INFO("Play Not terminated");
      return false;
    }
    else
    {
      playResult = result;
      return true;
    }
  } // playTerminated

  bool PExec::playCompleted(void)
  {
    return canTransit() && !tryTransit();
  }
} // namespace Strategy