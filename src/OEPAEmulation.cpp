
/**
 * OpenEmulator
 * OEPA Emulation interface
 * (C) 2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * OEPA Emulation interface.
 */

#include "OEPAEmulation.h"
#include "OEPA.h"

OEPAEmulation::OEPAEmulation(string path) :
OEEmulation()
{
	oepa = NULL;
}

OEPAEmulation::OEPAEmulation(string path, string resourcePath) :
OEEmulation(path, resourcePath)
{
	oepa = NULL;
}

OEPAEmulation::OEPAEmulation(OEPA *oepa,
							 string path, string resourcePath) :
OEEmulation(path, resourcePath)
{
	this->oepa = oepa;
}



void OEPAEmulation::lock()
{
	((OEPA *)oepa)->lockEmulations();
}

void OEPAEmulation::unlock()
{
	((OEPA *)oepa)->unlockEmulations();
}

bool OEPAEmulation::save(string path)
{
	lock();
	
	bool status = OEEmulation::save(path);
	
	unlock();
	
	return status;
}

bool OEPAEmulation::setProperty(string ref, string name, string value)
{
	lock();
	
	OEComponent *component = getComponent(ref);
	bool status;
	if (component)
		status = component->setProperty(name, value);
	else
	{
		status = false;
		oepaLog("could not set property " << ref << "." << name <<
				" (ref not found)");
	}
	
	unlock();
	
	return status;
}

bool OEPAEmulation::getProperty(string ref, string name, string &value)
{
	lock();
	
	OEComponent *component = getComponent(ref);
	bool status;
	if (component)
		status = component->getProperty(name, value);
	else
	{
		status = false;
		oepaLog("could not get property " << ref << "." << name << 
				" (ref not found)");
	}
	
	unlock();
	
	return status;
}

void OEPAEmulation::notify(string ref, int notification, void *data)
{
	lock();
	
	OEComponent *component = getComponent(ref);
	if (component)
		component->notify(NULL, notification, data);
	else
	{
		oepaLog("could not notify " + ref +
				" (ref not found)");
	}
	
	unlock();
}

int OEPAEmulation::postEvent(string ref, int message, void *data)
{
	lock();
	
	OEComponent *component = getComponent(ref);
	int status = 0;
	if (component)
		status = component->postEvent(component, message, data);
	else
	{
		oepaLog("could not post event to " + ref +
				" (ref not found)");
	}
	
	unlock();
	
	return status;
}

bool OEPAEmulation::add(string path, OEConnections &connections)
{
	lock();
	
	bool status = OEEmulation::add(path, connections);
	
	unlock();
	
	return status;
}

bool OEPAEmulation::removeDevice(string deviceName)
{
	lock();
	
	bool status = OEDML::removeDevice(deviceName);
	
	unlock();
	
	return status;
}
