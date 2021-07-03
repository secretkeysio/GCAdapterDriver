//
//  ViewController.swift
//  GCAdapterDriver
//
//  Created by Ryan McGrath on 6/15/21.
//

import Cocoa
import SystemExtensions

var ext_identifier = "com.secretkeys.gcadapterdriverkitext"

enum ActivatingStatus {
    case activating
    case deactivating
}

class FlippedView: NSView {
    override var isFlipped: Bool {
        true
    }
}

class ViewController: NSViewController, OSSystemExtensionRequestDelegate {
    var toggle: NSSwitch?
    var titleLabel: NSTextField?
    var debugLabel: NSTextField?
    var explanationLabel: NSTextField?
    var logoView: NSImageView?
    var menuButton: NSButton?
    
    var lockout = false
    var status: ActivatingStatus = .deactivating
    
    override func loadView() {
        self.view = FlippedView()
    }
    
    override var representedObject: Any? {
        didSet {}
    }
        
    override func viewDidLoad() {
        super.viewDidLoad()
        
        logoView = NSImageView(image: NSImage(named: NSImage.Name("AppIcon"))!)
        logoView?.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(logoView!)

        titleLabel = NSTextField(labelWithString: "GCAdapterDriver v1.6")
        titleLabel?.translatesAutoresizingMaskIntoConstraints = false
        titleLabel?.font = NSFont.boldSystemFont(ofSize: 14.0)
        view.addSubview(titleLabel!)
        
        toggle = NSSwitch()
        toggle?.translatesAutoresizingMaskIntoConstraints = false
        toggle?.target = self
        toggle?.action = #selector(doRequest)
        view.addSubview(toggle!)
        
        explanationLabel = NSTextField(wrappingLabelWithString: "This software is offered as-is, without warranty. Nothing bad should happen, but by activating you hereby acknowledge this.\n\nYou may see a message that the extension was blocked, in which case you can open System Preferences and allow it via your Security tab.\n\nYou may want to unplug and re-plug your adapter after activating to ensure detection.\n\nDepending on the app you're trying to use, you may need to restart it.")
        explanationLabel?.translatesAutoresizingMaskIntoConstraints = false
        explanationLabel?.lineBreakMode = .byWordWrapping
        view.addSubview(explanationLabel!)

        debugLabel = NSTextField(wrappingLabelWithString: "Deactivated.")
        debugLabel?.textColor = .systemRed
        debugLabel?.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(debugLabel!)
        
        let icon = NSImage(systemSymbolName: "gear", accessibilityDescription: "")
        menuButton = NSButton(image: icon!, target: self, action: #selector(showMenu))
        menuButton?.translatesAutoresizingMaskIntoConstraints = false
        menuButton?.isBordered = false
        view.addSubview(menuButton!)
        
        NSLayoutConstraint.activate([
            logoView!.topAnchor.constraint(equalTo: view.topAnchor, constant: 16.0),
            logoView!.leftAnchor.constraint(equalTo: view.leftAnchor, constant: 12.0),
            logoView!.widthAnchor.constraint(equalToConstant: 100.0),
            logoView!.heightAnchor.constraint(equalToConstant: 100.0),

            titleLabel!.topAnchor.constraint(equalTo: view.topAnchor, constant: 40.0),
            titleLabel!.leftAnchor.constraint(equalTo: logoView!.rightAnchor, constant: 8.0),
            titleLabel!.rightAnchor.constraint(equalTo: view.rightAnchor, constant: -16.0),
            
            toggle!.topAnchor.constraint(equalTo: titleLabel!.bottomAnchor, constant: 12.0),
            toggle!.leftAnchor.constraint(equalTo: logoView!.rightAnchor, constant: 8.0),
            
            debugLabel!.topAnchor.constraint(equalTo: logoView!.bottomAnchor, constant: 12.0),
            debugLabel!.leftAnchor.constraint(equalTo: view.leftAnchor, constant: 26.0),
            debugLabel!.rightAnchor.constraint(equalTo: view.rightAnchor, constant: -26.0),
            
            explanationLabel!.topAnchor.constraint(equalTo: debugLabel!.bottomAnchor, constant: 16.0),
            explanationLabel!.leftAnchor.constraint(equalTo: view.leftAnchor, constant: 26.0),
            explanationLabel!.rightAnchor.constraint(equalTo: view.rightAnchor, constant: -26.0),
            explanationLabel!.bottomAnchor.constraint(equalTo: view.bottomAnchor, constant: -26.0),
            
            menuButton!.topAnchor.constraint(equalTo: view.topAnchor, constant: 12.0),
            menuButton!.rightAnchor.constraint(equalTo: view.rightAnchor, constant: -8.0)
        ])
    }

    // This is a bit hacky for showing the menu, but screw it.
    @objc func showMenu() {
        let menu = NSMenu()

        let quitItem = NSMenuItem(
            title: "Quit GCAdapterDriver",
            action: #selector(NSApp.terminate(_:)),
            keyEquivalent: ""
        )
        menu.addItem(quitItem)
        menu.popUp(
            positioning: quitItem,
            at: NSPoint(x: 0.0, y: menuButton!.frame.size.height),
            in: menuButton!
        )
    }

    @objc func doRequest() {
        if status == .activating {
            deactivate()
            status = .deactivating
        } else {
            activate()
            status = .activating
        }
    }
    
    func request(_ request: OSSystemExtensionRequest, actionForReplacingExtension existing: OSSystemExtensionProperties, withExtension ext: OSSystemExtensionProperties) -> OSSystemExtensionRequest.ReplacementAction {
        .replace
    }
    
    func requestNeedsUserApproval(_ request: OSSystemExtensionRequest) {
        debugLabel?.textColor = .systemYellow
        debugLabel?.stringValue = "Approve the extension in your System Preferences > Security screen to continue."
    }
    
    func request(_ request: OSSystemExtensionRequest, didFinishWithResult result: OSSystemExtensionRequest.Result) {
        
        switch result {
            case .completed:
                if status == .activating {
                    debugLabel?.textColor = .systemGreen
                    debugLabel?.stringValue = "Activated!"
                } else {
                    debugLabel?.textColor = .systemRed
                    debugLabel?.stringValue = "Deactivated."
                }
                
            case .willCompleteAfterReboot:
                debugLabel?.textColor = .systemYellow
                debugLabel?.stringValue = "Restart your Mac to finish activating the driver."
                
            default:
                debugLabel?.textColor = .systemYellow
                debugLabel?.stringValue = "Unexpected completion value received: \(result.rawValue)"
        }
    }
    
    func request(_ request: OSSystemExtensionRequest, didFailWithError error: Error) {
        debugLabel?.textColor = .systemRed
        
        let text = status == .activating ? "activate" : "deactivate"
        debugLabel?.stringValue = "Failed to \(text) extension!\n\nError: \(error)\n\nDetails: \(error.localizedDescription)"
    }
    
    @objc func activate() {
        let request = OSSystemExtensionRequest.activationRequest(
            forExtensionWithIdentifier: ext_identifier,
            queue: DispatchQueue.main
        )
        
        request.delegate = self
        let manager = OSSystemExtensionManager.shared
        
        manager.submitRequest(request)
    }

    @objc func deactivate() {
        let request = OSSystemExtensionRequest.deactivationRequest(
            forExtensionWithIdentifier: ext_identifier,
            queue: DispatchQueue.main
        )
        request.delegate = self
        let manager = OSSystemExtensionManager.shared
        
        manager.submitRequest(request)
    }
}
