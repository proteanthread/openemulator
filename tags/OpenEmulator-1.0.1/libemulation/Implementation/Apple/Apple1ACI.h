
/**
 * libemulation
 * Apple-1 ACI
 * (C) 2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Implements an Apple-1 ACI (Apple Cassette Interface)
 */

#include "OEComponent.h"

class Apple1ACI : public OEComponent
{
public:
    Apple1ACI();
    
    bool setRef(string name, OEComponent *ref);
    bool setValue(string name, string value);
    bool getValue(string name, string &value);
    bool init();
    
    OEUInt8 read(OEAddress address);
    void write(OEAddress address, OEUInt8 value);
    
private:
    OEComponent *rom;
    OEComponent *memoryBus;
    OEComponent *audioCodec;
    
    OEUInt8 audioLevel;
    OEUInt8 noiseRejection;
    OEUInt8 threshold;
    
    void toggleSpeaker();
};
