//
//  Copyright (c) SRG. All rights reserved.
//
//  License information is available from the LICENSE file.
//

#import <AVFoundation/AVFoundation.h>
#import <AVKit/AVKit.h>
#import <UIKit/UIKit.h>

#import "RTSMediaPlayerConstants.h"

/**
 *  `RTSMediaPlayerController` is inspired by the `MPMoviePlayerController` class.
 *
 *  A media player (of type `RTSMediaPlayerController`) manages the playback of a media from a file or a network stream.
 *  For maximum flexibility, you can incorporate a media player’s view into a view hierarchy owned by your app and have 
 *  it managed by an `RTSMediaPlayerController` instance. If you just need a standard player with a view looking just
 *  like the standard iOS media player, you should simply instantiate an `RTSMediaPlayerViewController` which will manage
 *  the view for you.
 *
 *  The media player controller posts several notifications, see RTSMediaPlayerConstants.h
 *
 *  Errors are handled through the `RTSMediaPlayerPlaybackDidFailNotification` notification. There are two possible
 *  source of errors: either the error comes from the dataSource (see `RTSMediaPlayerControllerDataSource`) or from
 *  the network (playback error).
 *
 *  The media player controller manages its overlays visibility. See the `overlayViews` property.
 *
 *  Methods related to playback can be found in the `RTSMediaPlayback` protocol
 */
@interface RTSMediaPlayerController : NSObject

/**
 *  -------------------
 *  @name Player Object
 *  -------------------
 */

/**
 *  The player that provides the media content.
 *
 *  @discussion This can be used to implement advanced behaviors. This property should not be used to alter player properties,
 *              but merely for KVO registration or information extraction. Altering player properties in any way results in
 *              undefined behavior
 */
@property (nonatomic, readonly) AVPlayer *player;

/**
 *  ------------------------
 *  @name Accessing the View
 *  ------------------------
 */

/**
 *  The view containing the media content.
 *
 *  @discussion This property contains the view used for presenting the media content. To display the view into your own
 *  view hierarchy, use the `attachPlayerToView:` method.
 *
 *  This view has two gesture recognziers: a single tap gesture recognizer and a double tap gesture recognizer which
 *  toggle overlays visibility, respectively the video aspect between `AVLayerVideoGravityResizeAspectFill` and 
 *  `AVLayerVideoGravityResizeAspect`.
 *
 *  If you want to handle taps yourself, you can disable these gesture recognizers and add your own gesture recognizers.
 *
 *  @see `attachPlayerToView:`
 */
@property (nonatomic, readonly) UIView *view;

/**
 *  Attach the player view into specified container view with default autoresizing mask. The player view will have the
 *  same frame as its `containerView`
 *
 *  @param `containerView` The parent view in hierarchy what will contains the player layer
 */
- (void)attachPlayerToView:(UIView *)containerView;

/**
 *  -------------------
 *  @name Overlay Views
 *  -------------------
 */

@property (nonatomic, readonly) AVPlayerItem *playerItem;

/**
 *  -------------------
 *  @name Overlay Views
 *  -------------------
 */

/**
 *  View on which user activity is detected (to prevent the UI overlays from being automatically hidden, see 'overlayViews' and
 *  'overlayViewsHidingDelay')
 */
@property (nonatomic, weak) IBOutlet UIView *activityView;

/**
 *  A collection of views that will be shown / hidden automatically or manually when user interacts with the view.
 */
@property (nonatomic) IBOutletCollection(UIView) NSArray *overlayViews;

/**
 *  The delay after which the overlay views are hidden. Default to `RTSMediaPlayerOverlayHidingDelay` (5 sec).
 *  Ignored if <= 0.0;
 */
@property (nonatomic) NSTimeInterval overlayViewsHidingDelay;

/**
 *  Return YES iff overlays are currently visible
 */
@property (nonatomic, readonly, getter=areOverlaysVisible) BOOL overlaysVisible;


/**
 *  -------------------------
 *  @name Controling Playback
 *  -------------------------
 */

/**
 *  Returns the current playback state of the media player. See `RTSMediaPlaybackState` for possible values
 */
@property (nonatomic, readonly) RTSMediaPlaybackState playbackState;

/**
 *  Start playing a media specified using its identifier. Retrieving the media URL requires a data source to be bound
 *  to the player controller
 */
- (void)playURL:(NSURL *)URL;

/**
 *  ------------------------------------
 *  @name Accessing playback information
 *  ------------------------------------
 */

/**
 *  The current media time range (might be empty or indefinite). Use `CMTimeRange` macros for checking time ranges
 */
@property (nonatomic, readonly) CMTimeRange timeRange;

/**
 *  The media type (audio / video). See `RTSMediaType` for possible values
 *
 *  Warning: Is currently unreliable when Airplay playback has been started before the media is played
 *           Related to https://openradar.appspot.com/27079167
 */
@property (nonatomic, readonly) RTSMediaType mediaType;

/**
 *  The stream type (live / DVR / VOD). See `RTSMediaStreamType` for possible values
 *
 *  Warning: Is currently unreliable when Airplay playback has been started before the media is played
 *           Related to https://openradar.appspot.com/27079167
 */
@property (nonatomic, readonly) RTSMediaStreamType streamType;

/**
 *  Return YES iff the stream is currently played in live conditions
 */
@property (nonatomic, readonly, getter=isLive) BOOL live;

/**
 *  The minimum window length which must be available for a stream to be considered to be a DVR stream, in seconds. The 
 *  default value is 0. This setting can be used so that streams detected as DVR ones because their window is small can
 *  behave as live streams. This is useful to avoid usual related seeking issues, or slider hiccups during playback, most
 *  notably
 */
@property (nonatomic) NSTimeInterval minimumDVRWindowLength;

/**
 *  Return the tolerance (in seconds) for a DVR stream to be considered being played in live conditions. If the stream
 *  playhead is located within the last liveTolerance conditions of the stream, it is considered to be live, not live
 *  otherwise. The default value is 30 seconds and matches the standard iOS behavior
 */
@property (nonatomic) NSTimeInterval liveTolerance;

@end

/**
 *  Picture in picture functionality (not available on all devices)
 *
 *  Remark: When the application is sent to the background, the behavior is the same as the vanilla picture in picture
 *          controller: If the managed player layer is the one of a view controller's root view ('full screen'), picture
 *          in picture is automatically enabled when switching to the background (provided the corresponding flag has been
 *          enabled in the system settings). This is the only case where switching to picture in picture can be made
 *          automatically. Picture in picture must otherwise always be user-triggered, otherwise you application might
 *          get rejected by Apple (see `AVPictureInPictureController` documentation)
 */
@interface RTSMediaPlayerController (PictureInPicture)

/**
 *  Return the picture in picture controller if available, nil otherwise
 */
@property (nonatomic, readonly) AVPictureInPictureController *pictureInPictureController;

@end
