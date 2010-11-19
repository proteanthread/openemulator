
/**
 * OpenEmulator
 * Mac OS X Audio Controls Window Controller
 * (C) 2009 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls the audio controls window.
 */

#import "AudioControlsWindowController.h"

@implementation AudioControlsWindowController

- (id)init
{
	if (self = [self initWithWindowNibName:@"AudioControls"])
	{
		playURL = nil;
		recordingURL = nil;
	}
	
	return self;
}

- (void)windowDidLoad
{
	[self setWindowFrameAutosaveName:@"AudioControls"];
	
	[[self window] registerForDraggedTypes:[NSArray arrayWithObjects:
											NSFilenamesPboardType,
											nil]];
	
	timer = [NSTimer scheduledTimerWithTimeInterval:0.25
											 target:self
										   selector:@selector(timerDidExpire:)
										   userInfo:nil
											repeats:YES];
}

- (void)dealloc
{
	if (playURL)
		[playURL release];
	
	if (recordingURL)
	{
		NSError *error;
		
		[[NSFileManager defaultManager] removeItemAtPath:[recordingURL path]
												   error:&error];
		
		[recordingURL release];
	}
	
	[timer release];
	
	[[self window] unregisterDraggedTypes];
	
	[super dealloc];
}

- (BOOL)validateUserInterfaceItem:(id)item
{
    if ([item action] == @selector(toggleAudioControls:))
	{  
		NSString *menuTitle;
		if (![[self window] isVisible])
			menuTitle = NSLocalizedString(@"Show Audio Controls",
										  @"Title for menu item to show Audio Controls.");
		else
			menuTitle = NSLocalizedString(@"Hide Audio Controls",
										  @"Title for menu item to hide Audio Controls.");
		[item setTitleWithMnemonic:menuTitle];
    }
	
    return YES;
}

- (void)toggleAudioControls:(id)sender
{
	if ([[self window] isVisible])
		[[self window] orderOut:self];
	else
		[[self window] orderFront:self];
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
    NSPasteboard *pasteboard = [sender draggingPasteboard];
	
	if ([[pasteboard types] containsObject:NSFilenamesPboardType])
	{
		NSString *path = [[pasteboard propertyListForType:NSFilenamesPboardType]
						  objectAtIndex:0];
		NSString *pathExtension = [[path pathExtension] lowercaseString];
		
		if ([[fDocumentController audioPathExtensions] containsObject:pathExtension])
			return NSDragOperationCopy;
	}
	return NSDragOperationNone;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
	NSPasteboard *pasteboard = [sender draggingPasteboard];
	if ([[pasteboard types] containsObject:NSFilenamesPboardType])
	{
		NSString *path = [[pasteboard propertyListForType:NSFilenamesPboardType]
						  objectAtIndex:0];
		NSString *pathExtension = [[path pathExtension] lowercaseString];
		
		if ([[fDocumentController audioPathExtensions] containsObject:pathExtension])
		{
			[self readFromURL:[NSURL fileURLWithPath:path]];
			
			return YES;
		}
	}
	
	return NO;
}

- (void)timerDidExpire:(NSTimer *)theTimer
{
	[self updatePlay];
	[self updateRecording];
}

- (NSString *)formatTime:(int)time
{
	return [NSString stringWithFormat:@"%02d:%02d:%02d",
			time / 3600,
			(time / 60) % 60,
			time % 60];
}

- (NSString *)formatSize:(long long)size
{
	if (size < 1e6)
		return [NSString stringWithFormat:@"%3.0f kB", size / 1E3];
	else if (size < 1e9)
		return [NSString stringWithFormat:@"%3.1f MB", size / 1E6];
	else
		return [NSString stringWithFormat:@"%3.1f GB", size / 1E9];
}

- (void)updatePlay
{
	if (!playURL)
	{
		[fPlayNameLabel setStringValue:@""];
		[fPlayTimeLabel setToolTip:@"--:--:--"];
		[fPlayDurationLabel setToolTip:@"--:--:--"];
	}
	else
	{
		NSString *path = [[playURL path] lastPathComponent];
		[fPlayNameLabel setStringValue:path];
		[fPlayNameLabel setToolTip:path];
		
		float playTime = [fDocumentController getPlayTime];
		float playDuration = [fDocumentController getPlayDuration];
		NSString *timeLabel = [self formatTime:playTime];
		NSString *durationLabel = [self formatTime:playDuration];
		[fPlayTimeLabel setStringValue:timeLabel];
		[fPlayDurationLabel setStringValue:durationLabel];
	}
	
	BOOL isPlaying = [fDocumentController isPlaying];
	[fOpenPlayButton setEnabled:!isPlaying];
	[fTogglePlayButton setEnabled:playURL ? YES : NO];
	[fTogglePlayButton setImage:(isPlaying ?
								 [NSImage imageNamed:@"AudioStop.png"] :
								 [NSImage imageNamed:@"AudioPlay.png"]
								 )];
}

- (IBAction)openPlay:(id)sender
{
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	NSArray *fileTypes = [[NSArray alloc] initWithObjects:
						  @"wav",
						  @"aiff", @"aif", @"aifc",
						  @"au",
						  @"flac",
						  @"caf",
						  @"ogg", @"oga",
						  nil];
	
	if ([panel runModalForTypes:fileTypes] == NSOKButton)
	{
		NSURL *url = [panel URL];
		[self readFromURL:url];
		
		[self updatePlay];
	}
}

- (IBAction)togglePlay:(id)sender
{
	if (![fDocumentController isPlaying])
		[fDocumentController startPlaying:playURL];
	else
		[fDocumentController stopPlaying];
}

- (void)readFromURL:(NSURL *)theURL
{
	if (playURL)
		[playURL release];
	
	playURL = [theURL copy];
	if (playURL)
		[fDocumentController startPlaying:playURL];
}

- (void)updateRecording
{
	if (!recordingURL)
	{
		[fRecordingTimeLabel setStringValue:@"--:--:--"];
		[fRecordingSizeLabel setStringValue:@"- kB"];
		[fSaveRecordingAsButton setEnabled:NO];
	}
	else
	{
		float recordingTime = [fDocumentController getRecordingTime];
		long long recordingSize = [fDocumentController getRecordingSize];
		NSString *timeLabel = [self formatTime:recordingTime];
		NSString *sizeLabel = [self formatSize:recordingSize];
		[fRecordingTimeLabel setStringValue:timeLabel];
		[fRecordingSizeLabel setStringValue:sizeLabel];
	}
	
	BOOL isRecording = [fDocumentController isRecording];
	[fToggleRecordingButton setImage:(isRecording ?
									  [NSImage imageNamed:@"AudioStop.png"] :
									  [NSImage imageNamed:@"AudioRecord.png"]
									  )];
	[fSaveRecordingAsButton setEnabled:(recordingURL && !isRecording)];
}

- (IBAction)toggleRecording:(id)sender
{
	if (![fDocumentController isRecording])
	{
		NSString *thePath = [NSTemporaryDirectory()
							 stringByAppendingPathComponent:@"oerecording"];
		recordingURL = [[NSURL alloc] initFileURLWithPath:thePath];
		
		[fDocumentController startRecording:recordingURL];
	}
	else
		[fDocumentController stopRecording];
}

- (IBAction)saveRecording:(id)sender
{
	NSSavePanel *panel = [NSSavePanel savePanel];
	[panel setAllowedFileTypes:[NSArray arrayWithObject:@"wav"]];
	[panel setAllowsOtherFileTypes:NO];
	
	if ([panel runModal] == NSOKButton)
	{
		NSURL *url = [panel URL];
		[self writeToURL:url];
		
		[self updateRecording];
	}
}

- (void)writeToURL:(NSURL *)theURL
{
	if (!recordingURL)
		return;
	
	NSError *error;
	
	[[NSFileManager defaultManager] removeItemAtPath:[theURL path]
											   error:&error];
	if (![[NSFileManager defaultManager] moveItemAtPath:[recordingURL path]
												 toPath:[theURL path]
												  error:&error])
	{
		NSAlert *alert = [[NSAlert alloc] init];
		
		[alert setMessageText:NSLocalizedString(@"The document could not be saved.",
												@"The document could not be saved.")];
		[alert setInformativeText:NSLocalizedString(@"Try saving the file to another volume.",
													@"Try saving the file to another volume.")];
		[alert runModal];
		
		[alert release];
		
		return;
	}
	
	[recordingURL release];
	
	recordingURL = nil;
}

@end
