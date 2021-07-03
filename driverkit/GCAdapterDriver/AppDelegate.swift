//
//  AppDelegate.swift
//  GCAdapterDriver
//
//  Created by Ryan McGrath on 6/15/21.
//

import Cocoa
import SystemExtensions

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    var popover: NSPopover!
    var statusBarItem: NSStatusItem!
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        let popover = NSPopover()
        popover.contentSize = NSSize(width: 400, height: 400)
        popover.behavior = .transient
        popover.contentViewController = ViewController()
        self.popover = popover
        
        self.statusBarItem = NSStatusBar.system.statusItem(withLength: CGFloat(NSStatusItem.variableLength))
        
        if let button = self.statusBarItem.button {
            // We resize it before setting, because using a vector here is an odd level of pain to begin with.
            let image = NSImage(named: "MenuBarIcon")
            let resizedLogo = NSImage(size: NSSize(width: 16, height: 16), flipped: false) { (dstRect) -> Bool in
                image!.draw(in: dstRect)
                return true
            }
            resizedLogo.isTemplate = true
            button.image = resizedLogo
            button.action = #selector(togglePopover(_:))
        }
    }

    // Hides or shows the menu bar popover.
    @objc func togglePopover(_ sender: AnyObject?) {
         if let button = self.statusBarItem.button {
              if self.popover.isShown {
                   self.popover.performClose(sender)
              } else {
                   self.popover.show(relativeTo: button.bounds, of: button, preferredEdge: NSRectEdge.minY)
              }
         }
    }
}
