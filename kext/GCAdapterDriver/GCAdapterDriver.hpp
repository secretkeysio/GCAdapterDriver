#include <IOKit/IOService.h>

class GCAdapterDriver : public IOService
{
OSDeclareDefaultStructors(GCAdapterDriver)

public:
    virtual bool init(OSDictionary *dictionary = 0) override;
    virtual void free(void) override;
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;
    
    void updateInterval(IOUSBHostInterface *interface);
};
