pkgbuild --root ./passthrough/Products/ \
    --scripts ./passthrough/Scripts/ \
    --component-plist passthrough/GCAdapterDriverPassThroughComponents.plist \
    GCAdapterDriverPassThrough.pkg

pkgbuild --root ./overclocked/Products/ \
    --scripts ./overclocked/Scripts/ \
    --component-plist overclocked/GCAdapterDriverComponents.plist \
    GCAdapterDriverOverclocked.pkg

productbuild --package GCAdapterDriverOverclocked.pkg --package GCAdapterDriverPassThrough.pkg GCAdapterDriverInstaller.pkg

productbuild --distribution ./Distribution.xml \
    --resources Resources/ \
    GCAdapterDriverInstaller.pkg

/usr/bin/productsign --sign "Developer ID Installer: SecretKeys LLC (8JNDBG9U79)" GCAdapterDriverInstaller.pkg GCAdapterDriverInstaller-Signed.pkg

../../notarize-kext.sh GCAdapterDriverInstaller-Signed.pkg com.secretkeys.installer.GCAdapterDriver
