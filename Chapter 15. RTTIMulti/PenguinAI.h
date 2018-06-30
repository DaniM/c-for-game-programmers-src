#ifndef PENGUINAI_H_
#define PENGUINAI_H_

#include "RTTI.h"
#include "WaterAI.h"
#include "GroundAI.h"

class PenguinAI : public WaterAI, public GroundAI 
{
    RTTI_DECL_2_PARENT
};

#endif 
