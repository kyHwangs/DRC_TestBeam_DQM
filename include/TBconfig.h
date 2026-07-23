#ifndef TBconfig_h
#define TBconfig_h 1

#include <iostream>
#include <string>

#include "yaml-cpp/yaml.h"
#include "TBplotengine.h"
#include "TBfastengine.h"

class TBconfig
{
public:
    TBconfig(const std::string &config_, int fRunNumber_);
    ~TBconfig(){};

    const auto &GetConfig() { return config; }

private:
    YAML::Node config;
    int fRunNumber;
};

#endif