# 🚀 Packet Odyssey - Network Packet Journey Simulator

A beginner-friendly Python application that visualizes the journey of network packets across the OSI layers. Perfect for cybersecurity beginners and Red/Blue team preparation.

## 📋 Table of Contents

- [Features](#-features)
- [Installation](#-installation)
- [Usage](#-usage)
- [OSI Model Overview](#-osi-model-overview)
- [Educational Content](#-educational-content)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)

## ✨ Features

### 🔍 DNS Resolution
- Real-time domain name resolution with visual feedback
- Support for multiple DNS servers (Google, Cloudflare, OpenDNS)
- Reverse DNS lookup capabilities
- Simulation mode for offline learning

### 📦 OSI Layer Encapsulation
- **Layer 7 (Application)**: HTTP request visualization
- **Layer 6 (Presentation)**: Data formatting and encryption
- **Layer 5 (Session)**: Session management
- **Layer 4 (Transport)**: TCP segment creation
- **Layer 3 (Network)**: IP packet construction
- **Layer 2 (Data Link)**: Ethernet frame assembly
- **Layer 1 (Physical)**: Physical transmission details

### 🗺️ Traceroute Analysis
- Cross-platform traceroute functionality
- Hop-by-hop network path visualization
- Latency and packet loss analysis
- Network topology insights

### 📊 Packet Capture
- Real packet capture using Scapy
- Traffic analysis and pattern detection
- PCAP file export capabilities
- Suspicious activity detection

### 🎨 Rich CLI Interface
- Colorful, interactive terminal interface
- Progress indicators and animations
- Detailed tables and visualizations
- Educational explanations throughout

## 🛠️ Installation

### Prerequisites

- Python 3.10 or higher
- Administrator/root privileges (for real packet capture)

### Step 1: Clone the Repository

```bash
git clone <repository-url>
cd cybesec/Fase\ 0/
```

### Step 2: Install Dependencies

```bash
pip install -r requirements.txt
```

### Step 3: Verify Installation

```bash
python main.py --help
```

## 🚀 Usage

### Interactive Mode (Recommended for Beginners)

```bash
python main.py
```

This launches the interactive menu where you can explore each module step by step.

### Command Line Mode

#### Complete Packet Journey
```bash
python main.py --full-journey example.com
```

#### DNS Resolution Only
```bash
python main.py --dns google.com
```

#### Packet Encapsulation
```bash
python main.py --encapsulation example.com
```

#### Traceroute Analysis
```bash
python main.py --traceroute github.com
```

#### Packet Capture
```bash
python main.py --capture stackoverflow.com
```

#### Simulation Mode (No Internet Required)
```bash
python main.py --simulate --full-journey
```

### Advanced Usage Examples

```bash
# Capture packets for 30 seconds
python main.py --capture --simulate

# Traceroute with custom settings
python main.py --traceroute 8.8.8.8

# Full journey with custom target
python main.py --full-journey github.com
```

## 🌐 OSI Model Overview

### Layer 7: Application Layer
- **Purpose**: User interface and application services
- **Protocols**: HTTP, FTP, SMTP, DNS, SSH
- **Data Unit**: Message
- **Cybersecurity**: Application-level attacks (SQL injection, XSS)

### Layer 6: Presentation Layer
- **Purpose**: Data formatting, encryption, compression
- **Protocols**: SSL/TLS, JPEG, MPEG, ASCII
- **Data Unit**: Message
- **Cybersecurity**: Encryption/decryption, certificate validation

### Layer 5: Session Layer
- **Purpose**: Session management and synchronization
- **Protocols**: NetBIOS, RPC, SQL, NFS
- **Data Unit**: Message
- **Cybersecurity**: Session hijacking, authentication

### Layer 4: Transport Layer
- **Purpose**: End-to-end communication and reliability
- **Protocols**: TCP, UDP, SCTP
- **Data Unit**: Segment (TCP) / Datagram (UDP)
- **Cybersecurity**: Port scanning, SYN floods, connection hijacking

### Layer 3: Network Layer
- **Purpose**: Logical addressing and routing
- **Protocols**: IP, ICMP, OSPF, BGP
- **Data Unit**: Packet
- **Cybersecurity**: IP spoofing, routing attacks, DDoS

### Layer 2: Data Link Layer
- **Purpose**: Physical addressing and error detection
- **Protocols**: Ethernet, WiFi, PPP, Frame Relay
- **Data Unit**: Frame
- **Cybersecurity**: ARP spoofing, MAC flooding, VLAN hopping

### Layer 1: Physical Layer
- **Purpose**: Physical transmission of bits
- **Protocols**: Cables, signals, network cards
- **Data Unit**: Bit
- **Cybersecurity**: Physical access, signal interception

## 📚 Educational Content

### Packet Encapsulation Process

1. **Application Layer**: Creates HTTP request
2. **Presentation Layer**: Formats and encrypts data
3. **Session Layer**: Manages session information
4. **Transport Layer**: Adds TCP headers (ports, sequence numbers)
5. **Network Layer**: Adds IP headers (source/dest IP, TTL)
6. **Data Link Layer**: Adds Ethernet headers (MAC addresses)
7. **Physical Layer**: Converts to electrical/optical signals

### Common Network Attacks by Layer

| Layer | Attack Type | Example |
|-------|-------------|---------|
| 7 | Application | SQL Injection, XSS, CSRF |
| 6 | Presentation | Certificate manipulation |
| 5 | Session | Session hijacking |
| 4 | Transport | SYN floods, port scanning |
| 3 | Network | IP spoofing, routing attacks |
| 2 | Data Link | ARP spoofing, MAC flooding |
| 1 | Physical | Cable tapping, signal interception |

### Network Security Best Practices

1. **Layer 7**: Input validation, output encoding
2. **Layer 6**: Strong encryption, certificate validation
3. **Layer 5**: Secure session management
4. **Layer 4**: Firewall rules, port filtering
5. **Layer 3**: Network segmentation, IP filtering
6. **Layer 2**: VLAN isolation, MAC filtering
7. **Layer 1**: Physical security, access control

## 🔧 Troubleshooting

### Common Issues

#### Permission Denied for Packet Capture
```bash
# On Linux/macOS, run with sudo
sudo python main.py --capture

# On Windows, run as Administrator
```

#### Scapy Import Error
```bash
# Reinstall Scapy
pip uninstall scapy
pip install scapy
```

#### Rich Library Not Found
```bash
# Install Rich library
pip install rich
```

#### Traceroute Command Not Found
- **Windows**: Use `tracert` (built-in)
- **Linux**: Install `traceroute` package
- **macOS**: Use built-in `traceroute`

### Simulation Mode

If you encounter issues with real network operations, use simulation mode:

```bash
python main.py --simulate --full-journey
```

This mode works offline and provides educational value without requiring network access.

## 🤝 Contributing

We welcome contributions! Here's how you can help:

1. **Fork** the repository
2. **Create** a feature branch
3. **Make** your changes
4. **Test** thoroughly
5. **Submit** a pull request

### Development Setup

```bash
# Clone your fork
git clone <your-fork-url>
cd packet-odyssey

# Install development dependencies
pip install -r requirements.txt

# Run tests
python -m pytest tests/

# Run linting
flake8 .
```

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Scapy**: For packet manipulation capabilities
- **Rich**: For beautiful terminal interfaces
- **OSI Model**: The foundation of network communication
- **Cybersecurity Community**: For continuous learning and improvement

## 📞 Support

If you encounter any issues or have questions:

1. Check the [Troubleshooting](#-troubleshooting) section
2. Search existing [Issues](../../issues)
3. Create a new issue with detailed information

## 🎓 Learning Resources

### Recommended Reading
- "Computer Networks" by Andrew Tanenbaum
- "TCP/IP Illustrated" by W. Richard Stevens
- "Network Security Essentials" by William Stallings

### Online Courses
- Cisco CCNA Networking
- CompTIA Network+
- SANS Network Security

### Practice Labs
- TryHackMe Network Fundamentals
- HackTheBox Network Challenges
- VulnHub Network Vulnerabilities

---

**Happy Learning! 🚀**

*Packet Odyssey - Making Network Security Accessible to Everyone* 