#import <Foundation/NSNetServices.h>
#import <Foundation/Foundation.h>

#include "oscit/zeroconf_browser.h"
#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"
#include "oscit/location.h"

using namespace oscit;

@interface NetServiceDelegate : NSObject {
  oscit::ZeroConfBrowser *master_;
  NSNetServiceBrowser *browser_;
  bool running_;
  std::string protocol_;
}

- (id)initWithDelegate:(oscit::ZeroConfBrowser *)master serviceType:(const char *)service_type protocol:(const char *)protocol;
- (void)stop;
// NSNetServiceBrowser delegate methods
- (void)netServiceBrowserWillSearch:(NSNetServiceBrowser *)browser;
- (void)netServiceBrowserDidStopSearch:(NSNetServiceBrowser *)browser;
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didNotSearch:(NSDictionary *)errorDict;
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didFindService:(NSNetService *)aNetService moreComing:(BOOL)moreComing;
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didRemoveService:(NSNetService *)aNetService moreComing:(BOOL)moreComing;

// resolving host
- (void)netServiceDidResolveAddress:(NSNetService *)service;
- (void)netService:(NSNetService *)sender didNotResolve:(NSDictionary *)errorDict;
@end

@implementation NetServiceDelegate
- (id)initWithDelegate:(oscit::ZeroConfBrowser *)master
        serviceType:(const char *)service_type
        protocol:(const char *)protocol {
  if (self = [super init]) {
    protocol_ = protocol;
    master_   = master;
    browser_  = [[NSNetServiceBrowser alloc] init];
    [browser_ setDelegate:self];
    [browser_ searchForServicesOfType:[NSString stringWithCString:service_type encoding:NSUTF8StringEncoding] inDomain:@""];
    running_ = YES;
  }
  return self;
}

- (void)stop {
  if (running_) {
    [browser_ stop];
    running_ = NO;
  }
}
// NSNetServiceBrowser delegate methods
- (void)netServiceBrowserWillSearch:(NSNetServiceBrowser *)browser {
  // ignore
}

- (void)netServiceBrowserDidStopSearch:(NSNetServiceBrowser *)browser {
  // ignore
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didNotSearch:(NSDictionary *)errorDict {
  // handle errors ....
}

- (void)netServiceWillResolve:(NSNetService *)service {
  NSLog(@"Going to resolve %@\n", [service name]);
}

- (void)netServiceDidResolveAddress:(NSNetService *)service {
  NSLog(@"\n\nResolved: %@ in %@:%i addresses: %i\n", [service name], [service hostName], [service port], [[service addresses] count]);

  NSLog(@"master: %p\n", master_);
  master_->add_device(Location(
                          protocol_.c_str(),
                          [[service name] UTF8String],
                          [[service hostName] UTF8String],
                          [service port]
                          ));

  NSLog(@"device added.\n");
  [service release];
}

- (void)netService:(NSNetService *)service didNotResolve:(NSDictionary *)errorDict {
  NSLog(@"Could not resolve %@!\n", [service name]);
  // handle errors ...
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didFindService:(NSNetService *)service moreComing:(BOOL)moreComing {
  NSLog(@"\n\nFound: %@ in %@:%i addresses: %i\n", [service name], [service hostName], [service port], [[service addresses] count]);
  [service retain];
  [service setDelegate:self];
  [service resolveWithTimeout:0.0];
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didRemoveService:(NSNetService *)aNetService moreComing:(BOOL)moreComing {
  master_->remove_device( [[aNetService name] UTF8String]);
}

-(BOOL) respondsToSelector:(SEL)selector {
  NSLog(@"SELECTOR: %@\n", NSStringFromSelector(selector));
  return [super respondsToSelector:selector];
}

-(void)dealloc {
  [self stop];
  [browser_ release];
  [super dealloc];
}

@end

namespace oscit {

class ZeroConfBrowser::Implementation {
public:
  Implementation(ZeroConfBrowser *master) {
    delegate_ = [[NetServiceDelegate alloc] initWithDelegate:master serviceType:master->service_type_.c_str() protocol:master->protocol_.c_str()];
  }

  ~Implementation() {
    [delegate_ release];
  }

  void stop() {
    [delegate_ stop];
  }

  NetServiceDelegate *delegate_;
};

ZeroConfBrowser::ZeroConfBrowser(const char *service_type) :
                  service_type_(service_type),
                  command_(NULL),
                  proxy_factory_(NULL),
                  found_devices_(FOUND_DEVICE_HASH_SIZE) {
  get_protocol_from_service_type();
  NSLog(@"master: %p\n", this);
  impl_ = new ZeroConfBrowser::Implementation(this);
}

ZeroConfBrowser::~ZeroConfBrowser() {
  delete impl_;
  if (proxy_factory_) delete proxy_factory_;
}

void ZeroConfBrowser::stop() {
  impl_->stop();
}

} // oscit



