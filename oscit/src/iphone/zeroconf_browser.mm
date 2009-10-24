#include "oscit/zeroconf.h"
#import <Foundation/NSNetServices.h>
#import <Foundation/Foundation.h>

@interface NetServiceDelegate : NSObject {
  oscit::ZeroConfBrowser *master_;
  NSNetServiceBrowser *browser_;
  bool running_;
}

- (id)initWithDelegate:(oscit::ZeroConfBrowser *)master serviceType:(const char *)service_type;
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
- (id)initWithDelegate:(oscit::ZeroConfBrowser *)master serviceType:(const char *)service_type {
    if (self = [super init]) {
    master_  = master;
    browser_ = [[NSNetServiceBrowser alloc] init];
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
  //[[service hostName] UTF8String]

  master_->add_device(Location(
                        master_->protocol_.c_str(),
                        [[service name] UTF8String],
                        Location::ANY_IP, // FIXME: get IP !
                        [service port]
                        ));
}

- (void)netService:(NSNetService *)service didNotResolve:(NSDictionary *)errorDict {
  NSLog(@"Could not resolve %@!\n", [service name]);
  // handle errors ...
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didFindService:(NSNetService *)service moreComing:(BOOL)moreComing {
  NSLog(@"\n\nFound: %@ in %@:%i addresses: %i\n", [service name], [service hostName], [service port], [[service addresses] count]);
  [service setDelegate:self];
  [service resolveWithTimeout:0.0];
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didRemoveService:(NSNetService *)aNetService moreComing:(BOOL)moreComing {
  master_->remove_device( [[aNetService name] UTF8String], moreComing);
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
    delegate_ = [[NetServiceDelegate alloc] initWithDelegate:master serviceType:master->service_type_.c_str()];
  }

  ~Implementation() {
    [delegate_ release];
  }

  void stop() {
    [delegate_ stop];
  }

  NetServiceDelegate *delegate_;
};

ZeroConfBrowser::ZeroConfBrowser(const char *service_type) : service_type_(service_type) {
  get_protocol_from_service_type();
  impl_ = new ZeroConfBrowser::Implementation(this);
}

ZeroConfBrowser::~ZeroConfBrowser() {
  delete impl_;
}

void ZeroConfBrowser::stop() {
  impl_->stop();
}

} // oscit



