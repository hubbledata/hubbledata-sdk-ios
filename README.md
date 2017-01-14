# iOS SDK 使用文档 #

## 依赖库 ##

* AdSupport.framework - 启用 Apple ADID 支持
* CoreTelephony.framework - 获取运营商
* Security.framework - 加密支持
* CoreLocation.framework - 获取定位信息的支持
* SystemConfiguration.framework - 获取联网方式(wifi, cellular)
* libsqlite3.dylib - sqlite 支持
* libz.dylib - gzip 压缩支持

**路径：TARGETS -> Build Phases -> Link Binary With Libraries**

## 导入 SDK ##

将下载包里面 DATracker.h, libMobilytics.a 文件添加到 App 项目中

## 启用 API ##

在 `*AppDelegate.m` `application:didFinishLaunchingWithOptions` 方法中调用如下方法，参数依次为 app key，版本和来源渠道。

    [[DATracker sharedTracker] startTrackerWithAppKey:@"app-key" appVersion:@"0.1" appChannel:@"AppStore"];

如需要禁用 SDK 自动上传数据功能，调用

    [[DATracker sharedTracker] startTrackerWithAppKey:@"app-key" appVersion:@"0.1" appChannel:@"AppStore" autoUpload:NO];

如需要设置只在 wifi 环境下发送数据，调用

    [[DATracker sharedTracker] startTrackerWithAppKey:@"app-key" appVersion:@"0.1" appChannel:@"AppStore" autoUpload:YES sendOnWifi:YES];

**设置为只在 WIFI 下发送数据，会导致服务器接收数据延迟，对统计系统结果的及时性会产生影响，不建议使用**

如需要使用自定义设备标识（比如 UDID），调用

    [[DATracker sharedTracker] startTrackerWithAppKey:@"app-key" appVersion:@"0.1" appChannel:@"AppStore" autoUpload:YES sendOnWifi:NO deviceUDID:@"id-set-by-app"];

**App Key 可从移动分析系统网站获取，不得使用为空值或者 null**

**以下所有调用均需发生在 `启用 API` 之后**

--------------------------

**手动发送数据请调用**

    [[DATracker sharedTracker] upload];
    
**设置两次数据发送的最小时间间隔，单位秒**

	- (void)setUploadInterval:(NSInteger)uploadInterval;

**本地缓存的最大事件数目**

	- (void)setUploadBulkSize:(NSInteger)uploadBulkSize;

**手动禁用自动上传**

    [[DATracker sharedTracker] setAutoUploadOn:NO];

**手动开启只在 WIFI 下发送数据**

    [[DATracker sharedTracker] setSendOnWifiOn:YES];

## 推广跟踪 ##

紧跟 `启用 API` 后调用

    [[DATracker sharedTracker] enableCampaign];

## 获取 Device ID ##

    [[DATracker sharedTracker] getDeviceId];

**该 Device ID 并非 Apple UDID, 仅用户系统本身设备去重用途, 并且可能根据 Apple 政策做相应调整, 不保证长期唯一性**

## 用户帐号管理 ##

在用户登录后，请调用如下接口，参数为用户帐号

    - (void)loginUser:(NSString *)userId;

当用户登出后，请调用

    - (void)logoutUser;

**如登录发生在需要捕捉事件后，则该事件无用户信息**

## 用户位置记录 ##

在拿到用户经纬度时, 调用如下接口记录用户位置

    - (void)setLocation:(double)latitude andLongitude:(double)longitude

## 事件捕捉 ##

调用如下方法进行事件捕捉

    - (void)trackEvent:(NSString *)eventId;
    - (void)trackEvent:(NSString *)eventId withAttributes:(NSDictionary *)attributes;

eventId 为事件标识，如 "login", "buy"

    [[DATracker sharedTracker] trackEvent:@"buy"];

attributes 为自定义字段名称，格式如 "{@"money":@"100", @"timestamp":@"1357872572"}"

可对事件发生时的其他信息进行记录

    [[DATracker sharedTracker] trackEvent:@"login"
	    withAttributes:[NSDictionary dictionaryWithObjectsAndKeys:@"100", @"money", nil]];

还可以对事件进行归类和打标签

    - (void)trackEvent:(NSString *)eventId cagtegory:(NSString *)category label:(NSString *)label;
    - (void)trackEvent:(NSString *)eventId cagtegory:(NSString *)category label:(NSString *)label withAttributes:(NSDictionary *)attributes;

如果需要记录事件发生持续时间，可调用如下接口

    - (void)trackEvent:(NSString *)eventId costTime:(int)seconds category:(NSString *)category label:(NSString *)label;
    - (void)trackEvent:(NSString *)eventId costTime:(int)seconds category:(NSString *)category label:(NSString *)label withAttributes:(NSDictionary *)attributes;

如果需要记录事件发生时的位置信息, 可调用如下接口

    - (void)trackEvent:(NSString *)eventId costTime:(int)seconds latitude:(double)latitude longitude:(double)longitude category:(NSString *)category label:(NSString *)label withAttributes:(NSDictionary *)attributes;

**事件捕捉调用虽然可以使用在任何地方，但最好不要在较多循环内或者非主线程中调用，以及最好不要使用很长 eventID 或者 key value 值，否则会增加 SDK 发送的数据量**

## 事件自定义通用属性 ##

特别地，如果某个事件的属性，在所有事件中都会出现，可以将该属性设置为事件通用属性，通用属性会保存在 App 本地，可调用如下接口：

	- (void)registerSuperProperties:(NSDictionary *)properties;

成功设置事件通用属性后，再通过 trackEvent: 追踪事件时，事件通用属性会被添加进每个事件中。重复调用 registerSuperProperties: 会覆盖之前已设置的通用属性。

如果不覆盖之前已经设定的通用属性（除非已存在的对象值和defaultValue相等），可调用：

	- (void)registerSuperPropertiesOnce:(NSDictionary *)properties;
	- (void)registerSuperPropertiesOnce:(NSDictionary *)properties defaultValue:(id)defaultValue;

查看当前已设置的通用属性，调用：

	- (NSDictionary *)currentSuperProperties;

删除一个通用属性，调用：

	- (void)unregisterSuperProperty:(NSString *)propertyName;

删除所有已设置的事件通用属性，调用：

	- (void)clearSuperProperties;

**当事件通用属性和事件属性的 Key 冲突时，事件属性优先级最高，它会覆盖事件公共属性。**

## 事件耗时统计 ##

可以通过计时器统计事件的持续时间，默认的时间单位是毫秒。首先，在事件开始时调用 trackTimer: 记录事件开始时间，该方法并不会真正发送事件，接口为

	- (void)trackTimer:(NSString *)eventId;

调用trackEvent: 时，若已记录该事件的开始时间，SDK会在追踪相关事件时自动将事件持续时间记录在事件属性中，并删除该事件定时器。

清除所有的事件定时器，调用：

	- (void)clearTrackTimer;

多次调用 trackTimer:时，相同eventId的事件的开始时间以最后一次调用时为准。

## 异常捕捉 ##

可以在 try catch block 中进行异常捕捉，传入参数为 NSException (含子类)实例

    @try {
        [@"str" characterAtIndex:10];
    }
    @catch (NSException *exception) {
        [[DATracker sharedTracker] trackException:exception];
    }

如还需要记录 Callstack，可调用

    [[DATracker sharedTracker] trackExceptionWithCallstack:exception];

如需要手动指定异常信息，可调用

    [[DATracker sharedTracker] trackExceptionWithName:@"exception" reason:@"some reason" callstack:@"long callstack"];

如需要 crash 捕捉, 需要在启用 SDK 后尽早调用如下方法来开启该功能, 并确保 App 中没有绑定过 signal 和 uncaught exception handler

- (void)enableCrashReporting;

## 屏幕 View 捕捉 ##

screenName 为当前 View 名称

    - (void)trackScreen:(NSString *)screenName;

## 搜索动作捕捉 ##

keyword 为搜索关键词，searchType 为搜索类型，比如新闻搜索，视频搜索等

    - (void)trackSearch:(NSString *)keyword in:(NSString *)searchType;

## 分享动作捕捉 ##

content 为分享内容，from 为分享发生地，to 为分享目的地，比如新浪微博，网易微博等

    -(void)trackShare:(NSString*)content from:(NSString *)from to:(NSString *)to;

## 用户任务记录 ##

对用户的任务进行记录，参数为任务 id 和任务失败原因，可用于用户行为完成，用户行为耗时等统计。

    [[DATracker sharedTracker] trackOnMissionBegan:@"mission-1"];
    [[DATracker sharedTracker] trackOnMissionAccomplished:@"mission-1"];
    [[DATracker sharedTracker] trackOnMissionFailed:@"mission-2" reason:@"no power"];

## 设置用户属性 ##

为了更准确地提供针对人群的分析服务，可以使用 SDK 的 DATracker的People 设置用户属性。用户可以在留存分析、分布分析等功能中，使用用户属性作为过滤条件，精确分析特定人群的指标。

获取当前用户信息设置实例

	[[DATracker sharedTracker] people];

设置当前用户信息属性（全局或单个）

	- (void)set:(NSDictionary *)profileDict;
	- (void)set:(NSString *) profile to:(id)content;

设置当前用户信息属性（如果该属性已经设置过，则忽略）。与 set() 方法不同的是，如果被设置的用户属性已存在，则这条记录会被忽略而不会覆盖已有数据，如果属性不存在则会自动创建。因此，setOnce() 比较适用于为用户设置首次激活时间、首次注册时间等属性。

	- (void)setOnce:(NSDictionary *)profileDict;
	- (void)setOnce:(NSString *) profile to:(id)content;

清除单个用户信息属性

	- (void)unset:(NSString *)property;

删除当前用户记录

	- (void)deleteUser;

记录用户消费接口（用于用户价值评估功能），amount代表金额，其中正数金额代表客户的消费，负数金额代表客户的退款

	- (void)trackCharge:(NSNumber *)amount;
	- (void)trackCharge:(NSNumber *)amount withProperties:(NSDictionary *)properties;

清除用户的消费记录

	- (void)clearCharges;

## 设置用户公共属性 ##

设置用户账户

    - (void)setAccount:(NSString *)account;

设置用户真实姓名

	- (void)setRealName:(NSString *)realName;

设置用户性别(0-女，1-男，2-未知)

    - (void)setGender:(NSInteger)gender;

设置用户出生日期

    - (void)setBirthday:(NSDate *)birthday;
    
批量设置用户基本信息

	- (void)setPopulationWithAccount:(NSString *)account realName:(NSString *)realName birthday:(NSDate *)birthday gender:(NSInteger)gender;

设置用户地址

    - (void)setLocation:(NSString *)country region:(NSString *)region city:(NSString *)city;

## CocoaPods安装 ##

1、在Podfile中添加
   `pod 'HubbleDataSDK'`  
2、执行`pod install` 或者 `pod update`	
	
## 手动安装 ##

将`DATracker.h` 与 `libHubbleDataSDK.a`导入到工程中