
#import <Foundation/Foundation.h>

@class DAPeople;

/*
 @abstract
 Implement the protocol, SDK will auto track Screen Event with <getTrackProperties>,
 Screen title with <getScreenTitle> and Screen URL with <getScreenUrl>.
 
 @discussion
 TrackProperties keys must be <code>NSString</code> objects and values must be
 <code>NSString</code>, <code>NSNumber</code>, <code>NSNull</code>objects.
 */
@protocol DAScreenAutoTracker
@required
-(NSString *)getScreenTitle;

@optional
-(NSDictionary *)getTrackProperties;
-(NSString *)getScreenUrl;

@end

/*
 This class manages tracking and uploading work through sessions.

 It does provide two APIs,
    'startTrackerWithAppKey' to configure and start data tracking,
    'trackEvent' to track whatever you like.

 To use this lib, there are some libs/frameworks you need to include:
    AdSupport.framework - To enable the ADID support by Apple,
    CoreTelephony.framework - To get the mobile phone carrier,
    Security.framework - To encrypt data,
    SystemConfiguration.framework - To check network type (wifi or cellular),
    libsqlite3.dylib - To support sqlite,
    libz.dylib - To compress the data before sending.

 Also, this SDK adopt ARC, make sure your compiler support it.
 
 Version: 2.1.4
 */
@interface DATracker : NSObject

/*
 @property
 
 @abstract
 Accessor to the DATracker People API object.
 */
@property (atomic, readonly, strong) DAPeople *people;

/*
 Singleton class which maintains a sharedTracker throughout your application.
 The class should be accessed within code using following syntax:
 [[DATracker sharedTracker] someFunction]
 */
+ (id)sharedTracker;

// Set remoteDebug status, default is NO.
// For more app's Debug open and activation detail. Must Called before startTrackerWithAppKey.
- (void)setRemoteDebugOn:(BOOL)remoteDebug;
//  URL Scheme.
- (BOOL)handleUrl:(NSURL*)url;

/*
 Tracking functionality provided by this lib should be activated by this method.
 It is strongly recommended that this call should be placed in
    application:DidFinishLaunching

 @appKey The unique key for each application, you can get it from our web system.
 @appVersion Current version of the application using this SDK.
 @appChannel Release channel of the application using this SDK.
 
 Options:
 @isAutoUpload Set it to NO if you don't want SDK sends data automatically.
               You should manually call upload() method.
 @sendOnWifi Set it to YES if you want SDK sends data only when WIFI is on,
             doing this will delay the server to receive data, do not recommend.
 @udid Device UDID of Apple or anything you set as an unique identifier.
         This SDK doesn't collect device UDID due to Apple's policy,
         but you can still set it manually in this interface.
         Please be careful, doing this might make your App rejected by AppStore.
 */
- (void)startTrackerWithAppKey:(NSString *)appKey appVersion:(NSString *)appVersion
                    appChannel:(NSString *)appChannel;

- (void)startTrackerWithAppKey:(NSString *)appKey appVersion:(NSString *)appVersion
                    appChannel:(NSString *)appChannel autoUpload:(BOOL)isAutoUpload;
- (void)startTrackerWithAppKey:(NSString *)appKey appVersion:(NSString *)appVersion
                    appChannel:(NSString *)appChannel autoUpload:(BOOL)isAutoUpload
                    sendOnWifi:(BOOL)sendOnWifi;
- (void)startTrackerWithAppKey:(NSString *)appKey appVersion:(NSString *)appVersion
                    appChannel:(NSString *)appChannel autoUpload:(BOOL)isAutoUpload
                    sendOnWifi:(BOOL)sendOnWifi customUDID:(NSString *)udid;

// Set auto upload status
- (void)setAutoUploadOn:(BOOL)isAutoUpload;
// Set send only on wifi status
- (void)setSendOnWifiOn:(BOOL)sendOnWifi;

// Set Page View Track. Default is <YES>.
- (void)setPageViewTrack:(BOOL)isAllowTrack;
// Filter Page View Track.
- (void)setFilterControllers:(NSArray <NSString *>*)filerControllers;

/*
 Enable marketing campaign tracking
 */
- (void)enableCampaign;

/*
 Get device ID
 Notice: This ID isn't the Apple UDID, 
    and it might be changed later due to Apple's policy change.
 */
- (NSString *)getDeviceId;

/*
 Set user account
 */
- (void)loginUser:(NSString *)userId;

- (void)logoutUser;

/*
 Set user's location
 */
- (void)setLocation:(double)latitude andLongitude:(double)longitude;

/*
 Track a particular event as having occurred.
 It might make sense to track button click with the name of the button as eventId
    or track some important view loading with the name of the view as eventId.
 Also, customized attributes can be added as key value pairs.

 Please DO NOT use this method to record personally identifiable information.
 The Best way to use this trackEvent is to create all eventId or customized attributes' names
    as predefined constants and only use those. This helps keeping eventIds and attributes names
    in order, which will be of great benefit for you to check on the analytics report.
 Do not use this method inside loops or any place which gets called frequently,
    which can cause a lot of data to be stored and uploaded.
 Please DO NOT use any event id begin with "da", which is reserved by SDK itself.
 */
- (void)trackEvent:(NSString *)eventId;
- (void)trackEvent:(NSString *)eventId withAttributes:(NSDictionary *)attributes;
/*
 Events can be categoried and labeled
 */
- (void)trackEvent:(NSString *)eventId category:(NSString *)category label:(NSString *)label;
- (void)trackEvent:(NSString *)eventId category:(NSString *)category label:(NSString *)label withAttributes:(NSDictionary *)attributes;
 
/*
 If you want to record the cost time of an event
 */
- (void)trackEvent:(NSString *)eventId costTime:(int)seconds category:(NSString *)category label:(NSString *)label;
- (void)trackEvent:(NSString *)eventId costTime:(int)seconds category:(NSString *)category label:(NSString *)label withAttributes:(NSDictionary *)attributes;
/*
 If you also want to record location of an event
 */
- (void)trackEvent:(NSString *)eventId costTime:(int)seconds latitude:(double)latitude longitude:(double)longitude category:(NSString *)category label:(NSString *)label withAttributes:(NSDictionary *)attributes;

/*
 Record search activity.
 @keyword search keyword
 @searchType in which search this activity happens
 */
- (void)trackSearch:(NSString *)keyword in:(NSString *)searchType;
/*
 Record share activity.
 @content share content
 @from share from
 @to share desitination
 */
- (void)trackShare:(NSString*)content from:(NSString *)from to:(NSString *)to;
/*
 Record comment activity.
 @content comment content
 @onItem comment on
 */
- (void)trackComment:(NSString *)content onItem:(NSString *)onItem;
/*
 Record favorite activity.
 @item favorite on
 */
- (void)trackFavorite:(NSString *)item;

/*
 Record Screen
 @screenName screen name
 */
- (void)trackScreen:(NSString *)screenName;

/*
 Track an exception occurred in @try @catch block.
 */
- (void)trackException:(NSException *)exception;
- (void)trackExceptionWithCallstack:(NSException *)exception;
- (void)trackExceptionWithName:(NSString *)name reason:(NSString *)reason
                 callstack:(NSString *)callstack;


/*
 @method
 
 @abstract
 Starts a timer that will be stopped and added as a property when a
 corresponding event is tracked.
 
 @discussion
 This method is intended to be used in advance of events that have
 a duration. For example, if a developer were to track an "Image Upload" event
 she might want to also know how long the upload took. Calling this method
 before the upload code would implicitly cause the <code>track</code>
 call to record its duration.
 
 <pre>
 // begin timing the image upload
 [DATracker trackTimer:@"Image Upload"];
 
 // upload the image
 [self uploadImageWithSuccessHandler:^{
 
 // track the event
 [DATracker trackEvent:@"Image Upload"];
 }];
 </pre>
 
 @param event   a string, identical to the name of the event that will be tracked
 
 */
- (void)trackTimer:(NSString *)eventId;

/*
 @method
 
 @abstract
 Clears all current event timers.
 */
- (void)clearTrackTimer;

/*
 * Enable crash reporting to track unhandled exceptions and signals
 */
- (void)enableCrashReporting;

/*
 Send all data to server, then remove all data when upload succeeded.
 All the data collected by this SDK will send to server automatically,
    Usually, it is NOT necessary to use this method.
 If you DO want to upload data as soon as possible sometime, be cautious,
    too many upload tasks will block the queue, might influence other activities,
 Please use it as less as possible, and at your own risk.
 */
- (void)upload;

/*
 @method
 
 @abstract
 Set upload timer's interval.
 Default uploadInterval is 15 seconds.
 */
- (void)setUploadInterval:(NSInteger)uploadInterval;

/*
 @method
 
 @abstract
 Set upload max bulkSize.
 Default uploadBulkSize is 100.
 */
- (void)setUploadBulkSize:(NSInteger)uploadBulkSize;

/*
 Track a mission, by marking its began, accomplishment or failurea.
 @missonId id of mission
 @reason mission failed reason
 */
- (void)trackOnMissionBegan:(NSString *)missionId;
- (void)trackOnMissionAccomplished:(NSString *)missionId;
- (void)trackOnMissionFailed:(NSString *)missionId reason:(NSString *)reason;

/*
 @method
 
 @abstract
 Returns the currently set super properties.
 */
- (NSDictionary *)currentSuperProperties;

/*
 @method
 
 @abstract
 Registers super properties, overwriting ones that have already been set.
 
 @discussion
 Super properties, once registered, are automatically sent as properties for
 all event tracking calls. They save you having to maintain and add a common
 set of properties to your events. Property keys must be <code>NSString</code>
 objects and values must be <code>NSString</code>, <code>NSNumber</code>objects.
 
 @param properties      properties dictionary
 */
- (void)registerSuperProperties:(NSDictionary *)properties;

/*
 @method
 
 @abstract˜
 Removes a previously registered super property.
 
 @discussion
 As an alternative to clearing all properties, unregistering specific super
 properties prevents them from being recorded on future events. This operation
 does not affect the value of other super properties. Any property name that is
 not registered is ignored.
 
 Note that after removing a super property, events will show the attribute as
 having the value <code>undefined</code> in DATracker until a new value is
 registered.
 
 @param propertyName   array of property name strings to remove
 */
- (void)unregisterSuperProperty:(NSString *)propertyName;

/*
 @method
 
 @abstract
 Registers super properties without overwriting ones that have already been
 set.
 
 @discussion
 Property keys must be <code>NSString</code> objects and values must be
 <code>NSString</code>, <code>NSNumber</code>objects.
 
 @param properties      properties dictionary
 */
- (void)registerSuperPropertiesOnce:(NSDictionary *)properties;

/*
 @method
 
 @abstract
 Registers super properties without overwriting ones that have already been set
 unless the existing value is equal to defaultValue.
 
 @discussion
 Property keys must be <code>NSString</code> objects and values must be
 <code>NSString</code>, <code>NSNumber</code> objects.
 
 @param properties      properties dictionary
 @param defaultValue    overwrite existing properties that have this value
 */
- (void)registerSuperPropertiesOnce:(NSDictionary *)properties defaultValue:(id)defaultValue;

/*
 @method
 
 @abstract
 Clears all currently set super properties.
 */
- (void)clearSuperProperties;

/*
 @methid
 
 @abstract
 Set up connection between H5 and Native. This must be called after <code>startTrackerWithAppKey</code>
 */
- (void)addWebViewUserAgentFlag;

/*
 @methid
 
 @abstract
 Set up connection between H5 and Native.
 
 @param webView  current webview, <code>UIWebView</code> or <code>WKWebView</code>.
 @param request  NSURLRequest.
 */
- (BOOL)showUpWebView:(id)webView request:(NSURLRequest *)request;

/*
 @methid
 
 @abstract
 Set up connection between H5 and Native.
 
 @param webView     Current webview, <code>UIWebView</code> or <code>WKWebView</code>.
 @param request     NSURLRequest.
 @param properties  Properties set by user. Properties keys must be <code>NSString</code> objects and values must be
                    <code>NSString</code>, <code>NSNumber</code> objects
 */
- (BOOL)showUpWebView:(id)webView request:(NSURLRequest *)request properties:(NSDictionary *)properties;

@end

/*
 @class
 DA People API.
 
 @abstract
 Access to the DA People API, available as a property on the main
 DATracker API.
 
 @discussion
 <b>You should not instantiate this object yourself.</b> An instance of it will
 be available as a property of the main DATracker object. Calls to DATracker
 People methods will look like this:
 */
@interface  DAPeople: NSObject

/*
 DAPeople init
 
 @discussion
 Called by the property <people> in DATracker.
 
 @param tracker   SDK
 */
- (instancetype)initWithSDK:(DATracker *)tracker;

/*
 set user's location
 
 @param country country
 @param region  province
 @param city    city
 */
- (void)setLocation:(NSString *)country region:(NSString *)region city:(NSString *)city;

/*
 set user's account, realName, birthday, gender
 
 @param account
 @param realName
 @param birthday
 @param gender   0 is female, 1 is male, 2 is unknown
 */
- (void)setPopulationWithAccount:(NSString *)account realName:(NSString *)realName birthday:(NSDate *)birthday gender:(NSInteger)gender;

/*
 set user's account
 
 @param account
 */
- (void)setAccount:(NSString *)account;

/*
 set user's real name
 
 @param realName
 */
- (void)setRealName:(NSString *)realName;

/*
 set user's birthday
 
 @param birthday
 */
- (void)setBirthday:(NSDate *)birthday;

/*
 set user's gender
 
 @param gender    0 is female, 1 is male, 2 is unknown
 */
- (void)setGender:(NSInteger)gender;

/*
 @method
 
 @abstract
 Set properties on the current user in DATracker People.
 
 @discussion
 The properties will be set on the current user. The keys must be NSString
 objects and the values should be NSString, NSNumber objects.
 
 @param properties       properties dictionary
 
 */
- (void)set:(NSDictionary *)properties;

/*
 @method
 
 @abstract
 Convenience method for setting a single property in DATracker People.
 
 @discussion
 Property keys must be <code>NSString</code> objects and values must be
 <code>NSString</code>, <code>NSNumber</code> objects.
 
 @param property        property name
 @param object          property value
 */
- (void)set:(NSString *)property to:(id)object;

/*
 @method
 
 @abstract
 Set properties on the current user in DATracker People, but don't overwrite if
 there is an existing value.
 
 @discussion
 This method is identical to <code>set:</code> except it will only set
 properties that are not already set. It is particularly useful for collecting
 data about the user's initial experience and source, as well as dates
 representing the first time something happened.
 
 @param properties       properties dictionary
 
 */
- (void)setOnce:(NSDictionary *)properties;

/*
 @method
 
 @abstract
 Convenience method for setting a single property in DATracker People, but don't overwrite if
 there is an existing value.
 
 @discussion
 Property keys must be <code>NSString</code> objects and values must be
 <code>NSString</code>, <code>NSNumber</code> objects.
 
 @param property        property name
 @param object          property value
 */
- (void)setOnce:(NSString *)property to:(id)object;

/*
 @method
 
 @abstract
 Remove property and its value from the current user's profile
 in DATracker People.
 
 @discussion
 The property array must ony NSString. For property
 that don't exist there will be no effect.
 
 @param properties       properties array
 
 */
- (void)unset:(NSString *)property;

/*
 @method
 
 @abstract
 Delete current user's record from DATracker People.
 */
- (void)deleteUser;

/*
 @method
 
 @abstract
 Track money spent by the current user for revenue analytics.
 
 @param amount          amount of revenue received
 */
- (void)trackCharge:(NSNumber *)amount;

/*
 @method
 
 @abstract
 Track money spent by the current user for revenue analytics and associate
 properties with the charge.
 
 @discussion
 Charge properties allow you segment on types of revenue. For instance, you
 could record a product ID with each charge so that you could segment on it in
 revenue analytics to see which products are generating the most revenue.
 */
- (void)trackCharge:(NSNumber *)amount withProperties:(NSDictionary *)properties;

/*
 @method
 
 @abstract
 Delete current user's revenue history.
 */
- (void)clearCharges;

@end
