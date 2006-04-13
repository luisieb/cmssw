#include "RecoTracker/MeasurementDet/interface/NonPropagatingDetMeasurements.h"
#include "TrackingTools/PatternTools/interface/TrajectoryMeasurement.h"
#include "TrackingTools/PatternTools/interface/MeasurementEstimator.h"
#include "TrackingTools/PatternTools/interface/TrajMeasLessEstim.h"
#include "TrackingTools/MeasurementDet/interface/MeasurementDet.h"
#include "TrackingTools/TransientTrackingRecHit/interface/InvalidTransientRecHit.h"

std::vector<TrajectoryMeasurement> 
NonPropagatingDetMeasurements::get( const MeasurementDet& det,
				    const TrajectoryStateOnSurface& stateOnThisDet,
				    const MeasurementEstimator& est) const
{
  std::vector<TrajectoryMeasurement> result;

  MeasurementDet::RecHitContainer allHits = det.recHits( stateOnThisDet);
  for (MeasurementDet::RecHitContainer::const_iterator ihit=allHits.begin();
       ihit != allHits.end(); ihit++) {
    std::pair<bool,double> diffEst = est.estimate( stateOnThisDet, **ihit);
    if ( diffEst.first) {
      result.push_back( TrajectoryMeasurement( stateOnThisDet, *ihit, 
					       diffEst.second));
    }
  }
  if ( result.empty()) {
    // create a TrajectoryMeasurement with an invalid RecHit and zero estimate
    result.push_back( TrajectoryMeasurement( stateOnThisDet, 
					     new InvalidTransientRecHit(&det.geomDet()), 0.F)); 
  }
  else {
    // sort results according to estimator value
    if ( result.size() > 1) {
      sort( result.begin(), result.end(), TrajMeasLessEstim());
    }
  }
  return result;
}
