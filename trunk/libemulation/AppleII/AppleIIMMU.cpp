
/**
 * libemulator
 * Apple II Memory Management Unit
 * (C) 2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls the Apple II D000-FFFF range.
 */

#include "AppleIIMMU.h"

#define APPLEIIMMU_ROM_OFFSET	0xd000
#define APPLEIIMMU_ROM_SIZE		0x3000

AppleIIMMU::AppleIIMMU()
{
	memoryBus = NULL;
	floatingBus = NULL;
	
	romD0 = NULL;
	romD8 = NULL;
	romE0 = NULL;
	romE8 = NULL;
	romF0 = NULL;
	romF8 = NULL;

	romD0Socket = NULL;
	romD8Socket = NULL;
	romE0Socket = NULL;
	romE8Socket = NULL;
	romF0Socket = NULL;
	romF8Socket = NULL;
}

bool AppleIIMMU::setComponent(string name, OEComponent *component)
{
	if (name == "memoryBus")
		memoryBus = component;
	else if (name == "romD0")
		romD0 = component;
	else if (name == "romD8")
		romD8 = component;
	else if (name == "romE0")
		romE0 = component;
	else if (name == "romE8")
		romE8 = component;
	else if (name == "romF0")
		romF0 = component;
	else if (name == "romF8")
		romF8 = component;
	else if (name == "romD0Socket")
		romD0Socket = component;
	else if (name == "romD8Socket")
		romD8Socket = component;
	else if (name == "romE0Socket")
		romE0Socket = component;
	else if (name == "romE8Socket")
		romE8Socket = component;
	else if (name == "romF0Socket")
		romF0Socket = component;
	else if (name == "romF8Socket")
		romF8Socket = component;
	else
		return false;
	
	return true;
}
