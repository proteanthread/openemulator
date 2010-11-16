
/**
 * libemulation
 * OEEDL
 * (C) 2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls an EDL document
 */

#ifndef _OEEDL_H
#define _OEEDL_H

#include <libxml/tree.h>

#include "OETypes.h"
#include "OEPackage.h"

#define OE_STANDALONE_EXTENSION "xml"
#define OE_PACKAGE_EXTENSION "emulation"
#define OE_PACKAGE_EDL_FILENAME "info.xml"

typedef map<string, string> OEIdMap;
typedef vector<string> OEIdList;

struct OEEDLInfo
{
	string label;
	string image;
	string description;
	string windowFrame;
};

struct OEDeviceInfo
{
	string id;
	string type;
	string label;
	string image;
	
	string connectionLabel;
};

typedef vector<OEDeviceInfo> OEDevicesInfo;

struct OEPortInfo
{
	string id;
	string ref;
	string type;
	string label;
	string image;
};

typedef vector<OEPortInfo> OEPortsInfo;

typedef OEPortInfo OEConnectorInfo;
typedef OEPortsInfo OEConnectorsInfo;

class OEEDL
{
public:
	OEEDL();
	OEEDL(string path);
	~OEEDL();
	
	bool open(string path);
	bool isOpen();
	bool save(string path);
	void close();
	
	OEEDLInfo getEDLInfo();
	bool setEDLInfo(OEEDLInfo edlInfo);
	OEDevicesInfo getDevicesInfo();
	OEPortsInfo getPortsInfo();
	OEConnectorsInfo getConnectorsInfo();
	
	bool addEDL(string path, OEIdMap idMap);
	bool removeDevice(string id);
	
protected:
	OEPackage *package;
	xmlDocPtr doc;
	
	virtual bool update();
	
	string getString(int value);
	void setNodeProperty(xmlNodePtr node, string name, string value);
	bool hasNodeProperty(xmlNodePtr node, string name);
	string getNodeProperty(xmlNodePtr node, string name);
	string getPathExtension(string path);
	bool writeFile(string path, OEData *data);
	bool readFile(string path, OEData *data);
	
	// Helpers
	void setDeviceId(string &id, string deviceId);
	string getDeviceId(string id);
	void log(string message);
	
private:
	bool is_open;
	
	void init();
	
	bool validate();
	bool dump(OEData *data);
	
	OEIdMap buildNameMap(OEIdList deviceIds, OEIdList newDeviceIds);
	void rename(OEIdMap nameMap);
	void rename(OEIdMap nameMap, xmlNodePtr node, string property);
	bool renameConnectionMap(OEIdMap &connectionMap, OEIdMap nameMap);
	void insert(OEEDL *edl, string deviceId);
	void connect();
	
	bool removeConnectedDevices(string deviceId);
	void disconnect(string deviceId);
	void removeElements(string deviceId);
	
	bool isDevice(string deviceId);
	OEIdList getDeviceIds();
};

#endif