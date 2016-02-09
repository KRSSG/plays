#ifndef PLAY_HPP
#define PLAY_HPP

#include <utility>
#include <vector>
#include <string>
#include "timer.h"
#include "ps.h"
#include "tactics/tactic.h"
// #include "logger.h"
// #include "ssl_robot/robot.h"
#include <krssg_ssl_msgs/BeliefState.h>
#define SIZE 6 //SIZE of hometeam
using namespace std ;
using krssg_ssl_msgs::BeliefState;
namespace Strategy
{
  class Play
  {
    friend class PS;

  public:
    enum Result
    {
      SUCCESS_LOW ,//0
      SUCCESS_MED,//1
      SUCCESS_HIGH,//2
      FAILURE_LOW,//3
      FAILURE_MED,//4
      FAILURE_HIGH,//5
      COMPLETED,//6
      TIMED_OUT,//7
      NOT_TERMINATED//8
    }; // enum Result

    //This function***************************
    inline bool timedOut()
    {
      int tm = timer.split();
      if((tm - lastRoleReassignTime) > roleReassignPeriod) {
           lastRoleReassignTime = tm;
           shouldRolesReassign  = true;
         }
    
      return (tm >= timeoutPeriod);
    }

    enum PlayType
    {
      PLAYTYPE_YES,
      PLAYTYPE_NO,
      PLAYTYPE_DONTCARE
    };
  
  private:
    // Default timeout period of any play
    static const int DEFAULT_TIMEOUT_PERIOD = 60;

    Timer timer;
    int         timeoutPeriod; // in milliseconds
    int         roleReassignPeriod;
    int         lastRoleReassignTime;

    // Sets the timeout period for the play in seconds
    inline void startTimer(void)
    {
      timer.start();
      int tm = timer.split();
      lastRoleReassignTime = tm - 2*roleReassignPeriod; //so that 1st call to timedOut gives shouldRolesReassign = true. 2* just to be safe
    }
  protected:
    Result result;  // Assigned when the play terminates    
    const BeliefState &state;

    // Sets the timeout period of the play in seconds
    void setTimeout(double timeoutPeriod, int roleReassignPeriod = 1)
    {
      this->timeoutPeriod = (timeoutPeriod * 1000);
      this->roleReassignPeriod = roleReassignPeriod * 1000;
    }

    inline void computeMaxTacticTransits()
    {
      maxTacticsPerRole = roleList[0].size();
      for (int roleIdx = 1; roleIdx < SIZE; ++roleIdx)
      {
        int sz = roleList[roleIdx].size();
        if (roleList[roleIdx].size() > maxTacticsPerRole)
        {
          maxTacticsPerRole = sz;
        }
      }
    }

   public:
        
      bool shouldRolesReassign;
      virtual void reevaluateRoleParams() {} //Function to reevaluate the params for each role in every call of executePlay() in pExec
   
      inline Play(const BeliefState &state) :
      timeoutPeriod(Play::DEFAULT_TIMEOUT_PERIOD),
      result(NOT_TERMINATED),
      state(state),
      weight(1.0f),
      maxTacticsPerRole(0)
    {
      name = "";
      setTimeout(60);
    }

    // Name of the play
    std::string name;
    PlayType PositionPlay;
    PlayType AttackPlay;
    
    // Weight of the play - Measure of its applicability in the current scenario
    float weight;

    // roleList is a vector of pairs of Tactic ID and Tactic Parameter

    std::vector<std::pair<Tactic::ID, Tactic::Param> > roleList[SIZE];  //***************************
    int assignedBot[SIZE];
    unsigned int maxTacticsPerRole;

    // Returns true if the play is applicable otherwise false
    // virtual bool applicable(void) const {
    //   if((state.pr_gameRunning && this->PositionPlay == PLAYTYPE_YES)|| (!state.pr_gameRunning && this->PositionPlay == PLAYTYPE_NO)) {
    //     return false;
    //   }
    //   return true;
    // }

    /* Returns one of SUCCESS/FAILURE/ABORTED/COMPLETED as applicable if the play terminates
     * otherwise NO_TERMINATION
     */
    virtual Result done(void) const = 0;
  }; // class Play
} // namespace Strategy

#endif // PLAY_HPP
