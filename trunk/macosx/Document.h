
/**
 * OpenEmulator
 * Mac OS X Document
 * (C) 2009-2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Handles an emulation.
 */

#import <Cocoa/Cocoa.h>

#define USER_TEMPLATES_FOLDER @"~/Library/Application Support/OpenEmulator/Templates"

@class EmulationWindowController;

@interface Document : NSDocument
{
	void *emulation;
	
	EmulationWindowController *emulationWindowController;
	
	NSMutableArray *canvases;
	NSMutableArray *canvasWindowControllers;
}

- (id)initWithTemplateURL:(NSURL *)templateURL error:(NSError **)outError;
- (IBAction)saveDocumentAsTemplate:(id)sender;

- (void *)emulationInfo;
- (void)showEmulation:(id)sender;
- (void)showCanvas:(void *)canvas;

- (void)createEmulation:(NSURL *)url;
- (void)destroyEmulation;
- (void)lockEmulation;
- (void)unlockEmulation;

- (BOOL)mount:(NSString *)path inStorage:(void *)component;
- (BOOL)mount:(NSString *)path;
- (BOOL)unmountStorage:(void *)component;
- (BOOL)canMount:(NSString *)path inStorage:(void *)component;
- (BOOL)canMount:(NSString *)path;
- (BOOL)isImageSupported:(NSString *)path;
- (BOOL)isStorageMounted:(void *)component;
- (BOOL)isStorageWritable:(void *)component;
- (BOOL)isStorageLocked:(void *)component;
- (NSString *)imagePathForStorage:(void *)component;
- (NSString *)imageFormatForStorage:(void *)component;
- (NSString *)imageCapacityForStorage:(void *)component;

- (void)setValue:(NSString *)value
	  ofProperty:(NSString *)theName
	forComponent:(NSString *)theId;
- (NSString *)valueOfProperty:(NSString *)name
				 forComponent:(NSString *)theId;

- (void)addEDL:(NSString *)path connections:(NSDictionary *)connections;
- (void)removeDevice:(NSString *)deviceId;

@end
