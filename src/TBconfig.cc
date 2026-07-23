#include "TBconfig.h"

TBconfig::TBconfig(const std::string &config_, int fRunNumber_)
    : fRunNumber(fRunNumber_)
{

    config = YAML::LoadFile(config_);
}
