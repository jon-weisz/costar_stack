#ifndef _PLANNING_TOOL
#define _PLANNING_TOOL

// include ROS headers
#include <ros/ros.h>

#include <predicator_msgs/PredicateStatement.h>
#include <predicator_planning/PredicatePlan.h>

#include "predicator.h"

// deque more efficient for long arrays
#include <deque>

namespace predicator_planning {


  struct Planner {

    /**
     * SearchPose
     * Struct holding information on how good this RobotState is.
     * We use these to construct the 
     */
    struct SearchPose {
      RobotState *state;
      std::vector<bool> matching;
      std::vector<double> heuristics;
      unsigned int count_best; // number of heuristics for which this is best
      unsigned int count_met; // number of goal conditions met
      double hsum; // sum of negative heuristics
      double cost; // minimize distance

      SearchPose *parent; // which one comes before this in the path
      SearchPose *child; // which one comes next

      // default constructor
      SearchPose();

      // initialize parents, variables
      SearchPose(std::deque<SearchPose *> &search,
                 RobotState *state);

      // update with information from the context
      bool checkPredicates(PredicatePlan::Request &req, PredicateContext *context, unsigned int idx, bool &goals_reached);
    };

    // context contains information about the world and will produce new predicates
    PredicateContext * context;
    ros::ServiceServer planServer;

    int verbosity;
    unsigned int max_iter; // maximum iterations to attempt to find destination
    double search_volume; // random volume around high-value (good) nodes to search in
    double step; // distance to move
    double chance; // percent of the time to move at random
    double skip_distance; // used to smooth trajectories

    Planner(PredicateContext *context, unsigned int max_iter = 10000,
            double step = 0.05,
            double chance = 0.2,
            double skip_distance = 0.5,
            double search_volume = 0.5);

    bool plan(predicator_planning::PredicatePlan::Request &req,
              predicator_planning::PredicatePlan::Response &res);

  };


}

#endif
