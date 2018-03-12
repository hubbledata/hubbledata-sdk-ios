Pod::Spec.new do |s|
  s.name         = "HubbleDataSDK"
  s.version      = "2.1.5"
  s.summary      = "HubbleDataSDK"
  s.homepage     = "https://da.netease.com/"
  s.license      = { :type => 'MIT', :file => 'LICENSE.md' }
  s.author       = { "马守磊" => "hzmashoulei@corp.netease.com" }
  s.source       = { :git => "https://github.com/hubbledata/hubbledata-sdk-ios.git" , :tag => "v#{s.version}"}
  s.platform     = :ios, "7.0"
  s.ios.deployment_target = '7.0'

   s.source_files = '*.h'
   s.preserve_paths = '*.h','libHubbleDataSDK.a'
   s.vendored_libraries = 'libHubbleDataSDK.a'
   s.libraries = 'z','sqlite3'
   s.frameworks = 'SystemConfiguration', 'SafariServices', 'CoreTelephony', 'CoreLocation','UIKit','Security','AdSupport','Foundation'
   s.xcconfig = {'HEADER_SEARCH_PATHS' => "${PODS_ROOT}/#{s.name}" ,'LIBRARY_SEARCH_PATHS' => "${PODS_ROOT}/#{s.name}"}
  
end