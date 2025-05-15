# InternationalChallenges


### Configuring the access point on the raspberry pi 4
To configure the access point, a [debian program from pi-top](https://github.com/pi-top/Wi-Fi-Access-Point-and-Station-Mode) was used.

There were changes needed to the original script to use WPA2 instead of the insecure WPA1.

The updated version is available [here](https://github.com/kamo104/Wi-Fi-Access-Point-and-Station-Mode).

Installing is done via: 
```
  # clone the program repo
  git clone https://github.com/kamo104/Wi-Fi-Access-Point-and-Station-Mode
  cd Wi-Fi-Access-Point-and-Station-Mode
  
  # iptables is needed for the program
  sudo apt install iptables
  sudo install _wifi-ap-sta-with-network-manager /usr/local/bin/wifi-ap-sta
  
  # install the configuration
  cd [PATH-TO-THE-InternationalChallenges-REPO]
  sudo install ./rp4-config/wifi-ap-sta-config /etc/default/wifi-ap-sta
```

Then, the program can be run with:
```
  sudo wifi-ap-sta start
```

