
/**
 * OpenEmulator
 * Mac OS X Background view
 * (C) 2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls a background view.
 */

#import "BackgroundView.h"

@implementation BackgroundView

- (void)drawRect:(NSRect)rect
{
	NSEraseRect(rect);
}

@end
