## Kext Installer Packages
This directory contains resources for building installer packages for two kexts in this repository.

- `GCAdapterDriver`, which is a driver that attempts to poll adapters at a higher frequency to avoid dropped inputs. Competitive players often want this.
- `GCAdapterDriverPassThrough`, which is effectively a signed version of `SmashEnabler` by jackoalan. This kext is supported, theoretically, as far back as macOS 10.9.

## Steps to package/sign/notarize
Kext development and distribution, as well as packager bundling, are somewhat... odd arts these days. If you were to try and build this kext yourself, you would need to disable SIP and/or relax certain other permissions (e.g, on an M1) as it requires a certificate that's been granted the "kernel extension" requirement.

The general gist of it all is as follows, though:

### Build each driver
PassThrough:
```
pkgbuild --root ./passthrough/Products/ \
    --scripts ./passthrough/Scripts/ \
    --component-plist passthrough/GCAdapterDriverPassThroughComponents.plist \
    GCAdapterDriverPassThrough.pkg
```

Overclocked:
```
pkgbuild --root ./overclocked/Products/ \
    --scripts ./overclocked/Scripts/ \
    --component-plist overclocked/GCAdapterDriverComponents.plist \
    GCAdapterDriverOverclocked.pkg
```

### Combine the drivers
```
productbuild --package GCAdapterDriverOverclocked.pkg --package GCAdapterDriverPassThrough.pkg GCAdapterDriver.pkg
```

### Create a Product Archive
_(Depending on the version being built, you may want to change some names)_
```
productbuild --distribution ./Distribution.xml \
    --resources Resources/ \
    GCAdapterDriverInstaller.pkg
```

### Sign the Product Archive
_(Note that this requires an Installer certificate. This must be done with productsign, not codesign!)_
`/usr/bin/productsign --sign "Keychain Certificate Name" GCAdapterDriverInstaller.pkg GCAdapterDriverInstaller-Signed.pkg`

### Notarize the Product Archive
`../../notarize-kext.sh path/to/archive.pkg primary.bundle.identifier.here`

(Note that notarization will require some ENV variables, which you'll need to set up)
