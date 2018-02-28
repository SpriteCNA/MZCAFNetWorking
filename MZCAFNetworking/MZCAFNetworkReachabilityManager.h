// AFNetworkReachabilityManager.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>

#if !TARGET_OS_WATCH
#import <SystemConfiguration/SystemConfiguration.h>

typedef NS_ENUM(NSInteger, AFNetworkReachabilityStatus) {
    AFNetworkReachabilityStatusUnknown          = -1,
    AFNetworkReachabilityStatusNotReachable     = 0,
    AFNetworkReachabilityStatusReachableViaWWAN = 1,
    AFNetworkReachabilityStatusReachableViaWiFi = 2,
};

NS_ASSUME_NONNULL_BEGIN

/**
 `AFNetworkReachabilityManager` monitors the MZCReachability of domains, and addresses for both WWAN and WiFi network interfaces.

 MZCReachability can be used to determine background information about why a network operation failed, or to trigger a network operation retrying when a connection is established. It should not be used to prevent a user from initiating a network request, as it's possible that an initial request may be required to establish MZCReachability.

 See Apple's MZCReachability Sample Code ( https://developer.apple.com/library/ios/samplecode/MZCReachability/ )

 @warning Instances of `AFNetworkReachabilityManager` must be started with `-startMonitoring` before MZCReachability status can be determined.
 */
@interface MZCAFNetworkReachabilityManager : NSObject

/**
 The current network MZCReachability status.
 */
@property (readonly, nonatomic, assign) AFNetworkReachabilityStatus networkReachabilityStatus;

/**
 Whether or not the network is currently reachable.
 */
@property (readonly, nonatomic, assign, getter = isReachable) BOOL reachable;

/**
 Whether or not the network is currently reachable via WWAN.
 */
@property (readonly, nonatomic, assign, getter = isReachableViaWWAN) BOOL reachableViaWWAN;

/**
 Whether or not the network is currently reachable via WiFi.
 */
@property (readonly, nonatomic, assign, getter = isReachableViaWiFi) BOOL reachableViaWiFi;

///---------------------
/// @name Initialization
///---------------------

/**
 Returns the shared network MZCReachability manager.
 */
+ (instancetype)sharedManager;

/**
 Creates and returns a network MZCReachability manager with the default socket address.
 
 @return An initialized network MZCReachability manager, actively monitoring the default socket address.
 */
+ (instancetype)manager;

/**
 Creates and returns a network MZCReachability manager for the specified domain.

 @param domain The domain used to evaluate network MZCReachability.

 @return An initialized network MZCReachability manager, actively monitoring the specified domain.
 */
+ (instancetype)managerForDomain:(NSString *)domain;

/**
 Creates and returns a network MZCReachability manager for the socket address.

 @param address The socket address (`sockaddr_in6`) used to evaluate network MZCReachability.

 @return An initialized network MZCReachability manager, actively monitoring the specified socket address.
 */
+ (instancetype)managerForAddress:(const void *)address;

/**
 Initializes an instance of a network MZCReachability manager from the specified MZCReachability object.

 @param MZCReachability The MZCReachability object to monitor.

 @return An initialized network MZCReachability manager, actively monitoring the specified MZCReachability.
 */
- (instancetype)initWithReachability:(SCNetworkReachabilityRef)MZCReachability NS_DESIGNATED_INITIALIZER;

///--------------------------------------------------
/// @name Starting & Stopping MZCReachability Monitoring
///--------------------------------------------------

/**
 Starts monitoring for changes in network MZCReachability status.
 */
- (void)startMonitoring;

/**
 Stops monitoring for changes in network MZCReachability status.
 */
- (void)stopMonitoring;

///-------------------------------------------------
/// @name Getting Localized MZCReachability Description
///-------------------------------------------------

/**
 Returns a localized string representation of the current network MZCReachability status.
 */
- (NSString *)localizedNetworkReachabilityStatusString;

///---------------------------------------------------
/// @name Setting Network MZCReachability Change Callback
///---------------------------------------------------

/**
 Sets a callback to be executed when the network availability of the `baseURL` host changes.

 @param block A block object to be executed when the network availability of the `baseURL` host changes.. This block has no return value and takes a single argument which represents the various MZCReachability states from the device to the `baseURL`.
 */
- (void)setReachabilityStatusChangeBlock:(nullable void (^)(AFNetworkReachabilityStatus status))block;

@end

///----------------
/// @name Constants
///----------------

/**
 ## Network MZCReachability

 The following constants are provided by `AFNetworkReachabilityManager` as possible network MZCReachability statuses.

 enum {
 AFNetworkReachabilityStatusUnknown,
 AFNetworkReachabilityStatusNotReachable,
 AFNetworkReachabilityStatusReachableViaWWAN,
 AFNetworkReachabilityStatusReachableViaWiFi,
 }

 `AFNetworkReachabilityStatusUnknown`
 The `baseURL` host MZCReachability is not known.

 `AFNetworkReachabilityStatusNotReachable`
 The `baseURL` host cannot be reached.

 `AFNetworkReachabilityStatusReachableViaWWAN`
 The `baseURL` host can be reached via a cellular connection, such as EDGE or GPRS.

 `AFNetworkReachabilityStatusReachableViaWiFi`
 The `baseURL` host can be reached via a Wi-Fi connection.

 ### Keys for Notification UserInfo Dictionary

 Strings that are used as keys in a `userInfo` dictionary in a network MZCReachability status change notification.

 `MZCAFNetworkingReachabilityNotificationStatusItem`
 A key in the userInfo dictionary in a `MZCAFNetworkingReachabilityDidChangeNotification` notification.
 The corresponding value is an `NSNumber` object representing the `AFNetworkReachabilityStatus` value for the current MZCReachability status.
 */

///--------------------
/// @name Notifications
///--------------------

/**
 Posted when network MZCReachability changes.
 This notification assigns no notification object. The `userInfo` dictionary contains an `NSNumber` object under the `MZCAFNetworkingReachabilityNotificationStatusItem` key, representing the `AFNetworkReachabilityStatus` value for the current network MZCReachability.

 @warning In order for network MZCReachability to be monitored, include the `SystemConfiguration` framework in the active target's "Link Binary With Library" build phase, and add `#import <SystemConfiguration/SystemConfiguration.h>` to the header prefix of the project (`Prefix.pch`).
 */
FOUNDATION_EXPORT NSString * const MZCAFNetworkingReachabilityDidChangeNotification;
FOUNDATION_EXPORT NSString * const MZCAFNetworkingReachabilityNotificationStatusItem;

///--------------------
/// @name Functions
///--------------------

/**
 Returns a localized string representation of an `AFNetworkReachabilityStatus` value.
 */
FOUNDATION_EXPORT NSString * MZCAFStringFromNetworkReachabilityStatus(AFNetworkReachabilityStatus status);

NS_ASSUME_NONNULL_END
#endif
