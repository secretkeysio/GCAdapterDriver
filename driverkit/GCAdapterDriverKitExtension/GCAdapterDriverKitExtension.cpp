//
//  GCAdapterDriverKitExtension.cpp
//  GCAdapterDriverKitExtension
//
//  Created by Ryan McGrath on 6/15/21.
//

#include <os/log.h>

#include <DriverKit/IOUserServer.h>
#include <DriverKit/IOLib.h>

#include <USBDriverKit/USBDriverKit.h>

#include "GCAdapterDriverKitExtension.h"

#define __Require(assertion, exceptionLabel)                    \
do {                                                            \
    if ( __builtin_expect(!(assertion), 0) ) {                  \
        goto exceptionLabel;                                    \
    }                                                           \
} while (0)

#define __Require_Action(assertion, exceptionLabel, action)     \
do {                                                            \
    if ( __builtin_expect(!(assertion), 0) ) {                  \
        {                                                       \
            action;                                             \
        }                                                       \
        goto exceptionLabel;                                    \
    }                                                           \
} while (0)

kern_return_t
IMPL(GCAdapterDriverKitExtension, Start)
{
    kern_return_t ret;
/*    uint8_t desired_interval = 1;
    
    IOUSBHostInterface *interface = NULL;
    const IOUSBConfigurationDescriptor *configDescriptor;
    const IOUSBInterfaceDescriptor *interfaceDescriptor;
    const IOUSBEndpointDescriptor *endpointDescriptor = NULL;
*/
    os_log(OS_LOG_DEFAULT, "GCAdapterDriver Starting");
    ret = Start(provider, SUPERDISPATCH);
//    __Require(kIOReturnSuccess == ret, Exit);
    
/*
    interface = OSDynamicCast(IOUSBHostInterface, provider);
    __Require_Action(NULL != interface, Exit, ret = kIOReturnNoDevice);
    
//    ret = interface->Open(this, 0, NULL);
//    __Require_Action(kIOReturnSuccess == ret, Exit, ret = kIOReturnNotOpen);
    
    configDescriptor = interface->CopyConfigurationDescriptor();
    interfaceDescriptor = interface->GetInterfaceDescriptor(configDescriptor);

    while((
        endpointDescriptor = IOUSBGetNextEndpointDescriptor(
            configDescriptor,
            interfaceDescriptor,
            (IOUSBDescriptorHeader*)endpointDescriptor
        )
    ) != NULL) {
        os_log(OS_LOG_DEFAULT, "Walking endpoint descriptor... %i", endpointDescriptor->bEndpointAddress);

        if(endpointDescriptor->bEndpointAddress == 0x81) {
            os_log(OS_LOG_DEFAULT, "bEndpointAddress 0x81");
        } else if(endpointDescriptor->bEndpointAddress == 0x82) {
            os_log(OS_LOG_DEFAULT, "bEndpointAddress 0x82");
        }
        
        if(endpointDescriptor->bEndpointAddress == 0x81 || endpointDescriptor->bEndpointAddress == 0x82) {
            IOUSBHostPipe *pipe = NULL;
            interface->CopyPipe(endpointDescriptor->bEndpointAddress, &pipe);
            
            if(pipe) {
                os_log(OS_LOG_DEFAULT, "Acquired pipe, will attempt to update bInterval");
                
                // The problem area - GetDescriptors returns fine but descriptors remains NULL.
                IOUSBStandardEndpointDescriptors *descriptors = NULL;
                kern_return_t opt = pipe->GetDescriptors(descriptors, kIOUSBGetEndpointDescriptorOriginal);
                os_log(OS_LOG_DEFAULT, "GetDescriptors ret: %i", opt);

                if(descriptors) {
                    descriptors->descriptor.bInterval = desired_interval;
                    IOReturn result = pipe->AdjustPipe(descriptors);
                    os_log(OS_LOG_DEFAULT, "Status: %d", result);
                } else {
                    os_log(OS_LOG_DEFAULT, "No descriptors found?!");
                }
            
                pipe->release();
            } else {
                os_log(OS_LOG_DEFAULT, "Unable to acquire pipe to modify interval!");
            }
        }
    }

Exit:
//    if(interface != NULL)
//        interface->Close(this, 0);
 */

    return ret;
}

kern_return_t
IMPL(GCAdapterDriverKitExtension, Stop)
{
    kern_return_t ret;
    ret = Stop(provider, SUPERDISPATCH);
    os_log(OS_LOG_DEFAULT, "GCAdapterDriver shutting down");
    return ret;
}

// Given a USB interface, walks the endpoints and attempts to update them to poll at a more
// preferable rate.
//
// Also accepts a desired interval to try and set the device (adapter) to.
// The general math is: divide 1000 by bInterval. Want 1000hz? 1. Want 500hz? 2. Etc.
//void updateInterval(IOUSBHostInterface *interface, uint8_t desired_interval)
//{
//    const IOUSBConfigurationDescriptor *configDescriptor = interface->CopyConfigurationDescriptor();
//    const IOUSBInterfaceDescriptor *interfaceDescriptor = interface->GetInterfaceDescriptor(configDescriptor);
//
//    const IOUSBEndpointDescriptor *endpointDescriptor = NULL;
//
//    while((
//        endpointDescriptor = IOUSBGetNextEndpointDescriptor(
//            configDescriptor,
//            interfaceDescriptor,
//            (IOUSBDescriptorHeader*)endpointDescriptor
//        )
//    ) != NULL) {
//        if(endpointDescriptor->bEndpointAddress == 0x81 || endpointDescriptor->bEndpointAddress == 0x82) {
//            IOUSBHostPipe *pipe = NULL;
//            interface->CopyPipe(endpointDescriptor->bEndpointAddress, &pipe);
//
//            if(pipe) {
//                os_log(OS_LOG_DEFAULT, "Acquired pipe, will attempt to update bInterval");
//
//                IOUSBStandardEndpointDescriptors *descriptors = NULL;
//                pipe->GetDescriptors(descriptors, kIOUSBGetEndpointDescriptorOriginal);
//
//                if(descriptors) {
//                    descriptors->descriptor.bInterval = desired_interval;
//                    IOReturn result = pipe->AdjustPipe(descriptors);
//                    os_log(OS_LOG_DEFAULT, "Status: %d", result);
//                }
//
//                pipe->release();
//            } else {
//                os_log(OS_LOG_DEFAULT, "Unable to acquire pipe to modify interval!");
//            }
//        }
//    }
//}
