//
//  ViewController.swift
//  Pet Tracker Message App
//
//  Created by Le, Jaden T on 2/14/23.
//

import UIKit
import QuartzCore
import CoreBluetooth

class ViewController: UIViewController, UITextFieldDelegate, BluetoothSerialDelegate {
   
    var alert = UIAlertController()
    

    @IBOutlet weak var MessageField: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        serial = BluetoothSerial(delegate: self)
        
        // Do any additional setup after loading the view.
    }


    @IBAction func MessageTyped(_ sender: UITextField) {
        if (MessageField.text!.count > 32){
            alert = UIAlertController(title: "Warning", message: "Message is over 32 characters.", preferredStyle: UIAlertController.Style.alert)
            alert.addAction(UIAlertAction(title: "OK", style: UIAlertAction.Style.default, handler: nil))
            self.present(alert, animated: true, completion: nil)
        }else if ((MessageField.text?.contains("`")) == true){
            alert = UIAlertController(title: "Warning", message: "Message contains ` plese remove.", preferredStyle: UIAlertController.Style.alert)
            alert.addAction(UIAlertAction(title: "OK", style: UIAlertAction.Style.default, handler: nil))
            self.present(alert, animated: true, completion: nil)
        } else {
            serial.sendMessageToDevice(MessageField.text!)
        }
    }
    
    
    func serialDidChangeState() {
        //
    }
    
    func serialDidDisconnect(_ peripheral: CBPeripheral, error: NSError?) {
        
    }
}

