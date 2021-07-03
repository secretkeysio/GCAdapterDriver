## GCAdapterDriver
This repository contains adapter driver(s) for macOS, specifically for controller adapters that report as Gamecube controllers. In addition, this driver will attempt to opt-in to faster polling intervals ("overclocking"), which can be particularly useful in some scenarios (e.g, FGC, Slippi, Dolphin, etc).

Successfully tested on the following:

- Official adapter.
- MayFlash 4-port adapter.

This repository is fairly technical in nature - if all you want is to install support for your adapter, please [visit the project homepage](https://secretkeys.io/gcadapterdriver).

### Developing
If you'd like to try and build this yourself and/or tinker, fair warning: it's a little more obtuse than conventional macOS development. You'll likely want to temporarily disable System Integrity Protection (SIP) and/or set `systemextensionctl developer on` if you've not got the appropriate entitlements to sign your bundle with.

### Enabling Developer Mode
- Reboot into Recovery Mode, open a terminal
- Run: `csrutil disable && csrutil enable --without kext`
- M1 Macs may need to elect for permissive boot settings here
- Reboot into normal mode
- Run: `systemextensionsctl developer on` (if you're debugging in DriverKit, this enables running extensions from outside of `Applications`)

### Kexts
- You can build the Xcode project and grab the output kext (archive)
- Note that distributing a kext built here would need to be built on a 10.13 installation.
- Copy `GCAdapterDriver.kext` to `/Library/Extensions/GCAdapterDriver.kext`
- Run `sudo chown -R root:wheel /Library/Extensions/GCAdapterDriver.kext` to ensure correct permissions are set
- Run `sudo kextutil /Library/Extensions/GCAdapterDriver.kext`. It may complain about signing; this is what we're aiming to fix in the full release.

### DriverKit
- Thankfully, (slightly) easier! Just build the Xcode app. Run it, test it.

If you have a different driver installed, you may need to make sure it's uninstalled before loading this one.

### Acknowledgements
- Thanks in particular to Nikki, Frisk, PurpleZ, and trickyd in the Slippi macOS Discord for their testing/input/review.
- The general idea for the structure this uses is borrowed from [this implementation for Linux](https://github.com/HannesMann/gcadapter-oc-kmod), albeit written with macOS nuances and frameworks in mind.

### Legal (MIT License)
Copyright 2021 SecretKeys LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
