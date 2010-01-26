
/**
 * libemulator
 * DML Reference
 * (C) 2009-2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Type for representing DML refs
 */

#ifndef _OEREF_H
#define _OEREF_H

#define OE_DEVICE_SEP "::"
#define OE_COMPONENT_SEP "."

using namespace std;

class OERef
{
public:
	OERef();
	OERef(string stringRef);
	OERef(const OERef &ref);
	OERef &operator=(const OERef &ref);
	bool operator==(const OERef &ref);
	
	bool isEmpty();
	
	void setDevice(string device);
	string getDevice() const;
	void setComponent(string component);
	string getComponent() const;
	void setProperty(string property);
	string getProperty() const;
	
	OERef getRef(string stringRef);
	OERef getDeviceRef();
	OERef getComponentRef();
	
	string getStringRef();
	string getStringRef(string stringRef);
	
private:
	string device;
	string component;
	string property;
};

ostream &operator<<(ostream &stream, const OERef &ref);

#endif
