#include "../launchParamsGlobal.h"
#include  "../util.h"


struct GlobalParams{
    float4 lightPos;
    float4 *accumBuffer;
    float glossiness;
    int glossyRays;
    int shadowRays;
} ;

struct LaunchParams
{
    Frame frame;
    Camera camera;
    OptixTraversableHandle traversable;

    GlobalParams *global;
};
