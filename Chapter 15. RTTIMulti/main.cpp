
#include "StdAfx.h"

#include "LandAI.h"
#include "FlyingAI.h"
#include "HybridAI.h"

#include "GroundAI.h"
#include "WaterAI.h"
#include "CrawlAI.h"
#include "PenguinAI.h"

void PrintDerivedFrom(const RTTI & rtti1, const RTTI & rtti2)
{
    std::string relation = rtti1.DerivesFrom(rtti2) ? " does " : " does NOT ";
    std::cout << "Class " << rtti1.GetClassName() << relation << 
                 "derive from " << rtti2.GetClassName() << "\n";
}

void PrintDerivedFrom(const IRTTI& rtti1, const IRTTI& rtti2)
{
    std::string relation = rtti1.DerivesFrom(rtti2) || rtti1.IsExactly(rtti2) ? " does " : " does NOT ";
    std::cout << "Class " << rtti1.GetClassName() << relation << 
                 "derive from " << rtti2.GetClassName() << "\n";
}


int main(int argc, char* argv[])
{
    std::cout << "Custom RTTI with multiple inheritance test:\n";

    LandAI * pAI1 = new HybridAI;
    PrintDerivedFrom(pAI1->GetRTTI(), HybridAI::rtti);
    PrintDerivedFrom(pAI1->GetRTTI(), LandAI::rtti);
    PrintDerivedFrom(pAI1->GetRTTI(), FlyingAI::rtti);

    LandAI * pAI2 = new LandAI;
    PrintDerivedFrom(pAI2->GetRTTI(), HybridAI::rtti);
    PrintDerivedFrom(pAI2->GetRTTI(), LandAI::rtti);
    PrintDerivedFrom(pAI2->GetRTTI(), FlyingAI::rtti);

    FlyingAI * pAI3 = new FlyingAI;
    PrintDerivedFrom(pAI3->GetRTTI(), HybridAI::rtti);
    PrintDerivedFrom(pAI3->GetRTTI(), LandAI::rtti);
    PrintDerivedFrom(pAI3->GetRTTI(), FlyingAI::rtti);

	delete pAI1;
	delete pAI2;
	delete pAI3;

	std::cout << "Custom XRTTI with multiple inheritance test:\n";

	GroundAI* pai = new GroundAI;
	PrintDerivedFrom(pai->GetRTTI(), GroundAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), WaterAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), CrawlAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), PenguinAI::rtti);
	
	delete pai;

	pai = new CrawlAI;
	PrintDerivedFrom(pai->GetRTTI(), GroundAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), WaterAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), CrawlAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), PenguinAI::rtti);
	
	delete pai;

	pai = new PenguinAI;
	PrintDerivedFrom(pai->GetRTTI(), GroundAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), WaterAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), CrawlAI::rtti);
	PrintDerivedFrom(pai->GetRTTI(), PenguinAI::rtti);
	
	delete pai;

    return 0;
}

