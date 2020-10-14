#include <IOKit/IOLib.h>
#include <IOKit/usb/IOUSBHostInterface.h>
#include <os/log.h>

#include "GCAdapterDriver.hpp"

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(GCAdapterDriver, IOService)
 
// Define the driver's superclass.
#define super IOService
 
#define V_DEBUG 1
#define V_NOTE 2
#define V_ERROR 3

#define DEBUGLEVEL V_DEBUG
//#define DEBUGLEVEL V_NOTE

// Reading kernel logs post-Sierra is... fun. If you want to see the recent logs, you can do one of
// two things:
//
//  - Open Console.app, and type "prefix:kernel" into the search/filter bar. This will include
//      Kernel debugging logs, but for whatever reason the logs will often be a jumble of strings.
//
//  - Run the following in a terminal:
//      log show --info --debug --predicate 'senderImagePath contains "GCAdapterDriver"'
//
#define doLog(verbosity, s, ...) do { \
if (verbosity >= DEBUGLEVEL) IOLog("%s: " s "\n", __func__, ##__VA_ARGS__); } while(0)

// The desired bInterval to set the adapter to. We default to trying 1; you may want to tweak this
// depending on your hardware.
//
// The general math is: divide 1000 by bInterval. Want 1000hz? 1. Want 500hz? 2. Etc.
static uint8_t DESIRED_BINTERVAL = 1;

// Logging, mostly. If we need or want to allocate any memory, it should be done here.
bool GCAdapterDriver::init(OSDictionary *dict)
{
    bool result = super::init(dict);
    doLog(V_DEBUG, "Initializing");
    return result;
}

// Logging. If we need to clean up after ourselves, it should happen here.
void GCAdapterDriver::free(void)
{
    doLog(V_DEBUG, "Freeing");
    super::free();
}

// We don't opt to do anything special here, preferring to just set a high probe score
// in our Info.plist and have the system handle it.
IOService *GCAdapterDriver::probe(IOService *provider, SInt32 *score)
{
    doLog(V_DEBUG, "(score %i)\n", *score);
    IOService *result = super::probe(provider, score);
    return result;
}

// Called after probing; important to note - super::start(), THEN update the intervals. Trying to
// do it before will cause cryptic errors.
bool GCAdapterDriver::start(IOService *provider)
{
    bool result = super::start(provider);
    doLog(V_DEBUG, "Starting");

    IOUSBHostInterface *interface = OSDynamicCast(IOUSBHostInterface, provider);
    updateInterval(interface);
    
    return result;
}

// Mostly logging hooks - by this point, the device should be closed anyway.
void GCAdapterDriver::stop(IOService *provider)
{
    doLog(V_DEBUG, "Stopping");
    super::stop(provider);
}

// Given a USB interface, walks the endpoints and attempts to update them to poll at a more
// preferable rate.
void GCAdapterDriver::updateInterval(IOUSBHostInterface *interface)
{
    const StandardUSB::ConfigurationDescriptor *configDescriptor = interface->getConfigurationDescriptor();
    const StandardUSB::InterfaceDescriptor *interfaceDescriptor = interface->getInterfaceDescriptor();

    const EndpointDescriptor *endpointDescriptor = NULL;
    
    while((
        endpointDescriptor = StandardUSB::getNextEndpointDescriptor(
            configDescriptor,
            interfaceDescriptor,
            endpointDescriptor
        ))
    ) {
        if(endpointDescriptor->bEndpointAddress == 0x81 || endpointDescriptor->bEndpointAddress == 0x02) {
            IOUSBHostPipe *pipe = interface->copyPipe(StandardUSB::getEndpointAddress(endpointDescriptor));
            
            if(pipe) {
                doLog(V_DEBUG, "Acquired pipe, will attempt to update bInterval");
                
                EndpointDescriptor endpointDescriptorCopy = *endpointDescriptor;
                endpointDescriptorCopy.bInterval = DESIRED_BINTERVAL;

                IOReturn result = pipe->adjustPipe(&endpointDescriptorCopy, NULL);
                doLog(V_DEBUG, "Status: %s", IOService::stringFromReturn(result));

                pipe->release();
            } else {
                doLog(V_DEBUG, "Unable to acquire pipe to modify!");
            }
        }
    }
}
