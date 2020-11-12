## GCAdapterDriver
This repository contains adapter driver(s) for macOS, specifically for controller adapters that report as Gamecube controllers. In addition, this driver will attempt to opt-in to faster polling intervals ("overclocking"), which can be particularly useful in some scenarios (e.g, FGC, Slippi, Dolphin, etc).

Successfully tested on the following:

- Official adapter.
- MayFlash 4-port adapter.

### Installation
As this is currently a pre-release (not signed) kernel extension, you will need to disable SIP. Once signing has been worked out for the varying OS's this should be turned back on.

### Kext
- Reboot into Recovery Mode, open a terminal
- Run `csrutil disable && csrutil enable --without kext`
- Reboot into normal mode
- Copy `GCAdapterDriver.kext` to `/Library/Extensions/GCAdapterDriver.kext`
- Run `sudo chown -R root:wheel /Library/Extensions/GCAdapterDriver.kext` to ensure correct permissions are set
- Run `sudo kextutil /Library/Extensions/GCAdapterDriver.kext`. It may complain about signing; this is what we're aiming to fix in the full release.

If you have a different driver installed, you may need to make sure it's uninstalled before loading this one.

### Building from Source
- Open in Xcode. Depending on what OS you're building from, you might need the Kext (10.13-10.15) or the DriverKit (10.15+) project. If you're building the Kext, ensure you're using the correct Xcode for the platform you're on.
- For Kext: `Product -> Archive -> Distribute Contents -> Built Archive`

At the moment, only the Kext is implemented; however, _kexts are deprecated moving forward_. The code as is written will port mostly fine to a DriverKit solution, I just need to find time to do it.

### Acknowledgements
- Thanks in particular to Nikki, Frisk, PurpleZ, and trickyd in the Slippi macOS Discord for their testing/input/review.
- The general idea for the structure this uses is borrowed from [this implementation for Linux](https://github.com/HannesMann/gcadapter-oc-kmod), albeit written with macOS nuances and frameworks in mind.

### Legal (MIT License)
Copyright 2020 SecretKeys LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
