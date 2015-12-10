#ifndef PEXEC_H
#define PEXEC_H

#include <utility>
//#include "naivePS.h"
//#include "expPS.h"
#include "krssg_ssl_msgs/BeliefState.h"
#include "ps.h"
//#include "ssl_robot/robot.h"
// Forward Declarations
using namespace std;
using krssg_ssl_msgs::BeliefState;

/*
namespace Strategy
{
  class BeliefState;
  class Robot;
}
*/

namespace Strategy
{
#ifndef PS_CLASS
# error Macro PS_CLASS has to be defined as the name of one of the derived classed of class PS
#endif

// PS_CLASS
  class PExec : private PS 
  {
  private:

  // Change the private members .. do we need them 

    Play::Result playResult;
    //Robot*       robot[HomeTeam::SIZE];
    //Util::CS*    tacticCS[HomeTeam::SIZE];
		int roleIDMap[HomeTeam::SIZE];   //HomeTeam::SIZE 						// roleIDMap[i] = robot id with ith role.
  public:
    PExec(BeliefState* state);

    ~PExec();

  private:
    // Stores the index to the tactics in all role that is to be executed by the team
    unsigned int currTacticIdx;

    // Current tactic in execution by each bot
    //std::pair<Tactic::ID, Tactic::Param> currTactic[HomeTeam::SIZE];

    void assignRoles(bool isReassign);

    bool canTransit(void);

    bool tryTransit(void);

	public:
  
    void selectPlay(int *pID);
	
	void selectfromGUI(int pID);

    void executePlay(void);

    void evaluatePlay(void);

    bool playTerminated(void);

    bool playCompleted(void);
  }; // class PExec
} // namespace Strategy

#endif // PEXEC_H
