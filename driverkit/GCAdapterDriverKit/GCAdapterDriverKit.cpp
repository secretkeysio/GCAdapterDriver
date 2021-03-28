//
//  GCAdapterDriverKit.cpp
//  GCAdapterDriverKit
//
//  Created by Ryan McGrath on 1/3/21.
//

#include <os/log.h>

#include <DriverKit/IOUserServer.h>
#include <DriverKit/IOLib.h>

#include <USBDriverKit/USBDriverKit.h>

#include "GCAdapterDriverKit.h"

kern_return_t
IMPL(GCAdapterDriverKit, Start)
{
    kern_return_t ret;
    ret = Start(provider, SUPERDISPATCH);
    os_log(OS_LOG_DEFAULT, "Hello World");
    return ret;
}

kern_return_t
IMPL(GCAdapterDriverKit, Stop)
{
    kern_return_t ret;
    ret = Stop(provider, SUPERDISPATCH);
    os_log(OS_LOG_DEFAULT, "Goodbye World");
    return ret;
}

// Given a USB interface, walks the endpoints and attempts to update them to poll at a more
// preferable rate.
//
// Also accepts a desired interval to try and set the device (adapter) to.
// The general math is: divide 1000 by bInterval. Want 1000hz? 1. Want 500hz? 2. Etc.
void updateInterval(IOUSBHostInterface *interface, uint8_t desired_interval)
{
    const IOUSBConfigurationDescriptor *configDescriptor = interface->CopyConfigurationDescriptor();
    const IOUSBInterfaceDescriptor *interfaceDescriptor = interface->GetInterfaceDescriptor(configDescriptor);

    const IOUSBEndpointDescriptor *endpointDescriptor = NULL;

    while((
        endpointDescriptor = IOUSBGetNextEndpointDescriptor(
            configDescriptor,
            interfaceDescriptor,
            (IOUSBDescriptorHeader*)endpointDescriptor
        )
    ) != NULL) {
        if(endpointDescriptor->bEndpointAddress == 0x81 || endpointDescriptor->bEndpointAddress == 0x82) {
            IOUSBHostPipe *pipe = NULL;
            interface->CopyPipe(endpointDescriptor->bEndpointAddress, &pipe);
            
            if(pipe) {
                os_log(OS_LOG_DEFAULT, "Acquired pipe, will attempt to update bInterval");
                
                IOUSBStandardEndpointDescriptors *descriptors = NULL;
                pipe->GetDescriptors(descriptors, kIOUSBGetEndpointDescriptorOriginal);
                
                if(descriptors) {
                    descriptors->descriptor.bInterval = desired_interval;
                    IOReturn result = pipe->AdjustPipe(descriptors);
                    os_log(OS_LOG_DEFAULT, "Status: %d", result);
                }
            
                pipe->release();
            } else {
                os_log(OS_LOG_DEFAULT, "Unable to acquire pipe to modify interval!");
            }
        }
    }
}
